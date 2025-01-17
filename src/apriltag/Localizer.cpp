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

Localizer::Localizer(Config &config, PoseFilter &filter, std::function<void(Vector3D, Vector3D)> poseHandler)
	: config(config), filter(filter), poseHandler(poseHandler)
{
}

// Translates position origin from camera to tag
// UNUSED - Determine whether this is still useful...
# if 0
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
#endif

// FIXME - What is this for? Is it really valid to subtract Euler angles?
#if 0
Apriltag toGlobalPose(Apriltag &relative, Apriltag &global)
{
	Vector3D newPos = global.position - relative.position;
	Vector3D newRot = global.rotation - relative.rotation;

	Apriltag result(relative.id, newPos, newRot, relative.size);
	return result;
}
#endif

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
	RotationMatrix tagInCameraFrame_orientation = RotationMatrix::fromRotationVector(rvec, CoordinateSystem::OPENCV);
	Transform tagInCameraFrame = Transform(
		Translation(tvec, CoordinateSystem::OPENCV).convertToCoordinateSystem(CoordinateSystem::THREEJS), 
		tagInCameraFrame_orientation.convertToCoordinateSystem(CoordinateSystem::THREEJS));

	Apriltag tagInFieldFrame_Apriltag = getGlobalTag(id).value();
	Transform tagInFieldFrame = Transform(tagInFieldFrame_Apriltag.position, tagInFieldFrame_Apriltag.rotation.toRotationMatrix());
	
	Transform cameraInRobotFrame = Transform(cam.position, cam.rotation.toRotationMatrix());

	Transform robotInFieldFrame = tagInFieldFrame * tagInCameraFrame.inv() * cameraInRobotFrame.inv();

	Translation robotPosition = robotInFieldFrame.getPosition();

	// We need to pitch the predicted robot 180 degrees, then yaw the predicted robot 180 degrees, to make it face the AprilTag if 
	// the front camera is looking at the AprilTag.
	RotationMatrix pitch180 = EulerAngles(M_PI, 0, 0).toRotationMatrix();
	RotationMatrix yaw180 = EulerAngles(0, M_PI, 0).toRotationMatrix();
	RotationMatrix robotOrientation = (robotInFieldFrame.getOrientation() * pitch180) * yaw180;

	EulerAngles robotAngles = robotOrientation.toEulerAngles();

	// We need to flip the pitch of the robot across the horizontal plane.
	robotAngles.x = -robotAngles.x;
	// We need to flip the roll of the robot.
	robotAngles.z = -robotAngles.z;

	Apriltag robotInFieldFrame_Apriltag = Apriltag(id, robotPosition, robotAngles, size);
	
	double tagDist = cv::norm(tvec);

	printf("[Localizer] %s view of apriltag %d: position (%f, %f, %f) rotation (%f, %f, %f)\n",
		cam.name.c_str(), id,
		robotInFieldFrame_Apriltag.position.getX(), robotInFieldFrame_Apriltag.position.getY(), robotInFieldFrame_Apriltag.position.getZ(),
		robotInFieldFrame_Apriltag.rotation.x, robotInFieldFrame_Apriltag.rotation.y, robotInFieldFrame_Apriltag.rotation.z);
	filter.updateTag(robotInFieldFrame_Apriltag, tagDist, dt);
	this->poseHandler(robotInFieldFrame_Apriltag.position, robotInFieldFrame_Apriltag.rotation.coerceToVector3D());
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

		printf("[Localizer] dt = %f s, prediction: position (%f, %f, %f) rotation (%f, %f, %f)\n", 
			dt,
			filter.position.getX(), filter.position.getY(), filter.position.getZ(),
			filter.rotation.x, filter.rotation.y, filter.rotation.z);
	}
}

