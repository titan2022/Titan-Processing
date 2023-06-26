#ifndef RED_ROBOT_H
#define	RED_ROBOT_H

#include "Robot.h"

class RedRobot : public Robot
{
private:

public:
	RedRobot(Pixel pixel, const cv::Mat& positionMatrix);
	static std::vector<double> properHSL;
	std::vector<double> getProperHSL() override;
	std::string toString() override;
};

#endif