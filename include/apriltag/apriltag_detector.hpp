#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <string>

#include "apriltag.hpp"
#include "../helper/vector.hpp"

class ApriltagDetector
{
    public:
        ApriltagDetector(int streamId, bool showWindow);
        void detect(void (*handle)(const Apriltag &));
        void setFocalLength(double x, double y);

    private:
        int streamId;
        bool showWindow;
        double focalX = 50;
        double focalY = 50;
};

#endif