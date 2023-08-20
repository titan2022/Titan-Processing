#ifndef VISION_FUNCTIONS_H
#define VISION_FUNCTIONS_H

#include <opencv2/core/mat.hpp>

class VisionFunctions
{
	static void findContours(const cv::Mat& input, cv::Mat& output);
	static void mergeContours(const cv::Mat& input, cv::Mat& output);
	static std::vector<cv::Point> findCenters(cv::Mat& input);
};

#endif