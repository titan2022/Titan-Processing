#ifndef VECTOR_H
#define VECTOR_H

#include <opencv2/core.hpp>

/// <summary>
/// Basic vector class with x, y, and z components
/// </summary>
struct Vector
{
private:
	
public:
	double x, y, z;
	Vector();
	Vector(double inputX, double inputY, double inputZ);
	/// <summary>
	/// Constructors to convert opencv vectors to our vectors
	/// </summary>
	/// <returns></returns>
	Vector(cv::Vec<float, 3> vec);
	Vector(cv::Vec<double, 3> vec);
	Vector(cv::Vec<int, 3> vec);
	double getMagnitude();
	double getMagnitudeSquared();
	Vector getUnitVector();
	void rotateXAxis(double angle);
	void rotateYAxis(double angle);
	void rotateZAxis(double angle);
	/// <summary>
	/// Rotates the vector according to the world axes and 
	/// </summary>
	/// <param name="xAxisRotation">Rotations in degrees using RHR</param>
	/// <param name="yAxisRotation">Rotations in degrees using RHR</param>
	/// <param name="zAxisRotation">Rotations in degrees using RHR</param>
	/// <param name="order"></param>
	void rotateVector(double xAxisRotation, double yAxisRotation, double zAxisRotation, const char order[3]);
	std::string toString();

	/// <summary>
	/// Rotates a vector across the world axis
	/// </summary>
	/// <param name="xAxisRotation">Angle of rotation in the y-z plane in radians</param>
	/// <param name="yAxisRotation">Angle of rotation in the x-z plane in radians</param>
	/// <param name="zAxisRotation">Angle of rotation in the x-y plane in radians</param>
	void rotateVector(double xAxisRotation, double yAxisRotation, double zAxisRotation, const char order[3]) const;

};

Vector operator+ (Vector vec1, Vector vec2);
Vector operator- (Vector vec1, Vector vec2);
Vector operator- (Vector vec);
Vector operator* (double scalar, Vector vec);
Vector operator* (Vector vec, double scalar);
Vector operator/ (Vector vec, double divisor);
bool operator== (Vector vec1, Vector vec2);
bool operator!= (Vector vec1, Vector vec2);
void operator+= (Vector vec1, Vector vec2);
void operator-= (Vector vec1, Vector vec2);
void operator*= (Vector vec, double scalar);
void operator/= (Vector vec, double divisor);

#endif