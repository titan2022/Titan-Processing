#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <string>

#include "Apriltag.hpp"
#include "opencv2/opencv.hpp"

#include "../apriltag/Localizer.hpp"
#include "../helper/ConfigReader.hpp"
#include "../helper/Vector3D.hpp"
#include "../networking/Client.hpp"

class ApriltagDetector
{
    public:
        ApriltagDetector(int streamId, bool showWindow, ConfigReader &config, Localizer &localizer);
        void startStream();
        void detect();
        cv::VideoCapture cap;

    private:
        int streamId;
        bool showWindow;
        ConfigReader &config;
        Localizer &localizer;
};

#endif