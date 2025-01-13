#ifndef LOCALIZER
#define LOCALIZER

#include <functional>

#include "../networking/Client.h"
#include "../physics/PoseFilter.hpp"
#include "../util/ConfigReader.hpp"
#include "../util/Vector3D.hpp"
#include "./Apriltag.hpp"

namespace titan
{
class Localizer
{
  public:
	Localizer(ConfigReader &config, PoseFilter &filter, std::function<void(Vector3D&, Vector3D&)> poseHandler);
	void addApriltag(int id, int camId, cv::Vec3d &tvec, cv::Vec3d &rvec, int size, double dt);
	void step(double dt);
	Vector3D position;
	Vector3D rotation;

  private:
	Apriltag *getGlobalTag(int id);
	ConfigReader &config;
	PoseFilter &filter;
    std::function<void(Vector3D&, Vector3D&)> poseHandler;
};
} // namespace titan

#endif