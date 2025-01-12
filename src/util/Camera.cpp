#include "util/Camera.hpp"
#include <opencv2/core.hpp>
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