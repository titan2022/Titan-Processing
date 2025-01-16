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

Localizer::Localizer(Config &config, PoseFilter &filter, std::function<void(Vector3D &, Vector3D &)> poseHandler)
	: config(config), filter(filter), poseHandler(poseHandler)
{
}

// Translates position origin from camera to tag
Apriltag correctPerspective(int id, cv::Vec3d &tvec, cv::Vec3d &rvec, double size)
{
	cv::Mat R(3, 3, CV_64FC1);
	cv::Rodrigues(rvec, R);

	cv::Mat R_T = R.t();
	cv::Mat posMat = R_T * -tvec;
	cv::Vec3d posVec = posMat;

	cv::Mat rpyMat;
	cv::Mat placeholder; // OpenCV really wants me to keep this...
	cv::Vec3d rotVec = cv::RQDecomp3x3(R_T, rpyMat, placeholder);

	Vector3D pos(posVec);
	Vector3D rot(rotVec);
	rot *= Unit::DEG;

	Apriltag newTag(id, pos, rot, size);

	return newTag;
}

Apriltag toGlobalPose(Apriltag &relative, Apriltag &global)
{
	Vector3D newPos = global.position - relative.position;
	Vector3D newRot = global.rotation - relative.rotation;

	Apriltag result(relative.id, newPos, newRot, relative.size);
	return result;
}

std::optional<Apriltag> Localizer::getGlobalTag(int id)
{
	for (auto tagPair : this->config.tags)
	{
		if (tagPair.second.id == id)
		{
            // Flip axis
            Apriltag globalTag = tagPair.second;
            globalTag.position = Vector3D{globalTag.position.getX(), globalTag.position.getZ(), globalTag.position.getY()};
            globalTag.rotation = Vector3D{globalTag.rotation.getX(), globalTag.rotation.getZ(), globalTag.rotation.getY()};
			return globalTag;
		}
	}
	return {};
}

void Localizer::addApriltag(int id, Camera &cam, cv::Vec3d &tvec, cv::Vec3d &rvec, double size, double dt)
{
	// https://chatgpt.com/share/67884caa-0790-8013-a1d1-d7d813321c8c
	// Basic idea: convert the positions+orientations into 4x4 transforms, 
	// then apply the formula
	//     fieldToRobot = fieldToTag * cameraToTag.inv() * robotToCamera.inv()
	// which is equivalent to
	//     fieldToRobot = fieldToTag * tagToCamera * cameraToRobot
	// (where * means matmul).

	// Apriltag cameraToTag_Apriltag = correctPerspective(id, tvec, rvec, size);
	// cv::Mat cameraToTag = Vector3D::makeTransform(cameraToTag_Apriltag.position, cameraToTag_Apriltag.rotation);
	cv::Mat tagToCamera = Vector3D::makeTransform(tvec, rvec);
	Apriltag fieldToTag_Apriltag = getGlobalTag(id).value();
	cv::Mat fieldToTag = Vector3D::makeTransform(fieldToTag_Apriltag.position, fieldToTag_Apriltag.rotation);
	cv::Mat robotToCamera = Vector3D::makeTransform(cam.position, cam.rotation);

	cv::Mat fieldToRobot = fieldToTag * tagToCamera * robotToCamera.inv();
	Apriltag fieldToRobot_Apriltag = Apriltag(id, Vector3D::positionFromTransform(fieldToRobot), Vector3D::orientationFromTransform(fieldToRobot), size);
	
	// double tagDist = cameraToTag_Apriltag.position.getMagnitude();
	double tagDist = Vector3D(rvec).getMagnitude();
	filter.updateTag(fieldToRobot_Apriltag, tagDist, dt);
}

// Old localization code which also tried to deal with converting between WPILib and three.js coordinate systems
#if 0
void Localizer::addApriltag(int id, Camera &cam, cv::Vec3d &tvec, cv::Vec3d &rvec, double size, double dt)
{
	Apriltag invTag = correctPerspective(id, tvec, rvec, size);

	// Inverting tag position
	invTag.position *= -1;
	invTag.position.setZ(-invTag.position.getZ());
	invTag.rotation.setY(-invTag.rotation.getY());
	double tagDist = invTag.position.getMagnitude();

	// Reject tags over a certain distance (too much noise)
	// if (tagDist > 5)
	// {
	// 	return;
	// }

	// TODO: check if robot offsetting works for relative tag poses
	// Send relative tag (no Kalman filter)
	// Apriltag relTag = invTag;
	// relTag.position -= config.cameras[camId].position; // Offsetting by camera pose on the robot to get robot pose
	// relTag.rotation -= config.cameras[camId].rotation;
	// client.send_tag("tag", id, relTag.position, relTag.rotation);

    // Offset by field
    Vector3D fieldOffset(config.fieldLength / 2.0, 0, config.fieldWidth / 2.0);
	auto globTag = getGlobalTag(id);
    globTag->position -= fieldOffset;
    globTag->rotation.setY(globTag->rotation.getY() + M_PI / 2.0);

	// Rotating around tag to fit global position
	invTag.position.rotateX(globTag->rotation.getX());
	invTag.position.rotateY(globTag->rotation.getY());
	invTag.position.rotateZ(globTag->rotation.getZ());

	// Offsetting by global pose
    // The Z axis has to be flipped after rotation and before translation for some reason...
    globTag->position.setZ(-globTag->position.getZ());
	invTag.position += globTag->position;
	invTag.rotation -= globTag->rotation;

	// Offsetting by camera pose on the robot to get robot pose
	// Vector3D rotOffset = cam.position;
	// rotOffset.rotateY(-invTag.rotation.getY());
	// invTag.position += rotOffset;
    // this->poseHandler(invTag.position, invTag.rotation);
	// invTag.rotation.setY(M_PI - invTag.rotation.getY());
    // this->poseHandler(invTag.position, invTag.rotation);

    // Hacky camera offset, only works for Y-axis rotationally
    // Please fix
    Vector3D camPosOffset = cam.position;
    camPosOffset.rotateY(-invTag.rotation.getY());
    invTag.position.setX(invTag.position.getX() + camPosOffset.getX());
    invTag.position.setZ(invTag.position.getZ() + camPosOffset.getZ());

    // this->poseHandler(invTag.position, invTag.rotation);
	filter.updateTag(invTag, tagDist, dt);
}
#endif

void Localizer::step(double dt)
{
	this->poseHandler(filter.position, filter.rotation);
	filter.predict(dt);
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

		printf("[Localizer] dt = %f s, prediction: position (%f, %f, %f) rotation (%f, %f, %f)\n", 
			dt,
			filter.position.getX(), filter.position.getY(), filter.position.getZ(),
			filter.rotation.getX(), filter.rotation.getY(), filter.rotation.getZ());
	}
}

