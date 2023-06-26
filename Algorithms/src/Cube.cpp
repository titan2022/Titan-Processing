#include "../include/Cube.h"
#include <iostream>

//Actual field hsl values
//const std::vector<double> Cube::properHSL = { 125, 135, 30, 160, 55, 220 };
//Practice field hsl values
const std::vector<double> Cube::properHSL = { 120, 155, 30, 90, 40, 110 };

Cube::Cube()
{

}

Cube::Cube(Vector inputPosition) :
	Object(inputPosition)
{

}

Cube::Cube(Pixel pixel, const cv::Mat& positionMatrix) :
	Cube(calculatePositionFromPixel(pixel, positionMatrix))
{

}

Vector Cube::calculatePositionFromPixel(Pixel pixel, const cv::Mat& positionMatrix)
{
	Vector surfacePosition = pixel.getPixelPosition(positionMatrix);
	try
	{
		Vector unitVector = surfacePosition.getUnitVector();
		double magnitude = surfacePosition.getMagnitude();
		return unitVector * magnitude;
	}
	catch (std::domain_error& e)
	{
		std::cout << "ERROR: " << e.what() << "\n";
		return Vector(0, 0, 0);
	}
}


std::vector<double> Cube::getProperHSL()
{
	return properHSL;
}

std::string Cube::toString()
{
	return "Cube: " + position.toString();
}
