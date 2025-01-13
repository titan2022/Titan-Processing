#include <cmath>
#include <iostream>
#include <functional>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include "apriltag/Localizer.hpp"
#include "util/ConfigReader.hpp"
#include "util/Unit.hpp"
#include "util/Vector3D.hpp"

using namespace titan;

Localizer::Localizer(ConfigReader &config, PoseFilter &filter, std::function<void(Vector3D&, Vector3D&)> poseHandler)
	: config(config), filter(filter), poseHandler(poseHandler)
{
}

// Translates position origin from camera to tag
Apriltag correctPerspective(int id, cv::Vec3d &tvec, cv::Vec3d &rvec, int size)
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

Apriltag *Localizer::getGlobalTag(int id)
{
	for (auto tagPair : this->config.tags)
	{
		if (tagPair.second->id == id)
		{
			return tagPair.second;
		}
	}
	return nullptr;
}

void Localizer::addApriltag(int id, int camId, cv::Vec3d &tvec, cv::Vec3d &rvec, int size, double dt)
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

	// Rotating around tag to fit global position
	auto globTag = getGlobalTag(id);
	invTag.position.rotateX(globTag->rotation.getX());
	invTag.position.rotateY(globTag->rotation.getY());
	invTag.position.rotateZ(globTag->rotation.getZ());

	// Offsetting by global pose
	invTag.position += globTag->position;
	invTag.rotation += globTag->rotation;

	// Offsetting by camera pose on the robot to get robot pose
	Vector3D rotOffset = config.cameras[camId].position;
	rotOffset.rotateY(-invTag.rotation.getY());
	invTag.position += rotOffset;
	invTag.rotation.setY(M_PI - invTag.rotation.getY());

	std::cout << "robot x: " << invTag.position.getX() << std::endl;
	std::cout << "robot z: " << invTag.position.getZ() << std::endl;

	filter.updateTag(invTag, tagDist, dt);
}

void Localizer::step(double dt)
{
    this->poseHandler(filter.position, filter.rotation);
	filter.predict(dt);
}