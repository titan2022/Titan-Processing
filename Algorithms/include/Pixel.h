#ifndef PIXEL_H
#define PIXEL_H

#include "Vector.h"
#include <opencv2/core.hpp>

using cv::Mat;

struct Pixel
{
private:
	//Size of kernel to search for positions from
	static const int kernelWidth;
	static const int kernalHeight;
	/// <summary>
	/// In units of pixels
	/// </summary>
	double u, v;
	/// <summary>
	/// Width and length of the map the u and v are part of
	/// </summary>
	double width, length;
public:
	/// <summary>
	/// Will only accepted values from 0-1
	/// </summary>
	/// <param name="inputU"></param>
	/// <param name="inputV"></param>
	Pixel(double inputU, double inputV);
	/// <summary>
	/// Will take int u and v values as coordinates
	/// </summary>
	/// <param name="inputU"></param>
	/// <param name="inputV"></param>
	/// <param name="width"></param>
	/// <param name="length"></param>
	Pixel(double inputU, double inputV, double inputWidth, double inputLength);
	/// <summary>
	/// Get u as pixel coordinate
	/// </summary>
	/// <returns></returns>
	inline int getU() { return u; }
	/// <summary>
	/// Get v as pixel coordinate
	/// </summary>
	/// <returns></returns>
	inline int getV() { return v; }

	/// <summary>
	/// u as a ratio out of 1
	/// </summary>
	inline double getProperU() { return u / width; }
	/// <summary>
	/// v as ratio out of 1
	/// </summary>
	/// <param name="height"></param>
	/// <returns></returns>
	inline double getProperV() { return v / length; }

	inline double getWidth() { return width; }
	inline double getLength() { return length; }

	Vector getPixelPosition(const Mat& positionMatrix);

	void scale(double scaleFactor);


};

#endif