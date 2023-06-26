#include "../include/Pixel.h"
const int Pixel::kernelWidth = 1;
const int Pixel::kernalHeight = 1;

Pixel::Pixel(double inputU, double inputV) :
	u(inputU), v(inputV)
{

}

Pixel::Pixel(double inputU, double inputV, double inputWidth, double inputLength) :
	width(inputWidth), length(inputLength), u(inputU), v(inputV)
{

}

void Pixel::scale(double scaleFactor)
{
	width *= scaleFactor;
	length *= scaleFactor;
}

Vector Pixel::getPixelPosition(const Mat& positionMatrix)
{
	auto vec = positionMatrix.at<cv::Vec3f>(v, u);
	return Vector(vec);
}