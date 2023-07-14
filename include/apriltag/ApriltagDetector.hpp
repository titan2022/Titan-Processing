#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <string>

#include "Apriltag.hpp"
#include "../helper/Vector3D.hpp"
#include "opencv2/opencv.hpp"

class ApriltagDetector
{
    public:
        ApriltagDetector(int streamId, bool showWindow);
        void startStream();
        void detect(void (*handle)(const Apriltag &));
        cv::VideoCapture cap;
        double focalX = 50;
        double focalY = 50;

    private:
        int streamId;
        bool showWindow;
};

#endif