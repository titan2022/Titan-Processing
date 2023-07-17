#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <string>

#include "Apriltag.hpp"
#include "opencv2/opencv.hpp"
#include "../helper/ConfigReader.hpp"
#include "../helper/Vector3D.hpp"

class ApriltagDetector
{
    public:
        ApriltagDetector(int streamId, ConfigReader config, bool showWindow);
        void startStream();
        void detect(void (*handle)(const Apriltag &));
        cv::VideoCapture cap;

    private:
        int streamId;
        bool showWindow;
        ConfigReader config;
};

#endif