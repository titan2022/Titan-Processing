#ifndef LOCALIZER
#define LOCALIZER

#include <functional>

#include "../physics/PoseFilter.hpp"
#include "../util/Config.hpp"
#include "../util/Vector3D.hpp"
#include "./Apriltag.hpp"
#include "util/Camera.hpp"
#include <optional>

namespace titan
{
class Localizer
{
  public:
	Localizer(Config &config, PoseFilter &filter, std::function<void(Vector3D &, Vector3D &)> poseHandler);
	void addApriltag(int id, Camera &cam, cv::Vec3d &tvec, cv::Vec3d &rvec, int size, double dt);
	void step(double dt);
	Vector3D position;
	Vector3D rotation;

  private:
	std::optional<Apriltag> getGlobalTag(int id);
	Config &config;
	PoseFilter &filter;
	std::function<void(Vector3D &, Vector3D &)> poseHandler;
};
} // namespace titan

#endif