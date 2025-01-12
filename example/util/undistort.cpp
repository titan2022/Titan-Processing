#include <iostream>
#include <iomanip>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "../../include/util/ConfigReader.hpp"
#include "util/CameraVideoStream.hpp"
#include "util/VideoStream.hpp"

using namespace titan;

constexpr auto CONFIG_PATH = "../config/config.json";
constexpr auto TAGS_PATH = "../config/apriltags2025.json";


constexpr int CAM_ID = 1;
constexpr int CAM_CONFIG_INDEX = 0;

int main(int argc, char const *argv[])
{
    ConfigReader config;
    config.readFromFile(CONFIG_PATH, TAGS_PATH);

    CameraVideoStream stream;
    stream.id = CAM_CONFIG_INDEX;
    stream.initStream();

    while (true)
    {
        cv::Mat frame, out;
        frame = stream.getNextFrame();

        cv::undistort(frame, out, config.cameras[CAM_CONFIG_INDEX].cameraMat, config.cameras[CAM_CONFIG_INDEX].distCoeffs);

        cv::imshow("Apriltag Debug Window", out);
        if (cv::waitKey(1) == 27) { // ESC key
            break;
        }
    }
    
    return 0;
}