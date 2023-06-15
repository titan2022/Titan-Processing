#include <iostream>
#include <string>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/apriltag/ApriltagDetector.hpp"

void handleApriltag(const Apriltag &tag)
{
    std::cout << std::to_string(tag.id) << ": " << tag.position->toString() << std::endl;
}

int main(int argc, char const *argv[])
{
    ApriltagDetector detector(0, true);
    detector.detect(handleApriltag);

    return 0;
}