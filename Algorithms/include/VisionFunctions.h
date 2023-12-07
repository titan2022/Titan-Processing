#ifndef VISION_FUNCTIONS_H
#define VISION_FUNCITONS_H

#include <opencv2/core/core.hpp>

namespace VisionFunctions
{
	void hslThreshold(cv::Mat& input, std::vector<double> hslThreshold, cv::Mat& out);

	void andMask(cv::Mat& input, cv::Mat& mask, cv::Mat& output);

	void mergeMask(cv::Mat& input1, cv::Mat& input2, cv::Mat& output);

	std::vector<std::vector<cv::Point>> findContours(cv::Mat& input);

	cv::Point getMomentBasedContourCenter(const std::vector<cv::Point>& contour);
}

#endif