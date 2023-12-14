#define _USE_MATH_DEFINES

#include "../include/CoordinateTransformations.h"
#include <cmath>


/// Use right hand rule, y is down btw because camera is dumb
/// Z axis -90 degrees
/// X axis -10 degrees 
/// Y axis 22 degrees 
/// 
//const double CoordinateTransformations::xAxisAngle = -(10 / 180) * M_PI;
//const double CoordinateTransformations::yAxisAngle = (22 / 180) * M_PI;
//const double CoordinateTransformations::zAxisAngle = -M_PI / 2;
////const double CoordinateTransformations::xAxisAngle = 0;
////const double CoordinateTransformations::yAxisAngle = 0;
////const double CoordinateTransformations::zAxisAngle = 0;
//const Vector CoordinateTransformations::translation = Vector(-0.3683, 0.762, 0.0508);
////const Vector CoordinateTransformations::translation = Vector(0, 0, 0);

void CoordinateTransformations::objectTransform(Object& object)
{
	object.getPosition().rotateVector(xAxisAngle, yAxisAngle, zAxisAngle, "ZXY");
	object.getPosition().y = -object.getPosition().y;
	object.getPosition() += translation;
}

cv::Mat CoordinateTransformations::generateXRotationMatrix(double angle)
{
	double mat[4][4]
	{
		{1, 0, 0, 0},
		{ 0, cos(angle), -sin(angle), 0},
		{ 0, sin(angle), cos(angle), 0},
		{0, 0, 0, 1}
	};

	return cv::Mat(4, 4, CV_32FC3, mat);
}

cv::Mat CoordinateTransformations::generateYRotationMatrix(double angle)
{
	double mat[4][4]
	{
		{cos(angle), 0, sin(angle), 0},
		{ 0, 1,0, 0},
		{ -sin(angle), 0, cos(angle), 0},
		{0, 0, 0, 1}
	};

	return cv::Mat(4, 4, CV_32FC3, mat);
}

cv::Mat CoordinateTransformations::generateZRotationMatrix(double angle)
{
	double mat[4][4]
	{
		{cos(angle), -sin(angle), 0, 0},
		{ sin(angle), cos(angle), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	return cv::Mat(4, 4, CV_32FC3, mat);
}

cv::Mat CoordinateTransformations::generateTranslationMatrix(double x, double y, double z)
{
	double mat[4][4]
	{
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1}
	};

	return cv::Mat(4, 4, CV_32FC3, mat);
}