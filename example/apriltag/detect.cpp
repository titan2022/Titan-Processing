#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <string>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/apriltag/ApriltagDetector.hpp"
#include "../../include/apriltag/Localizer.hpp"
#include "../../include/helper/ConfigReader.hpp"
#include "../../include/networking/Client.h"
#include "../../include/physics/PoseFilter.hpp"

constexpr int MAX_CAMS = 4;
constexpr auto CONFIG_FOLDER = "../example";

// Getting command stdout: https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

int getCamNum()
{
    return std::stoi(exec("ls /sys/class/video4linux/ | wc -l"));
}

std::string getCamName(int id)
{
    std::string idCmd = "cat /sys/class/video4linux/video" + std::to_string(id) + "/name";
    return exec(idCmd.c_str());
}

/**
 * Stars streams with every camera available
*/
int main(int argc, char const *argv[])
{
    ConfigReader config(CONFIG_FOLDER);
    NetworkingClient client(config.ip, config.port);

    PoseFilter filter(config);
    Localizer localizer(config, client, filter);

    int camNum = std::min(MAX_CAMS, getCamNum());
    for (int i = 0; i < camNum; i++)
    {
        // Find cameras by name
        for (auto cam : config.cameras)
        {
            if (getCamName(i).find(cam.name) != std::string::npos)
            {
                cam.id = i;
                continue;
            }
        }

        ApriltagDetector detector(i, true, config, localizer);
        detector.startStream();

        // Multithread streams
        std::thread detectorThread(&ApriltagDetector::detect, &detector);
        detectorThread.join();
    }

    return 0;
}