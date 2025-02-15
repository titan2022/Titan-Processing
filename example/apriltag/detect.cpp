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

	auto clientPoseSender = [&](Transform3d pose) {
		Vector3D pos_wpilib {
			CoordinateSystem::Convert(
				pose.Translation(),
				CoordinateSystems::standard(),
				CoordinateSystems::WPILib()
			)
		};
		Vector3D rot_wpilib {
			CoordinateSystem::Convert(
				pose.Rotation(),
				CoordinateSystems::standard(),
				CoordinateSystems::WPILib()
			)
		};
		client.send_pose("pose", pos_wpilib, rot_wpilib);
		Vector3D pos_threejs {
			CoordinateSystem::Convert(
				pose.Translation(),
				CoordinateSystems::standard(),
				CoordinateSystems::THREEjs()
			)
		};
		Vector3D rot_threejs {
			CoordinateSystem::Convert(
				pose.Rotation(),
				CoordinateSystems::standard(),
				CoordinateSystems::THREEjs()
			)
		};
		dashboardClient.send_pose("pose", pos_threejs, rot_threejs);
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
		std::cout << "USAGE: ./detect                  (detect using the debug_cameras in ../config/config.json)" << "\n"
		          << "       ./detect production       (detect using the prod_cameras in ../config/config.json)" << "\n"
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

	// Determine whether we can use imshow
	char const* DISPLAY = std::getenv("DISPLAY");
	bool canUseImshow = false;
	if(DISPLAY != NULL && DISPLAY[0] != '\0') {
		canUseImshow = true;
	}

	// Start the localizer thread
	std::thread localizerThread(&Localizer::threadMainloop, std::ref(localizer));

	for (std::string cameraName : allowedCameras)
	{
		std::cout << "Starting stream for camera " << cameraName << "..." << std::endl;
		Camera cam = config.cameras[cameraName];
		cv::VideoCapture stream(cam.openStream());
		ApriltagDetector detector(stream, canUseImshow, config, cam, localizer);

		// Multithread streams
		std::thread detectorThread(&ApriltagDetector::detect, &detector);
		detectorThread.join();
	}

	localizerThread.join();

	return 0;
}