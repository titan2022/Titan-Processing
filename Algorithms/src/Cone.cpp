#define _USE_MATH_DEFINES

#include "../include/Cone.h"
#include <iostream>
#include <cmath>
//Actual field values
//const std::vector<double> Cone::properHSL = { 22, 35, 45, 210, 80, 255};
//Practice field values
//const std::vector<double> Cone::properHSL = {15, 35, 45, 210, 80, 255};
//temp ignore please
const std::vector<double> Cone::properHSL = {15, 35, 45, 210, 80, 255};

Cone::Cone()
{

}

Cone::Cone(Vector position) :
	Object(position)
{

}

Cone::Cone(Pixel pixel, const cv::Mat& positionMatrix) :
	Cone(calculatePositionFromPixel(pixel , positionMatrix))
{

}

Vector Cone::calculatePositionFromPixel(Pixel pixel, const cv::Mat& positionMatrix) 
{
	Vector surfacePosition = pixel.getPixelPosition(positionMatrix);
	try
	{
		Vector unitVector = surfacePosition.getUnitVector();
		double magnitude = surfacePosition.getMagnitude();
		return unitVector * magnitude;
		/*
		//Trig formula to figure out how much to scale the vector into the cone if the cone is below the camera
		//Angle of elevation of the cone's slope
		double coneHeight = 0.33;
		//Length of half the cone's base
		double coneBaseLength = 0.09;
		double coneAngle = atan(coneHeight / coneBaseLength);
		//Derived from the angle of the camera relative to cone and the cone's constants
		double theta = M_PI - coneAngle - abs(atan(surfacePosition.z / surfacePosition.y));
		//Use law of sines to get how much to project to get the center of the cone base
		double magnitudeIncrease = sin(coneAngle) * coneBaseLength / sin(theta);
		Vector yzVector = surfacePosition;
		yzVector.x = 0;
		Vector increase = yzVector.getUnitVector() * magnitudeIncrease;
		Vector result = yzVector + increase; 
		result.x = surfacePosition.x;
		return result;
		*/
	}
	catch (std::domain_error& e)
	{
		std::cout << "ERROR: " << e.what() << "\n";
		return Vector(0, 0, 0);
	}
}

std::vector<double> Cone::getProperHSL()
{
	return properHSL;
}

std::string Cone::toString()
{
	return "Cone: " + position.toString();
}
