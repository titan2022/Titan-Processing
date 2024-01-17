
#include "HSLFilterModule.h"
#include "Vector.h"
#include "VisionFunctions.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <iostream>

#define _USE_MATH_DEFINES

const vector<ImageType> HSLFilterModule::inputList = { ImageType::COLOR };
const vector<ImageType> HSLFilterModule::outputList = { ImageType::COLOR, ImageType::MASK };

HSLFilterModule::HSLFilterModule(const std::string& name, const std::vector<double>& hslThresholdParams) 
	: ProcessingModule(name, inputList, outputList), hslThresholdParameters(hslThresholdParams) {}

//void HSLFilterModule::push()
//{
//	if (outputModules[0].first)
//	{
//		auto graphicOutput = std::dynamic_pointer_cast<GraphicOutputModule>(outputModules[0].second);
//		inputMatrices[1].second.copyTo(graphicOutput->inputMatrices[0]);
//	}
//}

void HSLFilterModule::execute()
{	
	/*auto blurKernel = cv::Size(7, 7);
	cv::GaussianBlur(inputMatrices[1].second, inputMatrices[1].second, blurKernel, 3);*/
	cv::Mat thresholdMask;
	//std::vector<double> hslThresholdParams = { 15, 35, 45, 210, 80, 255 };
	VisionFunctions::hslThreshold(*(inputMatrices[0].second), hslThresholdParameters, thresholdMask);
	//VisionFunctions::erodeMask(thresholdMask, thresholdMask, 1);
	//VisionFunctions::mergeMask(thresholdMask, thresholdMask);
	/*auto contours = VisionFunctions::findContours(thresholdMask);*/
	//Error here in vector subscript
	//auto greatestContourCenter = VisionFunctions::getMomentBasedContourCenter(contours[0]);
	inputMatrices[0].second->copyTo(outputMatrices[0].second);
	outputMatrices[1].second = thresholdMask;

	cv::imshow("HSL Threshold Mask", thresholdMask);
	cv::waitKey(1);
	//inputMatrices[0].second->copyTo(outputMatrices[0].second);
	//cv::drawContours(outputMatrices[0].second, contours, -1, cv::Scalar(255, 0, 0), 3);
	/*cv::circle(inputMatrices[1].second, greatestContourCenter, 5, cv::Scalar(0, 255, 0), 2);*/
}

/*
cv::Mat HSLFilterModule::getData()
{
	return inputMatrices[ImageType::COLOR];
}*/

//void HSLFilterModule::printCenterRGB()
//{
//	cv::Mat hslMatrix; 
//	cv::cvtColor(colorMatrix, hslMatrix, cv::COLOR_BGR2HLS);
//	auto vec = hslMatrix.at<cv::Vec3b>(240, 424);
//	std::cout << (int) vec[0] << ' ' << (int) vec[1] << ' ' << (int) vec[2] << ' ' << '\n';
//}

//std::vector<Object*> HSLFilterModule::searchForObjects()
//{
//	//Kludge, should figure out a better way to do this format
//	/*auto redRobots = searchFor<RedRobot>();
//	auto blueRobots = searchFor<BlueRobot>();*/
//	auto purpleCubes = searchFor<Cube>();
//	auto yellowCones = searchFor<Cone>();
//	std::vector<Object*> results(purpleCubes.size() + yellowCones.size());
//
//	/*for (auto redRobotPtr : redRobots)
//	{
//		results.push_back(redRobotPtr);
//	}
//	for (auto blueRobotPtr : blueRobots)
//	{
//		results.push_back(blueRobotPtr);
//	}*/
//	for (auto purpleCubePtr : purpleCubes)
//	{
//		results.push_back(purpleCubePtr);
//	}
//	for (auto yellowCubePtr : yellowCones)
//	{
//		results.push_back(yellowCubePtr);
//	}
//
//	// This is to remove the pointers from the vectors 
//	// so that when they are deleted, the object are not deleted
//	/*redRobots.clear();
//	blueRobots.clear();*/
//	purpleCubes.clear();
//	yellowCones.clear();
//
//	return results;
//}

//std::vector<Robot*> HSLFilterModule::searchForRobots()
//{
//	std::vector<Robot*> results;
//	contourDetector.Process<RedRobot>(colorMatrix);
//	std::vector<cv::Point2i>* redRobotCoordinates = contourDetector.getContourCenters();
//	for (auto point : *redRobotCoordinates)
//	{
//		Pixel pixel(point.x, point.y, originalFrame->xResolution, originalFrame->yResolution);
//		Robot* redRobot = new RedRobot(pixel, positionMatrix);
//		results.push_back(redRobot);
//	}
//
//
//	contourDetector.Process<BlueRobot>(colorMatrix);
//	std::vector<cv::Point2i>* blueRobotCoordinates = contourDetector.getContourCenters();
//	for (auto point : *blueRobotCoordinates)
//	{
//		Pixel pixel(point.x, point.y, originalFrame->xResolution, originalFrame->yResolution);
//		Robot* blueRobot = new BlueRobot(pixel, positionMatrix);
//
//		results.push_back(blueRobot);
//	}
//
//	return results;
//}
//
//std::vector<Cone> HSLFilterModule::searchForCones()
//{
//	contourDetector.Process<Cone>(colorMatrix);
//	std::vector<cv::Point2i>* coordinates = contourDetector.getContourCenters();
//	std::vector<Cone> results(coordinates->size());
//	for (auto point : *coordinates)
//	{
//		Pixel pixel(point.x, point.y, originalFrame->xResolution, originalFrame->yResolution);
//		Cone cone(pixel, positionMatrix);
//		results.push_back(cone);
//	}
//	return results;
//}
//
//std::vector<Cube> HSLFilterModule::searchForCubes()
//{
//	contourDetector.Process<Cube>(colorMatrix);
//	std::vector<cv::Point2i>* coordinates = contourDetector.getContourCenters();
//	std::vector<Cube> results(coordinates->size());
//	for (auto point : *coordinates)
//	{
//		Pixel pixel(point.x, point.y, originalFrame->xResolution, originalFrame->yResolution);
//		Cube cube(pixel, positionMatrix);
//		results.push_back(cube);
//	}
//	return results;
//}

//
//void HSLFilterModule::showColorFrame()
//{
//	cv::imshow("Color Frame", colorMatrix);
//	cv::waitKey(1);
//}
//
//void HSLFilterModule::showContourFrame()
//{
//	Mat contourMatrix = colorMatrix;
//	cv::drawContours(contourMatrix, *contourDetector.GetFindContoursOutput(), -1, cv::Scalar(255, 0, 0));
//	std::vector<cv::Point2i>* coordinates = contourDetector.getContourCenters();
//	for (int i = 0; i < coordinates->size(); ++i)
//	{
//		circle(contourMatrix, cv::Point2i(coordinates->at(i).x, coordinates->at(i).y), 5, cv::Scalar(0, 255, 0), 2);
//	}
//	circle(contourMatrix, cv::Point2i(424, 240), 5, cv::Scalar(0, 255, 0), 2);
//	cv::imshow("Contours", contourMatrix);
//	cv::waitKey(1);
//}