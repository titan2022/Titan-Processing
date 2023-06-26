
#include "RealsenseFrame.h"

RealsenseFrame::RealsenseFrame(const Mat& inputDepth, const Mat& inputColor, const Mat& inputPosition) : depthMatrix(inputDepth), colorMatrix(inputColor), positionMatrix(inputPosition) {}
