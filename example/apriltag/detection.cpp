#include <iostream>
#include <string>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/helper/ConfigReader.hpp"
#include "../../include/networking/Client.hpp"

int main(int argc, char const *argv[])
{
    ConfigReader config("../example");
    NetworkingClient client(config.ip, config.port);
    
    Localizer localizer(config, client);
    ApriltagDetector detector(0, true, config, localizer);

    detector.startStream();
    detector.detect();
    
    return 0;
}