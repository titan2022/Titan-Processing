#ifndef CAMERA
#define CAMERA

#include "./Vector3D.hpp"
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <string>

namespace titan
{
class Camera
{
  public:
	std::string name;
	std::string cameraPath;
	Translation3d position;
	Rotation3d rotation;
	int width;
	int height;
	int fps;
	int exposure;
	cv::Matx<double, 3, 3> cameraMat;
	cv::Matx<double, 1, 5> distCoeffs;
	cv::VideoCapture openStream();
	static Camera fromJson(nlohmann::json json);
	nlohmann::json toJson();
};
} // namespace titan

#endif