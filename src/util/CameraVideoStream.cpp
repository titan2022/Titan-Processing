#include "util/Camera.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <util/CameraVideoStream.hpp>

using namespace titan;

int CameraVideoStream::initStream()
{
	if (config == nullptr)
	{
        std::cerr << "No config in `CameraVideoStream` object defined." << std::endl;
        return 1;
	}

    if (cameraIndex < 0)
	{
        std::cerr << "No camera ID in `CameraVideoStream` object defined." << std::endl;
        return 1;
	}

	Camera cam = config.get()->cameras[cameraIndex];

	std::string cameraPipeline =
		"v4l2src device=/dev/v4l/by-id/" + cam.usbName + 
        " ! image/jpeg, width=" + std::to_string(cam.width) +
        ", height=" + std::to_string(cam.height) +
		", framerate=" + std::to_string(cam.fps) +
		"/1 ! decodebin ! videoconvert ! appsink";
	cv::VideoCapture cap(0, cv::CAP_GSTREAMER);

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

bool CameraVideoStream::isOpened() {
    return cap.isOpened();
}

int CameraVideoStream::getWidth() {
    return cap.get(cv::CAP_PROP_FRAME_WIDTH);
}

int CameraVideoStream::getHeight() {
    return cap.get(cv::CAP_PROP_FRAME_HEIGHT);
}

int CameraVideoStream::getFPS() {
    return cap.get(cv::CAP_PROP_FPS);
}