#ifndef COORDINATE_TRANSFORMATIONS_H
#define COORDINATE_TRANSFORMATIONS_H

#include "Object.h"
#include "Vector.h"

class CoordinateTransformations
{
private:
	const static double xAxisAngle;
	const static double yAxisAngle;
	const static double zAxisAngle;
	const static Vector translation;
public:
	static void objectTransform(Object& object);
};

#endif
