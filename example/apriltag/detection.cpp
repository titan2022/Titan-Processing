#include <iostream>
#include <string>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/helper/ConfigReader.hpp"

void handleApriltag(const Apriltag &tag)
{
    std::cout << std::to_string(tag.id) << ": " << tag.position->toString() << std::endl;
}

int main(int argc, char const *argv[])
{
    ConfigReader config("example/processing.cfg");
    ApriltagDetector detector(0, config, true);

    detector.startStream();
    detector.detect(handleApriltag);

    return 0;
}