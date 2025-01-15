#ifndef APRILTAG_DETECTOR
#define APRILTAG_DETECTOR

#include "../apriltag/Localizer.hpp"
#include "../util/Config.hpp"
#include "util/Camera.hpp"
#include <opencv2/opencv.hpp>

namespace titan
{
class ApriltagDetector
{
  public:
	ApriltagDetector(cv::VideoCapture stream, bool showWindow, const Config &config, Camera &cam, Localizer &localizer);
	void detect();

  private:
	cv::VideoCapture stream;
	Camera cam;
	bool showWindow;
	int quadDecimate;
	int quadSigma;

	Localizer &localizer;
};
} // namespace titan

#endif