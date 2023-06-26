#ifndef BLUE_ROBOT_H
#define	BLUE_ROBOT_H

#include "Robot.h"

class BlueRobot : public Robot
{
private:

public:
	BlueRobot(Pixel pixel, const cv::Mat& positionMatrix);
	static std::vector<double> properHSL;
	std::vector<double> getProperHSL() override;
	std::string toString() override;
};

#endif