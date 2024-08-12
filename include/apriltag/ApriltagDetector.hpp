#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <string>

#include "Apriltag.hpp"
#include "opencv2/opencv.hpp"

#include "../apriltag/Localizer.hpp"
#include "../util/ConfigReader.hpp"
#include "../util/Vector3D.hpp"
#include "../networking/Client.h"

class ApriltagDetector
{
    public:
        ApriltagDetector(int streamId, bool showWindow, ConfigReader &config, Localizer &localizer, NetworkingClient &client);
        void startStream();
        void detect();
        cv::VideoCapture cap;

    private:
        int streamId;
        bool showWindow;
        ConfigReader &config;
        Localizer &localizer;
        NetworkingClient &client;
};

#endif