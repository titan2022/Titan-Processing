#include "../include/BlueRobot.h"

std::vector<double> BlueRobot::properHSL = { 110, 140, 20, 60, 30, 90 };
BlueRobot::BlueRobot(Pixel pixel, const cv::Mat& positionMatrix) :
	Robot(pixel, positionMatrix)
{

}

std::vector<double> BlueRobot::getProperHSL()
{
	return properHSL;
}


std::string BlueRobot::toString()
{
	return "Blue Robot: " + position.toString();
}