#include "RealsenseFrame.h"

#include "RealsenseCameraModule.h"

RealsenseFrame::RealsenseFrame(const cv::Mat& inputDepth, const cv::Mat& inputColor, const cv::Mat& inputPosition) : depthMatrix(inputDepth), colorMatrix(inputColor), positionMatrix(inputPosition) {}

size_t RealsenseFrame::getXResolution() const
{
	return RealsenseCameraModule::xResolution;
}

size_t RealsenseFrame::getYResolution() const
{
	return RealsenseCameraModule::yResolution;
}