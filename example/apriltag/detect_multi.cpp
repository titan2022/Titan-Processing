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

	// Start the localizer thread
	std::thread localizerThread(&Localizer::threadMainloop, std::ref(localizer));
	
	// See https://chatgpt.com/share/678743cd-bbf0-8013-a510-c28e38695088

	// We'll create and store these so we can join them all later
	std::vector<std::thread> detectorThreads;
	// If you need to keep each ApriltagDetector alive for the whole run,
	// you can store them in a vector as well:
	std::vector<std::unique_ptr<ApriltagDetector>> detectors;

	for (std::string cameraName : allowedCameras)
	{
		printf("Starting stream for camera %s...\n", cameraName.c_str());
		Camera cam = config.cameras[cameraName];
		cv::VideoCapture stream(cam.openStream());

		// Create each ApriltagDetector on the heap or as an object
		auto detectorPtr = std::make_unique<ApriltagDetector>(stream, false, config, cam, localizer);
		// note: GUI must not be used multithreaded
		
		// Keep the detector around so it doesn't go out of scope
		detectors.push_back(std::move(detectorPtr));
		
		// Now add a new thread that calls ApriltagDetector::detect on this instance
		// detectors.back().get() returns the raw pointer to the newly created ApriltagDetector
		detectorThreads.emplace_back(&ApriltagDetector::detect, detectors.back().get());
	}

	// Once all threads are started, THEN we join them.
	// This way they all run in parallel, and only after the user is done, we wait on them here.
	for (auto &t : detectorThreads)
	{
		t.join();
	}

	localizerThread.join();

	return 0;
}