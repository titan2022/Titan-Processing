#include "BlackScreenFilterModule.h"
#include "VisionFunctions.h"
#include <opencv2/highgui.hpp>

const vector<double> BlackScreenFilterModule::blackHSLThreshold = {0, 180, 0, 255, 130, 255};

BlackScreenFilterModule::BlackScreenFilterModule() 
	: HSLFilterModule("Black Screen Filter", blackHSLThreshold) 
{
	outputMatrices.push_back(pair<ImageType, Mat>(ImageType::MASK, Mat()));
}

void BlackScreenFilterModule::execute()
{
	// TODO Fix threshold bitwise not inverting black to white
	//vector<double> rgbFilter = { 170, 255, 170, 255, 170, 255 };
	VisionFunctions::hslThreshold(*(inputMatrices[0].second), hslThresholdParameters, outputMatrices[1].second);
	//VisionFunctions::mergeMask(thresholdMask, thresholdMask);
	//cv::imshow("initial mat", *(inputMatrices[0].second));
	//VisionFunctions::dilateMask(outputMatrices[1].second, outputMatrices[1].second, 2);
	//cv::imshow("thresholdMask", thresholdMask);
	Mat darkEdges;
	VisionFunctions::blackFilterMask(*(inputMatrices[0].second), darkEdges);
	cv::bitwise_or(outputMatrices[1].second, darkEdges, outputMatrices[1].second);
	//cv::imshow("Dark Edge + White Paper", outputMatrices[1].second);
	//cv::waitKey(1);
	//cv::waitKey(1);
	cv::bitwise_not(outputMatrices[1].second, outputMatrices[1].second);
	VisionFunctions::erodeMask(outputMatrices[1].second, outputMatrices[1].second, 2);
	outputMatrices[0].second = Mat(640, 480, CV_8UC3, cv::Scalar(0, 0, 0));
	inputMatrices[0].second->copyTo(outputMatrices[0].second, outputMatrices[1].second);

}

