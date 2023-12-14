#include "VisionFunctions.h"

#include <opencv2/imgproc/imgproc.hpp>

void VisionFunctions::hslThreshold(cv::Mat& input, std::vector<double> hslThreshold, cv::Mat& out) 
{
	cv::cvtColor(input, out, cv::COLOR_BGR2HLS);
	if (hslThreshold[0] < -180 || hslThreshold[1] > 180)
	{
		throw std::invalid_argument("HSL threshold values out of range");
	}
	else if (hslThreshold[0] < 0)
	{
		std::vector<double> lowerHSL(6);
		std::vector<double> upperHSL(6);
		for (int i = 2; i < hslThreshold.size(); ++i)
		{
			lowerHSL[i] = hslThreshold[i];
		}
		lowerHSL[0] = 0;
		lowerHSL[1] = hslThreshold[1];
		upperHSL[0] = 180 + hslThreshold[0];
		upperHSL[1] = 180;
		cv::Mat lowerMask;
		cv::Mat upperMask;
		cv::inRange(out, cv::Scalar(lowerHSL[0], lowerHSL[2], lowerHSL[4]), cv::Scalar(lowerHSL[1], lowerHSL[3], lowerHSL[5]), lowerMask);
		cv::inRange(out, cv::Scalar(upperHSL[0], upperHSL[2], upperHSL[4]), cv::Scalar(upperHSL[1], upperHSL[3], upperHSL[5]), upperMask);
		// Combines masks because if a matrix values are:
		// 1 OR 1 = 1
		// 1 OR 0 = 1
		// 0 OR 1 = 1
		// 0 OR 0 = 0
		out = lowerMask | upperMask;
	}
	else
	{
		cv::inRange(out, cv::Scalar(hslThreshold[0], hslThreshold[2], hslThreshold[4]), cv::Scalar(hslThreshold[1], hslThreshold[3], hslThreshold[5]), out);
	}
}

void VisionFunctions::erodeMask(cv::Mat& input, cv::Mat& output)
{
	auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Point matrixCenter(-1, -1);
	int iterations = 5;
	cv::erode(input, output, kernel, matrixCenter, iterations);
}

void VisionFunctions::dilateMask(cv::Mat& input, cv::Mat& output)
{
	auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
	cv::Point matrixCenter(-1, -1);
	int iterations = 3;
	cv::dilate(input, output, kernel, matrixCenter, iterations);
}

void VisionFunctions::mergeMask(cv::Mat& input, cv::Mat& output)
{
	dilateMask(input, input);
	erodeMask(output, output);
}

void VisionFunctions::andMask(cv::Mat& input, cv::Mat& mask, cv::Mat& output) 
{
	mask.convertTo(mask, CV_8UC1);
	cv::bitwise_and(input, mask, output);
	input.copyTo(output, mask);
}

std::vector<std::vector<cv::Point>> VisionFunctions::findContours(cv::Mat& input)
{
	//std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> contourOutput;

	int hierarchyOptions = cv::RETR_EXTERNAL;
	int contourApproximationMethod = cv::CHAIN_APPROX_SIMPLE;

	cv::findContours(input, contourOutput, hierarchyOptions, contourApproximationMethod);

	return contourOutput;
}

cv::Point VisionFunctions::getMomentBasedContourCenter(const std::vector<cv::Point>& contour)
{
	auto contourMoments = cv::moments(contour);
	cv::Point center(contourMoments.m10 / contour.size(), contourMoments.m01 / contour.size());

	return center;
}