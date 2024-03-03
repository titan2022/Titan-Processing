#include <iostream>
#include <string>
#include <thread>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/helper/ConfigReader.hpp"
#include "../../include/networking/Client.h"
#include "../../include/physics/PoseFilter.hpp"

constexpr int CAM_NUM = 1; // Number of available cameras

int main(int argc, char const *argv[])
{
    ConfigReader config("../example");
    NetworkingClient client(config.ip, config.port);
    
    PoseFilter filter(config);
    Localizer localizer(config, client, filter);

    for (int i = 0; i < CAM_NUM; i++) {
        ApriltagDetector detector(i, true, config, localizer);
        detector.startStream();

        // Multithread streams
        std::thread detectorThread(&ApriltagDetector::detect, &detector);
        detectorThread.join();
    }
    
    return 0;
}