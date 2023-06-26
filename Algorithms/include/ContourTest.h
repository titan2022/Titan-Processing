#ifndef CONTOUR_TEST_H
#define CONTOUR_TEST_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

using cv::Point2i;
using cv::Moments;
using cv::Mat;
/**
* A representation of the different types of blurs that can be used.
*
*/
enum BlurType
{
	BOX, GAUSSIAN, MEDIAN, BILATERAL
};
//
//enum ObjectType
//{
//	YELLOW_CONE, PURPLE_CUBE, RED_ROBOT, BLUE_ROBOT
//};
/**
* ContourTest class.
*
* An OpenCV pipeline generated by GRIP.
*/
class ContourTest {
private:
	// Each HSL are the threshold hsl values for the hslThresholdFilter
	// Format is hue min, hue max, sat min, sat max, lum min, lum mas
	/// <summary>
	/// Since the hue wheel goes from 0 - 180 for opencv,
	/// the mask have to be filtered from 170-180 and 0-10
	/// individually before being combined together because opencv
	/// is dumb
	/// </summary>
	static BlurType blurType;
	static cv::Mat cvErodeKernel;
	static cv::Point cvErodeAnchor;
	static double cvErodeIterations;  // default Double
	static int cvErodeBordertype;
	static cv::Scalar cvErodeBordervalue;
	double filterContoursMinArea = 0;  // default Double
	double filterContoursMinPerimeter = 0.0;  // default Double
	double filterContoursMinWidth = 0;  // default Double
	double filterContoursMaxWidth = 100000.0;  // default Double
	double filterContoursMinHeight = 0.0;  // default Double
	double filterContoursMaxHeight = 1000.0;  // default Double
	double filterContoursSolidity[2] = { 0.0, 100.0 };
	double filterContoursMaxVertices = 1000000.0;  // default Double
	double filterContoursMinVertices = 0.0;  // default Double
	double filterContoursMinRatio = 0.0;  // default Double
	double filterContoursMaxRatio = 1000000.0;  // default Double
	double blurRadius = 2.5225226943557324;
	cv::Mat blurOutput;
	cv::Mat hslThresholdOutput;
	cv::Mat maskOutput;
	cv::Mat cvErodeOutput;
	std::vector<std::vector<cv::Point>> findContoursOutput;
	std::vector<std::vector<cv::Point>> filterContoursOutput;
	std::vector<std::vector<cv::Point>> greatestContoursOutput;
	std::vector<std::vector<cv::Point>> convexHullsOutput;
	std::vector<cv::Moments> contourMoments;
	std::vector<cv::Point2i> contourCenters;
	void blur(cv::Mat&, BlurType&, double, cv::Mat&);
	/// <summary>
	/// Thresholds the image based on HSL values with hue from -180 - 180 for a hue wheel going from 0 - 180, saturation from 0 - 255, and lightness from 0 - 255
	/// </summary>
	/// <param name="input"> The matrix to threshold</param>
	/// <param name="hslThreshold"> Vector with lower and upper hue, lower and upper saturation, and lower and upper lightness</param>
	/// <param name="out"> Where to output the matrix</param>
	void hslThreshold(cv::Mat& input, std::vector<double> hslThreshold, cv::Mat& out);
	void mask(cv::Mat&, cv::Mat&, cv::Mat&);
	void cvErode(cv::Mat&, cv::Mat&, cv::Point&, double, int, cv::Scalar&, cv::Mat&);
	void findContours(cv::Mat&, bool, std::vector<std::vector<cv::Point> >&);
	void filterContours(std::vector<std::vector<cv::Point>>&, double, double, double, double, double, double, double[], double, double, double, double, std::vector<std::vector<cv::Point> >&);
	//Returns 1 if a contour exists, -1 if it does not exist
	bool greatestContours(std::vector<std::vector<cv::Point>>& inputContours, std::vector<std::vector<cv::Point>>& outputContours);
	void convexHulls(std::vector<std::vector<cv::Point> >&, std::vector<std::vector<cv::Point> >&);
	void calculateMoments();
	void calculateContourCenters();

public:
	ContourTest();
	template <typename ObjectType>
	inline void Process(cv::Mat& inputMatrix) {
		//Step Blur0:
		//input
		cv::Mat blurInput;
		inputMatrix.copyTo(blurInput);
		blur(blurInput, blurType, blurRadius, this->blurOutput);
		//Step HSL_Threshold0:
		
		hslThreshold(this->blurOutput, ObjectType::properHSL, this->hslThresholdOutput);
		/*switch (object)
		{
			case YELLOW_CONE:
				hslThreshold(hslThresholdInput, yellowConeHSL, this->hslThresholdOutput);
				break;
			case PURPLE_CUBE:
				hslThreshold(hslThresholdInput, purpleCubeHSL, this->hslThresholdOutput);
				break;
			case RED_ROBOT:
				hslThreshold(hslThresholdInput, redRobotHSL, this->hslThresholdOutput);
				break;
			case BLUE_ROBOT:
				hslThreshold(hslThresholdInput, blueRobotHSL, this->hslThresholdOutput);
				break;
			default:
				break;
		}*/

		//Step Mask0:
		//input
		mask(blurInput, hslThresholdOutput, maskOutput);
		//Step CV_erode0:
		//input
		cvErode(hslThresholdOutput, cvErodeKernel, cvErodeAnchor, cvErodeIterations, cvErodeBordertype, cvErodeBordervalue, cvErodeOutput);
		//Step Find_Contours0:
		//input
		bool findContoursExternalOnly = true;  // default Boolean
		findContours(this->cvErodeOutput, findContoursExternalOnly, this->findContoursOutput);
		//Step Filter_Contours0:
		//input
		//std::vector<std::vector<cv::Point>> filterContoursContours = findContoursOutput;
		//filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio, this->filterContoursOutput);
		//Step Convex_Hulls0:
		//input
		//std::vector<std::vector<cv::Point>> convexHullsContours = filterContoursOutput;
		//convexHulls(convexHullsContours, this->convexHullsOutput);
		greatestContours(this->findContoursOutput, greatestContoursOutput);
		calculateMoments();
		calculateContourCenters();
	}
	cv::Mat* GetBlurOutput();
	cv::Mat* GetHslThresholdOutput();
	cv::Mat* GetMaskOutput();
	cv::Mat* GetCvErodeOutput();
	std::vector<std::vector<cv::Point>>* GetFindContoursOutput();
	std::vector<std::vector<cv::Point>>* GetFilterContoursOutput();
	std::vector<std::vector<cv::Point>>* GetGreatestContoursOutput();
	std::vector<std::vector<cv::Point>>* GetConvexHullsOutput();
	std::vector<cv::Moments>* getMoments();
	std::vector<Point2i>* getContourCenters();
	std::vector<Point2i> getBoundingPixels(const std::vector<Point2i>& contourPoints);
};

#endif
