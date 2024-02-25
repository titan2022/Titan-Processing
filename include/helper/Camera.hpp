#ifndef CAMERA
#define CAMERA

#include <opencv2/core.hpp>

class Camera
{
    public:
        // Camera();

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