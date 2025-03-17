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

	// Get the table
	std::shared_ptr<nt::NetworkTable> networkTable = ntInstance.GetTable("TitanProcessing");
	// get a topic from a NetworkTable
	// the topic name in this case is the name within the table;
	// this line and the one above reference the same topic
	nt::StructTopic<Pose3d> poseTopic = networkTable->GetStructTopic<Pose3d>("pose");
	nt::StructPublisher<Pose3d> posePublisher = poseTopic.Publish();

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

		posePublisher.Set(Pose3d{pose.Translation(), pose.Rotation()});
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
