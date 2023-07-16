
#include "RealsenseFrame.h"

RealsenseFrame::RealsenseFrame(const cv::Mat& inputDepth, const cv::Mat& inputColor, const cv::Mat& inputPosition) : depthMatrix(inputDepth), colorMatrix(inputColor), positionMatrix(inputPosition) {}
