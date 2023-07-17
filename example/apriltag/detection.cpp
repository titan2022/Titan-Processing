#include <iostream>
#include <string>
#include <functional>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/helper/ConfigReader.hpp"
#include "../../include/networking/Client.hpp"

void handleApriltag(const Apriltag &tag, NetworkingClient &client)
{
    std::cout << std::to_string(tag.id) << ": " << tag.position.toString() << std::endl;
    client.send_data("test", false, tag.position);
}

int main(int argc, char const *argv[])
{
    NetworkingClient client("127.0.0.1", 5800);
    ConfigReader config("example/processing.cfg");
;
    ApriltagDetector detector(0, true, config, client);

    detector.startStream();
    detector.detect(handleApriltag);
    
    return 0;
}