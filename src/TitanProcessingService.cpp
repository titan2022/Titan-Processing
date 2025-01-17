#include <functional>
#include <string>
#include <thread>

#include "../include/apriltag/ApriltagDetector.hpp"
#include "../include/apriltag/Localizer.hpp"
#include "../include/networking/Client.h"
#include "../include/physics/PoseFilter.hpp"
#include "../include/util/Config.hpp"
#include "util/Vector3D.hpp"
#include <../include/apriltag/Apriltag.hpp>

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	std::cout << "Starting service..." << std::endl;

	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient client(config.ip, config.port);
	NetworkingClient dashboardClient(config.dashboardIp, config.port);

	auto clientPoseSender = [&](Vector3D pos, Vector3D rot) {
		client.send_pose("pose", pos, rot);
		dashboardClient.send_pose("pose", pos, rot);
	};

	PoseFilter filter(config);
	Localizer localizer(config, filter, clientPoseSender);

	for (auto cam_tuple : config.cameras)
	{
		Camera cam = std::get<1>(cam_tuple);
		std::cout << "Initializing camera: " << cam.name << std::endl;
		cv::VideoCapture stream = cam.openStream();
		ApriltagDetector detector(stream, false, config, cam, localizer);

		// Multithread streams
		std::thread detectorThread(&ApriltagDetector::detect, &detector);
		detectorThread.join();
	}

	return 0;
}