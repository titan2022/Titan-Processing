#include "util/Camera.hpp"
#include "util/Unit.hpp"
#include <cassert>
#include <nlohmann/json_fwd.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <stdexcept>
#include <string>

using namespace titan;

cv::VideoCapture Camera::openStream()
{
	auto cap =
		cv::VideoCapture("v4l2src device=/dev/v4l/by-id/" + usbName + " ! image/jpeg framerate=" + std::to_string(fps) +
							 " ! decodebin ! videoconvert ! appsink",
						 cv::CAP_GSTREAMER);
	if (!cap.isOpened())
	{
		throw std::runtime_error(std::string("Couldn't open camera at /dev/v4l/by-id/") + usbName);
	}

	return cap;
}
Camera Camera::fromJson(nlohmann::json json)
{

	std::vector<double> posArr = json["position"];
	std::vector<double> rotArr = json["rotation"];
	Vector3D pos(posArr);
	Vector3D rot(rotArr);
	rot *= Unit::DEG;

	cv::Matx<double, 3, 3> cameraMat(json["focalX"], 0, json["centerX"], 0, json["focalY"], json["centerY"], 0, 0, 1);
	cv::Matx<double, 1, 5> distCoeffs(json["k1"], json["k2"], json["p1"], json["p2"], json["k3"]);

	Camera cam = {
		.name = json["name"],
		.usbName = json["usbName"],
		.position = pos,
		.rotation = rot,
		.width = json["width"],
		.height = json["height"],
		.fps = json["fps"],
		.exposure = json["exposure"],
		.cameraMat = cameraMat,
		.distCoeffs = distCoeffs,
	};
	return cam;
}
nlohmann::json Camera::toJson()
{
	nlohmann::json json = {
		{"name", name},
		{"usbName", usbName},
		{"position",
		 {
			 position.getX(),
			 position.getY(),
			 position.getZ(),
		 }},
		{"rotation",
		 {
			 rotation.getX() * Unit::RAD,
			 rotation.getY() * Unit::RAD,
			 rotation.getZ() * Unit::RAD,
		 }},
		{"width", width},
		{"height", height},
		{"fps", fps},
		{"exposure", exposure},
		{"focalX", cameraMat(0, 0)},
		{"focalY", cameraMat(1, 1)},
		{"centerX", cameraMat(0, 2)},
		{"centerY", cameraMat(1, 2)},
		{"k1", distCoeffs(0, 0)},
		{"k2", distCoeffs(0, 1)},
		{"p1", distCoeffs(0, 2)},
		{"p2", distCoeffs(0, 3)},
		{"k3", distCoeffs(0, 4)},
	};
	return json;
}