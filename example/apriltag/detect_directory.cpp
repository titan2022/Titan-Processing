#include <string>
#include <thread>

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"

using namespace titan;

constexpr auto IMAGE_PATH = "/media/windows/files/github/Titan-Processing-1/test/client/util/img/XYZ(-3.5,0.2,1),Rot(0,30,0).png";

/**
 * Reads images from specified directory and calculates pose
 */
int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient dashboardClient(config.udp_dashboard_ip, config.udp_port);

    cv::VideoCapture stream(IMAGE_PATH);

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

	// Start the localizer thread
	std::thread localizerThread(&Localizer::threadMainloop, std::ref(localizer));

	Camera cam = config.cameras.at("Arducam");
    ApriltagDetector detector(stream, true, config, cam, localizer);
    detector.detect();

	localizerThread.join();

	return 0;
}
