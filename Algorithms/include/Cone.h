#ifndef CONE_H
#define CONE_H

#include "Object.h"

class Cone : public Object
{
private:
	static Vector calculatePositionFromPixel(Pixel pixel, const cv::Mat& positionMatrix);
public:
	static const std::vector<double> properHSL;
	Cone();
	Cone(Vector position);
	Cone(Pixel pixel, const cv::Mat& positionMatrix);
	std::vector<double> getProperHSL() override;
	std::string toString() override;
};

#endif