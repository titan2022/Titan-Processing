#include <string>
#include <thread>

#include "../include/apriltag/ApriltagDetector.hpp"
#include "../include/apriltag/Localizer.hpp"
#include "../include/networking/Client.h"
#include "../include/physics/PoseFilter.hpp"
#include "../include/util/ConfigReader.hpp"
#include "util/CameraVideoStream.hpp"
#include "util/VideoStream.hpp"
#include <../include/apriltag/Apriltag.hpp>

using namespace titan;

constexpr auto CONFIG_PATH = "../config/config.json";
constexpr auto TAGS_PATH = "../config/apriltags2025.json";

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	ConfigReader config;
	config.readFromFile(CONFIG_PATH, TAGS_PATH);
	NetworkingClient client(config.ip, config.port);
	NetworkingClient dashboardClient(config.dashboardIp, config.port);

	PoseFilter filter(config);
	Localizer localizer(config, client, dashboardClient, filter);

	for (int i = 0; i < config.cameras.size(); i++)
	{
		Camera cam = config.cameras[i];
        CameraVideoStream stream;
        stream.id = i;

		ApriltagDetector detector(std::make_shared<CameraVideoStream>(stream), false, config, localizer, client);
		detector.startStream();

		// Multithread streams
		std::thread detectorThread(&ApriltagDetector::detect, &detector);
		detectorThread.join();
	}

	return 0;
}