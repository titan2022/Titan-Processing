#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <util/FileVideoStream.hpp>

using namespace titan;

int FileVideoStream::initStream() {
    if (config == nullptr)
	{
        std::cerr << "No config in `FileVideoStream` object defined." << std::endl;
        return 1;
	}

    if (filePath.length() == 0)
	{
        std::cerr << "No video file path in `FileVideoStream` object defined." << std::endl;
        return 1;
	}

    cv::VideoCapture cap(filePath);
    this->cap = cap;
	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open video capture device." << std::endl;
		return 1;
	}

    return 0;
}

cv::Mat FileVideoStream::getNextFrame()
{
    cv::Mat frame;
    cap >> frame;
    cv::VideoCapture* thing = new cv::VideoCapture(1);
    return frame;
}

bool FileVideoStream::isOpened() {
    return cap.isOpened();
}

int FileVideoStream::getWidth() {
    return cap.get(cv::CAP_PROP_FRAME_WIDTH);
}

int FileVideoStream::getHeight() {
    return cap.get(cv::CAP_PROP_FRAME_HEIGHT);
}

int FileVideoStream::getFPS() {
    return cap.get(cv::CAP_PROP_FPS);
}