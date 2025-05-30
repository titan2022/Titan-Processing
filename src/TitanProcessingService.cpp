#include <functional>
#include <string>
#include <thread>

#include "apriltag/Apriltag.hpp"
#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"
#include "util/Vector3D.hpp"

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	std::cout << "Starting service..." << std::endl;

	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient client(config.udp_roborio_ip, config.udp_port);
	NetworkingClient dashboardClient(config.udp_dashboard_ip, config.udp_port);

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
		std::cout << "pos: " << pos_wpilib.toString() << std::endl;
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
		dashboardClient.send_pose("pose", pos_threejs, rot_threejs);
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
