#ifndef ROBOT_H
#define ROBOT_H

#include "Object.h"
class Robot : public Object
{
private:
	static Vector calculatePositionFromPixel(Pixel pixel, const cv::Mat& positionMatrix);
public:
	Robot(Pixel pixel, const cv::Mat& positionMatrix);
};

#endif

