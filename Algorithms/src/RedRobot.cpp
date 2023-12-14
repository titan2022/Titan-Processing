#include "../include/RedRobot.h"

std::vector<double> RedRobot::properHSL = {-1, 15, 40, 90, 150, 250 };

RedRobot::RedRobot(Pixel pixel, const cv::Mat& positionMatrix) :
	Robot(pixel, positionMatrix)
{

}

std::vector<double> RedRobot::getProperHSL()
{
	return properHSL;
}


std::string RedRobot::toString()
{
	return "Red Robot: " + position.toString();
}