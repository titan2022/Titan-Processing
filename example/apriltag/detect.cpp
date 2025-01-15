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

	std::vector<std::string> allowedCameras;

	if (argc == 1)
	{
		allowedCameras = config.debugCameras;
	}
	else if (argc > 2 || strcmp(argv[1], "--help") == 0)
	{
		std::cout << "USAGE: ./detect                  (detect using the debugCameras in ../config/config.json)" << "\n"
		          << "       ./detect production       (detect using the prodCameras in ../config/config.json)" << "\n"
				  << "       ./detect [camera name]    (detect using the named camera)" << std::endl;
	}
	else if (strcmp(argv[1], "production") == 0)
	{
		allowedCameras = config.prodCameras;
	}
	else
	{
		allowedCameras = { argv[1] };
	}

	for (std::string cameraName : allowedCameras)
	{
		Camera cam = config.cameras[cameraName];
		cv::VideoCapture stream(cam.openStream());
		ApriltagDetector detector(stream, true, config, localizer);

		// Multithread streams
		std::thread detectorThread(&ApriltagDetector::detect, &detector);
		detectorThread.join();
	}

	return 0;
}