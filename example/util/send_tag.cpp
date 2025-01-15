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
    clientPoseSender(tag.position, tag.rotation);
}