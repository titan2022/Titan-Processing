#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <string>

#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/helper/ConfigReader.hpp"
#include "../../include/networking/Client.h"
#include "../../include/physics/PoseFilter.hpp"

constexpr int CAM_ID = 0;
constexpr auto CONFIG_FOLDER = "../example";

/**
 * Just starts a stream with the first available camera
*/
int main(int argc, char const *argv[])
{
    ConfigReader config(CONFIG_FOLDER);
    NetworkingClient client(config.ip, config.port);

    PoseFilter filter(config);
    Localizer localizer(config, client, filter);

    for (auto cam : config.cameras) {
        cam->id = CAM_ID;
    }
    ApriltagDetector detector(CAM_ID, true, config, localizer);
    detector.startStream();

    // Multithread streams
    std::thread detectorThread(&ApriltagDetector::detect, &detector);
    detectorThread.join();

    return 0;
}