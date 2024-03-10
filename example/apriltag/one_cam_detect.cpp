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

constexpr int CAM_ID = 2;
constexpr int CAM_CONFIG_INDEX = 1;
constexpr auto CONFIG_FOLDER = "../example";

/**
 * Just starts a stream with the first available camera
*/
int main(int argc, char const *argv[])
{
    std::cout << "Starting stream at camera " << CAM_ID << std::endl;

    ConfigReader config(CONFIG_FOLDER);
    NetworkingClient client(config.ip, config.port);

    PoseFilter filter(config);
    Localizer localizer(config, client, filter);

    for (int i = 0; i < config.cameras.size(); i++) {
        config.cameras[i].id = CAM_ID;
    }

    ApriltagDetector detector(CAM_CONFIG_INDEX, true, config, localizer);
    detector.startStream();

    // Multithread streams
    std::thread detectorThread(&ApriltagDetector::detect, &detector);
    detectorThread.join();

    return 0;
}