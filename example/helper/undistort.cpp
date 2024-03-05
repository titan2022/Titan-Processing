#include <iostream>
#include <iomanip>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "../../include/helper/ConfigReader.hpp"

int main(int argc, char const *argv[])
{
    ConfigReader config("../example");

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open video capture device" << std::endl;
        return 0;
    }

    cap.set(cv::CAP_PROP_FPS, config.cameras[0]->fps);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, config.cameras[0]->width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.cameras[0]->height);

    while (true)
    {
        cv::Mat frame, out;
        cap >> frame;

        cv::undistort(frame, out, config.cameras[0]->cameraMat, config.cameras[0]->distCoeffs);

        cv::imshow("Apriltag Debug Window", out);
        if (cv::waitKey(1) == 27) { // ESC key
            break;
        }
    }
    
    return 0;
}