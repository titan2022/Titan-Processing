#include "util/Camera.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <util/CameraVideoStream.hpp>
#include "util/VideoStream.hpp"

using namespace titan;

int CameraVideoStream::initStream()
{
	if (config == nullptr)
	{
        std::cerr << "No config in `CameraVideoStream` object defined." << std::endl;
        return 1;
	}

    if (id < 0)
	{
        std::cerr << "No camera ID in `CameraVideoStream` object defined." << std::endl;
        return 1;
	}

	Camera cam = config.get()->cameras[id];

	std::string cameraPipeline;
	cameraPipeline =
		"v4l2src device=/dev/v4l/by-id/" + cam.usbName +
		" ! "
		// "videorate ! videoconvert ! videoscale !"
		// "video/x-raw, format=BGR, width=" + std::to_string(cam.width) + ", height=" + std::to_string(cam.height) + ",
		// pixel-aspect-ratio=1/1, framerate=" + std::to_string(cam.fps) + "/1 ! "
		"image/jpeg, width=" +
		std::to_string(cam.width) + ", height=" + std::to_string(cam.height) +
		", framerate=" + std::to_string(cam.fps) +
		"/1 ! "
		"decodebin ! videoconvert ! appsink";
	cv::VideoCapture cap(cameraPipeline);

	this->cap = cap;
	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open video capture device." << std::endl;
		return 1;
	}

	return 0;
}

cv::Mat CameraVideoStream::getNextFrame()
{
    cv::Mat frame;
    cap >> frame;
    return frame;
}