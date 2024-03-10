#include <iostream>
#include <iomanip>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "../../include/helper/ConfigReader.hpp"

constexpr int CAM_ID = 2;
constexpr int CAM_CONFIG_INDEX = 1;

int main(int argc, char const *argv[])
{
    ConfigReader config("../example");

    cv::VideoCapture cap(CAM_ID);
    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open video capture device" << std::endl;
        return 0;
    }

    // cap.set(cv::CAP_PROP_FPS, config.cameras[CAM_CONFIG_INDEX].fps);
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, config.cameras[CAM_CONFIG_INDEX].width);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.cameras[CAM_CONFIG_INDEX].height);

    while (true)
    {
        cv::Mat frame, out;
        cap >> frame;

        cv::undistort(frame, out, config.cameras[CAM_CONFIG_INDEX].cameraMat, config.cameras[CAM_CONFIG_INDEX].distCoeffs);

        cv::imshow("Apriltag Debug Window", out);
        if (cv::waitKey(1) == 27) { // ESC key
            break;
        }
    }
    
    return 0;
}