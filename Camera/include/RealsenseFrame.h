#ifndef REALSENSE_FRAME_H
#define REALSENSE_FRAME_H

#include <opencv2/core/mat.hpp>
#include "Frame.h"

class RealsenseFrame : public Frame
{
private:
	cv::Mat depthMatrix;
	cv::Mat colorMatrix;
	cv::Mat positionMatrix;

public:
	RealsenseFrame(const cv::Mat& inputDepth, const cv::Mat& inputColor, const cv::Mat& inputPosition);

	inline const cv::Mat getDepthMatrix() const { return depthMatrix; };
	inline const cv::Mat getColorMatrix() const { return colorMatrix;}
	inline const cv::Mat getPositionMatrix() const { return positionMatrix; }

	inline bool hasDepth() const override { return true; }
	inline bool hasColor() const override { return true; }
	inline bool hasPosition() const override { return true; }

	inline size_t getXResolution() const override { return RealsenseCamera::xResolution; }
	inline size_t getYResolution() const override { return RealsenseCamera::yResolution; }
};

#endif
