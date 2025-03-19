#include <cmath>
#include <functional>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <optional>

#include "apriltag/Localizer.hpp"
#include "util/Camera.hpp"
#include "util/Config.hpp"
#include "util/Unit.hpp"
#include "util/Vector3D.hpp"

using namespace titan;

Localizer::Localizer(Config &config, PoseFilter &filter, PoseHandler poseHandler)
	: config(config), filter(filter), poseHandler(poseHandler)
{
}

std::optional<Apriltag> Localizer::getGlobalTag(int id)
{
	for (auto tagPair : this->config.tags)
	{
		if (tagPair.second.id == id)
		{
            Apriltag globalTag = tagPair.second;
			return globalTag;
		}
	}
	return {};
}

void Localizer::addApriltag(int id, Camera &cam, cv::Vec3d tvec, cv::Vec3d rvec, double size, double dt)
{
	cv::Mat rmat_cv;
	cv::Rodrigues(rvec, rmat_cv);
	Eigen::Matrix3d rmat_eigen = (Eigen::Matrix3d() <<
		rmat_cv.at<double>(0,0), rmat_cv.at<double>(0,1), rmat_cv.at<double>(0,2),
		rmat_cv.at<double>(1,0), rmat_cv.at<double>(1,1), rmat_cv.at<double>(1,2),
		rmat_cv.at<double>(2,0), rmat_cv.at<double>(2,1), rmat_cv.at<double>(2,2)
	).finished();

	Transform3d tagInCameraFrame = CoordinateSystem::Convert(
		Transform3d{
			Translation3d{Eigen::Vector3d{tvec[0], tvec[1], tvec[2]}},
			Rotation3d{rmat_eigen}
		},
		CoordinateSystems::OpenCV(),
		CoordinateSystems::standard()
	);

	Apriltag tagInFieldFrame_Apriltag = getGlobalTag(id).value();
	Transform3d tagInFieldFrame = Transform3d{tagInFieldFrame_Apriltag.position, tagInFieldFrame_Apriltag.rotation};
	
	Transform3d cameraInRobotFrame = Transform3d{cam.position, cam.rotation};

	Transform3d robotInFieldFrame = tagInFieldFrame + tagInCameraFrame.Inverse() + cameraInRobotFrame.Inverse();

	Apriltag robotInFieldFrame_Apriltag = Apriltag(id, robotInFieldFrame.Translation(), robotInFieldFrame.Rotation(), size);
	
	double tagDist = cv::norm(tvec);

	printf("[Localizer] %s (path: %s), view of apriltag %d: position (%f, %f, %f) rotation (%f, %f, %f)\n",
		cam.name.c_str(), 
        cam.cameraPath.c_str(),
        id,
		robotInFieldFrame.Translation().X().value(), 
		robotInFieldFrame.Translation().Y().value(), 
		robotInFieldFrame.Translation().Z().value(),
		units::degree_t{robotInFieldFrame.Rotation().X()}.value(),
		units::degree_t{robotInFieldFrame.Rotation().Y()}.value(),
		units::degree_t{robotInFieldFrame.Rotation().Z()}.value());
	filter.updateTag(robotInFieldFrame_Apriltag, tagDist, dt);
	this->poseHandler(robotInFieldFrame);
}

void Localizer::step(double dt)
{
	filter.predict(dt);
	// this->poseHandler(filter.position, filter.rotation.coerceToVector3D());
}

void Localizer::submitStepCommand(LocalizerStepCommand command)
{
	std::unique_lock<std::mutex> lock(commandQueueMutex);
	commandQueue.push(command);
	commandQueueCondition.notify_all();
	// The lock is automatically released when it goes out of scope.
}

Localizer::LocalizerStepCommand Localizer::popCommand(void)
{
	std::unique_lock<std::mutex> lock(commandQueueMutex);
	Localizer::LocalizerStepCommand retval = commandQueue.front();
	commandQueue.pop();
	return retval;
	// The lock is automatically released when it goes out of scope.
}

void Localizer::threadMainloop()
{
	auto prevTS = std::chrono::steady_clock::now();
	auto postTS = prevTS;
	double dt = 0;

	printf("[Localizer] mainloop started\n");

	while (true)
	{
		// Wait for commands to be on the queue
		// std::unique_lock<std::mutex> lock(commandQueueMutex);
		// while(commandQueue.empty()) { commandQueueCondition.wait(lock); }
		while(commandQueue.empty()) { __asm__(""); }

		Localizer::LocalizerStepCommand command = popCommand();

		// Measure delta time
		postTS = command.timestamp;
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(postTS - prevTS).count() / 1000.0;
		prevTS = postTS;

		for(Localizer::AddApriltag apriltag : command.apriltags)
		{
			addApriltag(apriltag.id, apriltag.cam, apriltag.tvec, apriltag.rvec, apriltag.size, dt);
		}

		step(dt);

		// printf("[Localizer] dt = %f s, prediction: position (%f, %f, %f) rotation (%f, %f, %f)\n", 
		// 	dt,
		// 	filter.position.getX(), filter.position.getY(), filter.position.getZ(),
		// 	filter.rotation.x, filter.rotation.y, filter.rotation.z);
	}
}

