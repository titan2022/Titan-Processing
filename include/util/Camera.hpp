#ifndef CAMERA
#define CAMERA

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
	int id;
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
};
} // namespace titan

#endif