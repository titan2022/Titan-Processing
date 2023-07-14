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
    ApriltagDetector detector(4, true);
    detector.startStream();
    detector.cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    detector.detect(handleApriltag);

    return 0;
}