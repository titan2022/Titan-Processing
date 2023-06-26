#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"
#include "Pixel.h"
#include <vector>

class Object
{
protected:
	//Position relative to robot
	Vector position;

public:
	Vector& getPosition();

	/// <summary>
	/// Implemented by the subclasses to return the color values
	/// </summary>
	/// <returns></returns>
	virtual std::vector<double> getProperHSL() = 0;

	double getDistance();

	Object();

	/// <summary>
	/// The object constructor only takes in the coordinates of the surface pixel
	/// </summary>
	/// <param name="inputX"></param>
	/// <param name="inputY"></param>
	/// <param name="inputZ"></param>
	/// <param name="inputDistance"></param>
	Object(Vector inputPosition);

	virtual std::string toString() = 0;
};

#endif