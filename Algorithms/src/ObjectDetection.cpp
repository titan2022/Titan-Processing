#define _USE_MATH_DEFINES

#include "../include/ObjectDetection.h"
#include "../include/Vector.h"
#include "../include/RedRobot.h"
#include "../include/BlueRobot.h"
#include <opencv2/highgui.hpp>
#include <cmath>
#include <iostream>

const std::vector<Requirements> ObjectDetection::requirements = { Requirements::COLOR, Requirements::POSITION };
const std::vector<Outputs> ObjectDetection::outputs = { Outputs::GRAPHIC };

ObjectDetection::ObjectDetection() { }

void ObjectDetection::initialize()
{
	inputMatrices.insert(std::pair<Requirements, cv::Mat>(Requirements::COLOR, cv::Mat()));
	inputMatrices.insert(std::pair<Requirements, cv::Mat>(Requirements::POSITION, cv::Mat()));
	outputMatrices.insert(std::pair<Outputs, cv::Mat>(Outputs::GRAPHIC, cv::Mat()));
}

void ObjectDetection::execute()
{
	//contourDetector.Process<T>(matrices["Color"]);
}

//void ObjectDetection::printCenterRGB()
//{
//	cv::Mat hslMatrix; 
//	cv::cvtColor(colorMatrix, hslMatrix, cv::COLOR_BGR2HLS);
//	auto vec = hslMatrix.at<cv::Vec3b>(240, 424);
//	std::cout << (int) vec[0] << ' ' << (int) vec[1] << ' ' << (int) vec[2] << ' ' << '\n';
//}

//std::vector<Object*> ObjectDetection::searchForObjects()
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

//std::vector<Robot*> ObjectDetection::searchForRobots()
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
//std::vector<Cone> ObjectDetection::searchForCones()
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
//std::vector<Cube> ObjectDetection::searchForCubes()
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
//void ObjectDetection::showColorFrame()
//{
//	cv::imshow("Color Frame", colorMatrix);
//	cv::waitKey(1);
//}
//
//void ObjectDetection::showContourFrame()
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