#ifndef VISION_FUNCTIONS_H
#define VISION_FUNCITONS_H

#include <opencv2/core/core.hpp>

using std::vector;
using cv::Mat;

namespace VisionFunctions
{
	void hslThreshold(cv::Mat& input, std::vector<double> hslThreshold, cv::Mat& out);
	void rgbThreshold(const Mat& input, vector<double> rgbThreshold, Mat& out);

	void andMask(cv::Mat& input, cv::Mat& mask, cv::Mat& output);
	void mergeMask(cv::Mat& input, cv::Mat& output);
	void erodeMask(cv::Mat& input, cv::Mat& output, int iterations = 5);
	void dilateMask(cv::Mat& input, cv::Mat& output, int iterations = 3);
	void blackFilterMask(cv::Mat& input, cv::Mat& output);

	std::vector<std::vector<cv::Point>> findContours(cv::Mat& input);

	cv::Point getMomentBasedContourCenter(const std::vector<cv::Point>& contour);
}

#endif