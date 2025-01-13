#ifndef CAMERA
#define CAMERA

#include <opencv2/videoio.hpp>
#include <string>

#include "./Vector3D.hpp"
#include <opencv2/core.hpp>

namespace titan
{
class Camera
{
  public:
	std::string name;
	std::string usbName;
	Vector3D position;
	Vector3D rotation;
	int width;
	int height;
	int fps;
	int exposure;
	double focalX;
	double focalY;
	double centerX;
	double centerY;
	cv::Mat cameraMat;
	cv::Mat distCoeffs;
	cv::VideoCapture openStream();
	/// Gets the camera intrinsic matrix
	cv::Mat getCameraMatrix();
	/// Sets the camera intrinsic matrix
	void setCameraMatrix(cv::Mat matrix);
};
} // namespace titan

#endif