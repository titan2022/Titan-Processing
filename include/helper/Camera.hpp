#ifndef CAMERA
#define CAMERA

#include <string>

#include <opencv2/core.hpp>

class Camera
{
    public:
        std::string name;
        int id;
        int width;
        int height;
        int fps;
        int exposure;
        double focalX;
        double focalY;
        double centerX;
        double centerY;
        cv::Mat cameraMat;
        cv::Mat distCoeffs;
};

#endif