#include "../include/Vector.h"
#include <cmath>
#include <string>

using std::to_string;
using std::cos;
using std::sin;

Vector::Vector()
{

}

Vector::Vector(cv::Vec<float, 3> vec) :
	x(vec[0]), y(vec[1]), z(vec[2])
{
}

Vector::Vector(cv::Vec<double, 3> vec) :
	x(vec[0]), y(vec[1]), z(vec[2])
{

}

Vector::Vector(cv::Vec<int, 3> vec) :
	x(vec[0]), y(vec[1]), z(vec[2])
{

}

Vector::Vector(double inputX = 0, double inputY = 0, double inputZ = 0) :
	x(inputX), y(inputY), z(inputZ)
{

}

double Vector::getMagnitude()
{
	return std::sqrt(getMagnitudeSquared());
}

double Vector::getMagnitudeSquared()
{
	return x * x + y * y + z * z;
}

Vector Vector::getUnitVector()
{
	double magnitude = getMagnitude();
	if (magnitude != 0)
	{
		return (*this) / getMagnitude();
	}
	else
	{
		throw std::domain_error("ERROR: Vector has not magnitude");
	}
}


void Vector::rotateXAxis(double angle)
{
	double resultY = y * cos(angle) - z * sin(angle);
	double resultZ = y * sin(angle) + z * cos(angle);
	z = resultZ;
	y = resultY;
}
void Vector::rotateYAxis(double angle)
{
	double resultX = x * cos(angle) + z * sin(angle);
	double resultZ = x * -sin(angle) + z * cos(angle);
	x = resultX;
	z = resultZ;
}
void Vector::rotateZAxis(double angle)
{
	double resultX = x * cos(angle) - y * sin(angle);
	double resultY = x * sin(angle) + y * cos(angle);
	x = resultX;
	y = resultY;
}

void Vector::rotateVector(double xAxisRotation, double yAxisRotation, double zAxisRotation, const char order[3] = "XYZ")
{
	switch (order[0])
	{
	case 'X':
		rotateXAxis(xAxisRotation);
		break;
	case 'Y':
		rotateYAxis(yAxisRotation);
		break;
	case 'Z':
		rotateZAxis(zAxisRotation);
		break;
	}

	switch (order[1])
	{
	case 'X':
		rotateXAxis(xAxisRotation);
		break;
	case 'Y':
		rotateYAxis(yAxisRotation);
		break;
	case 'Z':
		rotateZAxis(zAxisRotation);
		break;
	}

	switch (order[2])
	{
	case 'X':
		rotateXAxis(xAxisRotation);
		break;
	case 'Y':
		rotateYAxis(yAxisRotation);
		break;
	case 'Z':
		rotateZAxis(zAxisRotation);
		break;
	}

}


std::string Vector::toString()
{
	return "X{" + to_string(x) + "} Y{" + to_string(y) + "} Z{" + to_string(z) + "}";
}

Vector operator+(Vector vec1, Vector vec2)
{
	return Vector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

Vector operator-(Vector vec)
{
	return Vector(-vec.x, -vec.y, -vec.z);
}

Vector operator-(Vector vec1, Vector vec2)
{
	return vec1 + (-vec2);
}

Vector operator*(double scalar, Vector vec)
{
	return Vector(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vector operator*(Vector vec, double scalar)
{
	return scalar * vec;
}

Vector operator/ (Vector vec, double divisor)
{
	return (1 / divisor) * vec;
}

bool operator== (Vector vec1, Vector vec2)
{
	if (vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool operator!=(Vector vec1, Vector vec2)
{
	return !(vec1 == vec2);
}

void operator+=(Vector vec1, Vector vec2)
{
	vec1 = vec1 + vec2;
}

void operator-= (Vector vec1, Vector vec2)
{
	vec1 = vec1 - vec2;
}

void operator*= (Vector vec, double scalar)
{
	vec = scalar * vec;
}

void operator/= (Vector vec, double divisor)
{
	vec = vec / divisor;
}
