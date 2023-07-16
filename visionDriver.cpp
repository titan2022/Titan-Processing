
#define _USE_MATH_DEFINES
// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.
// Include RealSense Cross Platform API
// Include short list of convenience functions for rendering
#include <string>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/cuda.hpp>
#include <cmath>
#include "RealsenseCamera.h"
#include "ContourTest.h"
#include "Cone.h"
#include "Cube.h"
#include "ObjectDetection.h"
#include "RedRobot.h"
#include "BlueRobot.h"
#include "CoordinateTransformations.h"
//#include "NetworkingClient.hpp"


int main(int argc, char* argv[])
{
//	int streamOffset = 0;
//	bool cameraDisconnected = false;
//	RealsenseCamera camera(0, 1);
//	ObjectDetection analysis;
//	NetworkingClient output("10.20.22.2", 5800);
//	camera.start();
//	while (true)
//	{
//		if (cameraDisconnected && camera.checkCameraIsConnected())
//		{
//			cameraDisconnected = false;
//			double status[1] = { -1 };
//			std::cout << "Camera reconnected\n";
//			output.sendData("disc", false, status);
//			camera = RealsenseCamera(streamOffset, streamOffset + 1);
//			camera.start();
//		}
//		else if (camera.checkCameraIsConnected())
//		{
//			CameraFrame alignedFrame;
//			try
//			{
//				camera.nextFrame();
//				alignedFrame = camera.getCameraFrame();
//			}
//			catch (const std::exception& e)
//			{
//				cameraDisconnected = true;
//				streamOffset += 3;
//				double status[1] = { 1 };
//				output.sendData("disc", false, status);
//				std::cout << "Camera disconnected\n";
//			}
//			analysis.load(alignedFrame);
//			auto cones = analysis.searchFor<Cone>();
//			auto cubes = analysis.searchFor<Cube>();
//			//analysis.showContourFrame();
//			//analysis.printCenterRGB();
//			for (auto objectPtr : cones)
//			{
//				CoordinateTransformations::objectTransform(*objectPtr);
//				std::cout << objectPtr->toString() << '\n';
//				double vector[3] = { objectPtr->getPosition().x, objectPtr->getPosition().z, objectPtr->getPosition().y };
//				output.sendData("cone", false, vector);
//			}
//			for (auto cubePtr : cubes)
//			{
//				CoordinateTransformations::objectTransform(*cubePtr);
//				std::cout << cubePtr->toString() << '\n';
//				double vector[3] = { cubePtr->getPosition().x, cubePtr->getPosition().z, cubePtr->getPosition().y };
//				output.sendData("cube", false, vector);
//			}
//
//		}
//		else
//		{
//			cameraDisconnected = true;
//			streamOffset += 3;
//			double status[1] = { 1 };
//			std::cout << "Camera disconnected\n";
//			output.sendData("disc", false, status);
//		}
//	}

	//RealsenseCamera camera;
	//ObjectDetection analysis;
	////NetworkingClient output("127.0.0.1", 5800);
	//camera.start();
	//std::cout << "Detected Camera\n";
	//while (true)
	//{
	//	camera.nextFrame();
	//	CameraFrame alignedFrame = camera.getCameraFrame();
	//	analysis.load(alignedFrame);
	//	std::vector<Object*> objects = analysis.searchForObjects();
	//	for (auto objectPtr : objects)
	//	{
	//		objectPtr->getPosition().y = -objectPtr->getPosition().y;
	//		CoordinateTransformations::objectTransform(*objectPtr);
	//		std::cout << objectPtr->toString() << '\n';
	//		/*double vector[3] = { objectPtr->getPosition().x, objectPtr->getPosition().y, objectPtr->getPosition().z };
	//		if (typeid(*objectPtr) == typeid(Cone))
	//		{
	//			output.sendData("cone", false, vector);
	//		} else if (typeid(*objectPtr) == typeid(Cube))
	//		{
	//			output.sendData("cube", false, vector);
	//		}*/
	//	}
	//}


	return 1;
}

//
//int main()
//{
//	Vector vec(0, 1, 0);
//	vec.rotateVector(M_PI, M_PI, M_PI, "YZX");
//	std::cout << vec.toString() << '\n';
//}
// Delta U is 0.001179
// Delta V 0.0020833
// 848 across the screen
// 480 down the screen
