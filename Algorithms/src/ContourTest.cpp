#include "../include/ContourTest.h"

BlurType ContourTest::blurType = BlurType::GAUSSIAN;
cv::Point ContourTest::cvErodeAnchor(-1, -1);
double ContourTest::cvErodeIterations = 2;
int ContourTest::cvErodeBordertype = cv::BORDER_CONSTANT;
cv::Scalar ContourTest::cvErodeBordervalue(-1);
Mat ContourTest::cvErodeKernel;

ContourTest::ContourTest()
{

}

void ContourTest::calculateMoments()
{
	//Should be fixed with parameters for easy adjustment later
	contourMoments.resize(greatestContoursOutput.size());
	for (size_t i = 0; i < contourMoments.size(); i++)
	{
		contourMoments.at(i) = cv::moments(greatestContoursOutput[i]);
	}
}

void ContourTest::calculateContourCenters()
{
	contourCenters.resize(contourMoments.size());
	for (size_t i = 0; i < contourMoments.size(); i++)
	{
		//add 1e-5 to avoid division by zero
		contourCenters[i] = cv::Point2i(static_cast<int>(contourMoments[i].m10 / (contourMoments[i].m00 + 1e-5)),
			static_cast<int>(contourMoments[i].m01 / (contourMoments[i].m00 + 1e-5)));
	}
	
}

std::vector<cv::Moments>* ContourTest::getMoments()
{
	return &contourMoments;
}

std::vector<cv::Point2i>* ContourTest::getContourCenters()
{
	return &contourCenters;
}

std::vector<Point2i> ContourTest::getBoundingPixels(const std::vector<Point2i>& contourPoints)
{
	Point2i min, max;
	for (auto point : contourPoints)
	{
		if (point.x > max.x)
		{
			max = point;
		}
		else if (point.x < min.x)
		{
			min = point;
		}
	}

	return std::vector<Point2i> { min, max };
}


/**
 * This method is a generated getter for the output of a Blur.
 * @return Mat output from Blur.
 */
cv::Mat* ContourTest::GetBlurOutput() {
	return &(this->blurOutput);
}
/**
 * This method is a generated getter for the output of a HSL_Threshold.
 * @return Mat output from HSL_Threshold.
 */
cv::Mat* ContourTest::GetHslThresholdOutput() {
	return &(this->hslThresholdOutput);
}
/**
 * This method is a generated getter for the output of a Mask.
 * @return Mat output from Mask.
 */
cv::Mat* ContourTest::GetMaskOutput() {
	return &(this->maskOutput);
}
/**
 * This method is a generated getter for the output of a CV_erode.
 * @return Mat output from CV_erode.
 */
cv::Mat* ContourTest::GetCvErodeOutput() {
	return &(this->cvErodeOutput);
}
/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point>>* ContourTest::GetFindContoursOutput() {
	return &(this->findContoursOutput);
}
/**
 * This method is a generated getter for the output of a Filter_Contours.
 * @return ContoursReport output from Filter_Contours.
 */
std::vector<std::vector<cv::Point> >* ContourTest::GetFilterContoursOutput() {
	return &(this->filterContoursOutput);
}
/**
 * This method is a generated getter for the output of a Convex_Hulls.
 * @return ContoursReport output from Convex_Hulls.
 */
std::vector<std::vector<cv::Point> >* ContourTest::GetConvexHullsOutput() {
	return &(this->convexHullsOutput);
}
/**
 * Softens an image using one of several filters.
 *
 * @param input The image on which to perform the blur.
 * @param type The blurType to perform.
 * @param doubleRadius The radius for the blur.
 * @param output The image in which to store the output.
 */
void ContourTest::blur(cv::Mat& input, BlurType& type, double doubleRadius, cv::Mat& output) {
	int radius = (int)(doubleRadius + 0.5);
	int kernelSize;
	switch (type) {
	case BOX:
		kernelSize = 2 * radius + 1;
		cv::blur(input, output, cv::Size(kernelSize, kernelSize));
		break;
	case GAUSSIAN:
		kernelSize = 6 * radius + 1;
		cv::GaussianBlur(input, output, cv::Size(kernelSize, kernelSize), radius);
		break;
	case MEDIAN:
		kernelSize = 2 * radius + 1;
		cv::medianBlur(input, output, kernelSize);
		break;
	case BILATERAL:
		cv::bilateralFilter(input, output, -1, radius, radius);
		break;
	}
}
/**
 * Segment an image based on hue, saturation, and luminance ranges.
 *
 * @param input The image on which to perform the HSL threshold.
 * @param hue The min and max hue.
 * @param sat The min and max saturation.
 * @param lum The min and max luminance.
 * @param output The image in which to store the output.
 */
 //void hslThreshold(Mat *input, double hue[], double sat[], double lum[], Mat *out) {
