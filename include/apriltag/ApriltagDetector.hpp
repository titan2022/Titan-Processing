#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include "opencv2/opencv.hpp"

#include "../apriltag/Localizer.hpp"
#include "../networking/Client.h"
#include "../util/ConfigReader.hpp"
#include "util/VideoStream.hpp"

namespace titan
{
class ApriltagDetector
{
  public:
	ApriltagDetector(VideoStream &stream, bool showWindow, ConfigReader &config, Localizer &localizer,
					 NetworkingClient &client);
	void startStream();
	void detect();
	cv::VideoCapture cap;

  private:
	VideoStream stream;
	bool showWindow;
	ConfigReader &config;
	Localizer &localizer;
	NetworkingClient &client;
};
} // namespace titan

#endif