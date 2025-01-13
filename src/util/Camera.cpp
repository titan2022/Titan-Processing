#include "util/Camera.hpp"
#include <cassert>
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
cv::Mat Camera::getCameraMatrix()
{
	cv::Mat mat(3, 3, CV_64F);
	mat.at<double>(0, 0) = focalX;
	mat.at<double>(1, 1) = focalY;
	mat.at<double>(0, 2) = centerX;
	mat.at<double>(1, 2) = centerY;
	mat.at<double>(2, 2) = 1;
	return mat;
}
void Camera::setCameraMatrix(cv::Mat mat)
{
	assert(mat.dims == 2);
	assert(mat.rows == 3);
	assert(mat.columns == 3);

	focalX = mat.at<double>(0, 0);
	focalY = mat.at<double>(1, 1);
	centerX = mat.at<double>(0, 2);
	centerY = mat.at<double>(1, 2);
}