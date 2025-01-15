#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <thread>

#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/networking/Client.h"
#include "../../include/physics/PoseFilter.hpp"
#include "../../include/util/Config.hpp"

using namespace titan;

constexpr auto IMAGE_PATH = "/media/windows/files/github/Titan-Processing-1/test/client/util/img/XYZ(-3.5,0.2,1),Rot(0,30,0).png";

/**
 * Reads images from specified directory and calculates pose
 */
int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient dashboardClient(config.dashboardIp, config.port);

    cv::VideoCapture stream(IMAGE_PATH);

	auto clientPoseSender = [&](Vector3D &pos, Vector3D &rot) {
        std::cout << "pos: " << pos.toString() << std::endl;
		dashboardClient.send_pose("pose", pos, rot);
	};

	PoseFilter filter(config);
	Localizer localizer(config, filter, clientPoseSender);

	// Start the localizer thread
	std::thread localizerThread(&Localizer::threadMainloop, std::ref(localizer));
	localizerThread.join();

	Camera cam = config.cameras.at("Arducam");
    ApriltagDetector detector(stream, true, config, cam, localizer);
    detector.detect();

	return 0;
}