#ifndef COORDINATE_TRANSFORMATIONS_H
#define COORDINATE_TRANSFORMATIONS_H

#include "Object.h"
#include "Vector.h"

namespace CoordinateTransformations
{
	const double xAxisAngle = -(10 / 180) * M_PI;
	const double yAxisAngle = (22 / 180) * M_PI;
	const double zAxisAngle = -M_PI / 2;
	const Vector translation = Vector(-0.3683, 0.762, 0.0508);

	void objectTransform(Object& object);
	static cv::Mat generateXRotationMatrix(double angle);
	static cv::Mat generateYRotationMatrix(double angle);
	static cv::Mat generateZRotationMatrix(double angle);
	static cv::Mat generateTranslationMatrix(double x, double y, double z);


};

#endif
