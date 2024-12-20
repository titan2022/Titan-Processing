#include <algorithm>
#include <iostream>
#include <string>
#include <thread>

#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/networking/Client.h"
#include "../../include/physics/PoseFilter.hpp"
#include "../../include/util/ConfigReader.hpp"
#include "util/CameraVideoStream.hpp"

using namespace titan;

// constexpr int CAM_ID = 0;
constexpr int CAM_CONFIG_INDEX = 0;
constexpr auto CONFIG_FOLDER = "../example";

/**
 * Just starts a stream with the first available camera
 */
int main(int argc, char const *argv[])
{
	std::cout << "Starting stream at camera " << CAM_CONFIG_INDEX << std::endl;

	ConfigReader config;
	config.readFromFile(CONFIG_FOLDER);
	NetworkingClient client(config.ip, config.port);
	NetworkingClient dashboardClient(config.dashboardIp, config.port);

	PoseFilter filter(config);
	Localizer localizer(config, client, dashboardClient, filter);

	// for (int i = 0; i < config.cameras.size(); i++) {
	//     config.cameras[i].id = CAM_ID;
	// }

    CameraVideoStream stream;
    stream.id = CAM_CONFIG_INDEX;
    
	ApriltagDetector detector(stream, true, config, localizer, client);
	detector.startStream();

	// Multithread streams
	std::thread detectorThread(&ApriltagDetector::detect, &detector);
	detectorThread.join();

	return 0;
}