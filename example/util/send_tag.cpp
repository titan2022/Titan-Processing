#include <string>
#include <thread>

#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
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

    int tagNumber;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
	{
		std::cout << "USAGE: ./send_tag [tag number]" << std::endl;
        return 1;
	}
	else
	{
		tagNumber = atoi(argv[1]);
	}

	Apriltag tag = config.tags.at(tagNumber);
    clientPoseSender({Transform3d{tag.position, tag.rotation}});
}
