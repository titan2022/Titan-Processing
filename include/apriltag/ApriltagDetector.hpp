#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include <opencv2/opencv.hpp>

#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "util/Camera.hpp"
#include "util/Config.hpp"

namespace titan
{
class ApriltagDetector
{
  public:
	ApriltagDetector(cv::VideoCapture stream, bool showWindow, Config &config, Camera &cam, Localizer &localizer);
	void detect();

  private:
	cv::VideoCapture stream;
	Camera cam;
	bool showWindow;
	Config &config;
	Localizer &localizer;
};
} // namespace titan

#endif
