#ifndef LOCALIZER
#define LOCALIZER

#include <condition_variable>
#include <functional>
#include <optional>
#include <queue>

#include "Apriltag.hpp"
#include "physics/PoseFilter.hpp"
#include "util/Camera.hpp"
#include "util/Config.hpp"
#include "util/Vector3D.hpp"

namespace titan
{
class Localizer
{
  public:
	struct AddApriltag
	{
		int id;
		Camera &cam;
		cv::Vec3d tvec;
		cv::Vec3d rvec;
		double size;
	};
	struct LocalizerStepCommand
	{
		std::vector<AddApriltag> apriltags;
		std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> timestamp;
	};

	using PoseHandler = std::function<void(Transform3d)>;

	Localizer(Config &config, PoseFilter &filter, PoseHandler poseHandler);

	void addApriltag(int id, Camera &cam, cv::Vec3d tvec, cv::Vec3d rvec, double size, double dt);
	void step(double dt);
	Vector3D position;
	Vector3D rotation;

	void submitStepCommand(LocalizerStepCommand command);
	void threadMainloop(void);

  private:
	std::optional<Apriltag> getGlobalTag(int id);
	Config &config;
	PoseFilter &filter;
	PoseHandler poseHandler;

	std::queue<LocalizerStepCommand> commandQueue;
	std::condition_variable commandQueueCondition;
	std::mutex commandQueueMutex;
	LocalizerStepCommand popCommand(void);
};
} // namespace titan

#endif
