#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <string>

#include "Apriltag.hpp"
#include "opencv2/opencv.hpp"
#include "../helper/ConfigReader.hpp"
#include "../helper/Vector3D.hpp"
#include "../networking/Client.hpp"

class ApriltagDetector
{
    public:
        ApriltagDetector(int streamId, bool showWindow, ConfigReader config, NetworkingClient client);
        void startStream();
        void detect(void (*handle)(const Apriltag &, NetworkingClient &));
        cv::VideoCapture cap;

    private:
        int streamId;
        bool showWindow;
        ConfigReader config;
        NetworkingClient &client;
};

#endif