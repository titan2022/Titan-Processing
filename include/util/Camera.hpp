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
	std::string usbName;
	Vector3D position;
	Vector3D rotation;
	int width;
	int height;
	int fps;
	int exposure;
	cv::Matx<double, 3, 3> cameraMat;
	cv::Matx<double, 1, 5> distCoeffs;
	[[nodiscard]] auto openStream() const -> cv::VideoCapture;
	static auto fromJson(nlohmann::json &json) -> Camera;
	[[nodiscard]] auto toJson() const -> nlohmann::json;
};
} // namespace titan

#endif