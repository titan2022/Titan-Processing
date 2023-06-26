#ifndef CUBE_H
#define CUBE_H

#include "Object.h"
#include <vector>

class Cube : public Object
{
private:
	static Vector calculatePositionFromPixel(Pixel pixel, const cv::Mat& positionMatrix);
public:
	static const std::vector<double> properHSL;
	Cube();
	Cube(Vector inputPosition);
	Cube(Pixel pixel, const cv::Mat& positionMatrix);
	std::vector<double> getProperHSL() override;
	std::string toString() override;
};

#endif