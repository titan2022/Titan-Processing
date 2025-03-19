#include <string>
#include <thread>

#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"

#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTable.h>
#include <networktables/StructTopic.h>

using namespace titan;
using namespace nt;

using frc::Pose3d;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient client(config.udp_roborio_ip, config.udp_port);
	NetworkingClient dashboardClient(config.udp_dashboard_ip, config.udp_port);

	nt::NetworkTableInstance ntInstance = nt::NetworkTableInstance::GetDefault();
	// start a NT4 client
	ntInstance.StartClient4("Titan-Processing");
	// connect to a specific host/port
	ntInstance.SetServer(config.nt_server_ip, config.nt_port);
    ntInstance.SetServerTeam(2022);

	// Get the table
	std::shared_ptr<nt::NetworkTable> networkTable = ntInstance.GetTable("TitanProcessing");
	// get a topic from a NetworkTable
	// the topic name in this case is the name within the table;
	// this line and the one above reference the same topic
	nt::StructTopic<Pose3d> poseTopic = networkTable->GetStructTopic<Pose3d>("pose");
	nt::StructPublisher<Pose3d> posePublisher = poseTopic.Publish();

	auto clientPoseSender = [&](titan::Localizer::PoseHandlerArgs args) {
		Vector3D pos_wpilib {
			CoordinateSystem::Convert(
				args.pose.Translation(),
				CoordinateSystems::standard(),
				CoordinateSystems::WPILib()
			)
		};
		Vector3D rot_wpilib {
			CoordinateSystem::Convert(
				args.pose.Rotation(),
				CoordinateSystems::standard(),
				CoordinateSystems::WPILib()
			)
		};
		client.send_pose("args.pose", pos_wpilib, rot_wpilib, args.distanceToTag);
		Vector3D pos_threejs {
			CoordinateSystem::Convert(
				args.pose.Translation(),
				CoordinateSystems::standard(),
				CoordinateSystems::THREEjs()
			)
		};
		Vector3D rot_threejs {
			CoordinateSystem::Convert(
				args.pose.Rotation(),
				CoordinateSystems::standard(),
				CoordinateSystems::THREEjs()
			)
		};
		dashboardClient.send_pose("args.pose", pos_threejs, rot_threejs, args.distanceToTag);

		posePublisher.Set(Pose3d{args.pose.Translation(), args.pose.Rotation()});
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
