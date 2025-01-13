#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include "opencv2/opencv.hpp"

#include "../apriltag/Localizer.hpp"
#include "../networking/Client.h"
#include "../util/ConfigReader.hpp"
#include "util/VideoStream.hpp"
#include <memory>

namespace titan
{
class ApriltagDetector
{
  public:
	ApriltagDetector(std::shared_ptr<VideoStream> stream, bool showWindow, ConfigReader &config, Localizer &localizer);
	void startStream();
	void detect();
	cv::VideoCapture cap;

  private:
	std::shared_ptr<VideoStream> stream;
	bool showWindow;
	ConfigReader &config;
	Localizer &localizer;
};
} // namespace titan

#endif