void ContourTest::hslThreshold(cv::Mat& input, std::vector<double> hslThreshold, cv::Mat& out) {
	cv::cvtColor(input, out, cv::COLOR_BGR2HLS);
	if (hslThreshold[0] < -180 || hslThreshold[1] > 180)
	{
		throw std::invalid_argument("HSL threshold values out of range");
	}
	else if (hslThreshold[0] < 0)
	{
		std::vector<double> lowerHSL(6);
		std::vector<double> upperHSL(6);
		for (int i = 2; i < hslThreshold.size(); ++i)
		{
			lowerHSL[i] = hslThreshold[i];
		}
		lowerHSL[0] = 0;
		lowerHSL[1] = hslThreshold[1];
		upperHSL[0] = 180 + hslThreshold[0];
		upperHSL[1] = 180;
		cv::Mat lowerMask;
		cv::Mat upperMask;
		cv::inRange(out, cv::Scalar(lowerHSL[0], lowerHSL[2], lowerHSL[4]), cv::Scalar(lowerHSL[1], lowerHSL[3], lowerHSL[5]), lowerMask);
		cv::inRange(out, cv::Scalar(upperHSL[0], upperHSL[2], upperHSL[4]), cv::Scalar(upperHSL[1], upperHSL[3], upperHSL[5]), upperMask);
		// Combines masks because if a matrix values are:
		// 1 OR 1 = 1
		// 1 OR 0 = 1
		// 0 OR 1 = 1
		// 0 OR 0 = 0
		out = lowerMask | upperMask;
	}
	else
	{
		cv::inRange(out, cv::Scalar(hslThreshold[0], hslThreshold[2], hslThreshold[4]), cv::Scalar(hslThreshold[1], hslThreshold[3], hslThreshold[5]), out);
	}
}

/**
 * Filter out an area of an image using a binary mask.
 *
 * @param input The image on which the mask filters.
 * @param mask The binary image that is used to filter.
 * @param output The image in which to store the output.
 */
void ContourTest::mask(cv::Mat& input, cv::Mat& mask, cv::Mat& output) {
	mask.convertTo(mask, CV_8UC1);
	cv::bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

/**
 * Expands area of lower value in an image.
 * @param src the Image to erode.
 * @param kernel the kernel for erosion.
 * @param anchor the center of the kernel.
 * @param iterations the number of times to perform the erosion.
 * @param borderType pixel extrapolation method.
 * @param borderValue value to be used for a constant border.
 * @param dst Output Image.
 */
void ContourTest::cvErode(cv::Mat& src, cv::Mat& kernel, cv::Point& anchor, double iterations, int borderType, cv::Scalar& borderValue, cv::Mat& dst) {
	cv::erode(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
}

/**
 * Finds contours in an image.
 *
 * @param input The image to find contours in.
 * @param externalOnly if only external contours are to be found.
 * @param contours vector of contours to put contours in.
 */
void ContourTest::findContours(cv::Mat& input, bool externalOnly, std::vector<std::vector<cv::Point> >& contours) {
	std::vector<cv::Vec4i> hierarchy;
	contours.clear();
	int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
	int method = cv::CHAIN_APPROX_SIMPLE;
	cv::findContours(input, contours, hierarchy, mode, method);
}


/**
 * Filters through contours.
 * @param inputContours is the input vector of contours.
 * @param minArea is the minimum area of a contour that will be kept.
 * @param minPerimeter is the minimum perimeter of a contour that will be kept.
 * @param minWidth minimum width of a contour.
 * @param maxWidth maximum width.
 * @param minHeight minimum height.
 * @param maxHeight  maximimum height.
 * @param solidity the minimum and maximum solidity of a contour.
 * @param minVertexCount minimum vertex Count of the contours.
 * @param maxVertexCount maximum vertex Count.
 * @param minRatio minimum ratio of width to height.
 * @param maxRatio maximum ratio of width to height.
 * @param output vector of filtered contours.
 */
void ContourTest::filterContours(std::vector<std::vector<cv::Point>>& inputContours, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio, std::vector<std::vector<cv::Point> >& output) {
	std::vector<cv::Point> hull;
	output.clear();
	for (std::vector<cv::Point> contour : inputContours) {
		cv::Rect bb = boundingRect(contour);
		if (bb.width < minWidth || bb.width > maxWidth) continue;
		if (bb.height < minHeight || bb.height > maxHeight) continue;
		double area = cv::contourArea(contour);
		if (area < minArea) continue;
		if (arcLength(contour, true) < minPerimeter) continue;
		cv::convexHull(cv::Mat(contour, true), hull);
		double solid = 100 * area / cv::contourArea(hull);
		if (solid < solidity[0] || solid > solidity[1]) continue;
		if (contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
		double ratio = (double)bb.width / (double)bb.height;
		if (ratio < minRatio || ratio > maxRatio) continue;
		output.push_back(contour);
	}
}

bool ContourTest::greatestContours(std::vector<std::vector<cv::Point>>& inputContours, std::vector<std::vector<cv::Point>>& outputContours)
{
	outputContours.clear();
	std::vector<cv::Point> currentContour;
	for (auto contour : inputContours)
	{
		cv::Rect previousContourRectangle = boundingRect(currentContour);
		cv::Rect boundingRectangle = boundingRect(contour);
		if (boundingRectangle.area() > previousContourRectangle.area())
		{
			currentContour = contour;
		}
	}
	if (currentContour.size() > 0)
	{
		outputContours.push_back(currentContour);
		return 1;
	}
	else
	{
		return -1;
	}
}

std::vector<std::vector<cv::Point>>* ContourTest::GetGreatestContoursOutput()
{
	return &greatestContoursOutput;
}

/**
 * Compute the convex hulls of contours.
 *
 * @param inputContours The contours on which to perform the operation.
 * @param outputContours The contours where the output will be stored.
 */
void ContourTest::convexHulls(std::vector<std::vector<cv::Point> >& inputContours, std::vector<std::vector<cv::Point> >& outputContours) {
	std::vector<std::vector<cv::Point> > hull(inputContours.size());
	outputContours.clear();
	for (size_t i = 0; i < inputContours.size(); i++) {
		cv::convexHull(cv::Mat((inputContours)[i]), hull[i], false);
	}
	outputContours = hull;
}


