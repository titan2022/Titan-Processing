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
	// Test this using gst-launch-1.0
	auto cap =
		cv::VideoCapture("v4l2src device=" + cameraPath + " ! image/jpeg,framerate=" + std::to_string(fps) + "/1" +
							 " ! decodebin ! videoconvert ! appsink",
						 cv::CAP_GSTREAMER);
		// cv::VideoCapture("udpsrc port=8081 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink",
			// cv::CAP_GSTREAMER);
	if (!cap.isOpened())
	{
		throw std::runtime_error(std::string("Couldn't open camera. To debug, try running `gst-launch-1.0 ") + 
			"v4l2src device=" + cameraPath + " ! image/jpeg,framerate=" + std::to_string(fps) + "/1" +
							 " ! decodebin ! videoconvert ! autovideosink`.");
	}

	return cap;
}
Camera Camera::fromJson(nlohmann::json json)
{

	std::vector<double> posArr = json["position"];
	std::vector<double> rotArr = json["rotation"];
	Translation3d pos {
		units::meter_t{posArr[0]},
		units::meter_t{posArr[1]},
		units::meter_t{posArr[2]}
	};
	Vector3D rot_vec3d(rotArr);
	rot_vec3d *= Unit::DEG;
	Rotation3d rot = Rotation3d{
		units::degree_t{rotArr[0]},
		units::degree_t{rotArr[1]},
		units::degree_t{rotArr[2]}
	};

	cv::Matx<double, 3, 3> cameraMat(json["focalX"], 0, json["centerX"], 0, json["focalY"], json["centerY"], 0, 0, 1);
	cv::Matx<double, 1, 5> distCoeffs(json["k1"], json["k2"], json["p1"], json["p2"], json["k3"]);

	Camera cam = {
		.name = json["name"],
		.cameraPath = json["cameraPath"],
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
		{"cameraPath", cameraPath},
		{"position",
		 {
			 position.X().value(),
			 position.Y().value(),
			 position.Z().value(),
		 }},
		{"rotation",
		 {
			 units::degree_t{rotation.X()}.value(),
			 units::degree_t{rotation.Y()}.value(),
			 units::degree_t{rotation.Z()}.value(),
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