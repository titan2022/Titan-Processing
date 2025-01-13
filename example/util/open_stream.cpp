#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "../../include/util/ConfigReader.hpp"
#include "util/CameraVideoStream.hpp"

using namespace titan;

constexpr auto CONFIG_PATH = "../config/config.json";
constexpr auto TAGS_PATH = "../config/apriltags2025.json";

constexpr int CAM_INDEX = 0;

int main(int argc, char const *argv[])
{
    ConfigReader config;
    config.readFromFile(CONFIG_PATH, TAGS_PATH);

    CameraVideoStream stream;
    stream.config = std::make_shared<ConfigReader>(config);
    stream.cameraIndex = CAM_INDEX;
    stream.initStream();

    while (true)
    {
        cv::Mat frame;
        frame = stream.getNextFrame();

        cv::imshow("Debug Window", frame);
        if (cv::waitKey(1) == 27) // ESC key
        {
            break;
        }
    }
    
    return 0;
}