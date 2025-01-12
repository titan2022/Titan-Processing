#include <string>
#include <thread>

#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/networking/Client.h"
#include "../../include/physics/PoseFilter.hpp"
#include "../../include/util/Config.hpp"

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient client(config.ip, config.port);
	NetworkingClient dashboardClient(config.dashboardIp, config.port);

	auto clientPoseSender = [&](Vector3D &pos, Vector3D &rot) {
		client.send_pose("pose", pos, rot);
		dashboardClient.send_pose("pose", pos, rot);
	};

	PoseFilter filter(config);
	Localizer localizer(config, filter, clientPoseSender);

	for (auto cam_tuple : config.cameras)
	{
		Camera cam = std::get<1>(cam_tuple);
		cv::VideoCapture stream(cam.openStream());
		ApriltagDetector detector(stream, true, config, localizer);

		// Multithread streams
		std::thread detectorThread(&ApriltagDetector::detect, &detector);
		detectorThread.join();
	}

	return 0;
}