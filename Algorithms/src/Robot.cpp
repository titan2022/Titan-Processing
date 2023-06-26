#include "../include/Robot.h"
#include <iostream>

Robot::Robot(Pixel pixel, const cv::Mat& positionMatrix) :
	Object(calculatePositionFromPixel(pixel, positionMatrix))
{

}

Vector Robot::calculatePositionFromPixel(Pixel pixel, const cv::Mat& positionMatrix)
{
	Vector surfacePosition = pixel.getPixelPosition(positionMatrix);
	try
	{
		Vector unitVector = surfacePosition.getUnitVector();
		double magnitude = (surfacePosition.getMagnitude() + 0.5);
		return unitVector * magnitude;
	}
	catch (std::domain_error& e)
	{
		std::cout << "ERROR: " << e.what() << "\n";
		return Vector(0, 0, 0);
	}
}