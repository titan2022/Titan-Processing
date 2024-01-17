
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
#include <ctime>
#include "BlackScreenFilterModule.h"
#include "RealsenseCameraModule.h"
#include "HSLFilterModule.h"
#include "CoordinateTransformations.h"
#include "GraphicOutputModule.h"
#include "HistogramOutputModule.h"
#include "HistProjectionModule.h"
#include "FileInputModule.h"
#include "VisionProcessor.h"
//#include "NetworkingClient.hpp"


int main(int argc, char* argv[])
{
	//vector<pair<int, Mat>> test1 = { pair<int, Mat>(1, Mat(5, 5, 1)) };
	//vector<pair<int, Mat&>> test2 = { pair<int, Mat&>(2, Mat(3, 3, 1)) };

	//pair<int, Mat> test3(1, Mat(7, 7, 1));
	//pair<int, Mat*> test4(1, &(test3.second));
	//
	//Mat a(10, 10, 1);
	//a.copyTo(test3.second);

	//test4.second = test3.second;
	//
	//test2[0].second = test1[0].second;

	//Mat a(9, 9, 1);
	//a.copyTo(test3.second);
	//a.copyTo(test1[0].second);
	VisionProcessor analyzer;

	RealsenseCameraModule* inputModule = new RealsenseCameraModule("RealsenseCameraModule", 0, 1);
	std::cout << "Camera connected\n";
	HSLFilterModule* hslFilterOutputModule = new HSLFilterModule("Note Filter", { 0, 40, 60, 256, 30, 230 });
	//BlackScreenFilterModule* processModule = new BlackScreenFilterModule();
	GraphicOutputModule* videoOutput = new GraphicOutputModule("Post Hist Image");
	HistogramOutputModule* createHistMod = new HistogramOutputModule("Note_Histogram", {0, 2}, {90, 128}, {0, 180, 0, 256}, true, "Note_Histogram_2.json");
	HistProjectionModule* projectionMod = new HistProjectionModule("Note_Histogram", { 0, 2}, { 0, 180, 0, 256 }, 5, "Note_Histogram_2.json");
	FileInputModule* fileInputMod = new FileInputModule("FileInput", "C:/Users/noone/Desktop/Code/Titan-Processing/out/build/x64-debug/note_images");

	//inputModule->addSubscriber(projectionMod);
	//projectionMod->addSubscriber(videoOutput);

	//analyzer.addInputModule(inputModule);
	//analyzer.addProcessingModule(projectionMod);
	//analyzer.addOutputModule(videoOutput);


	fileInputMod->addSubscriber(hslFilterOutputModule);
	hslFilterOutputModule->addSubscriber(videoOutput);
	hslFilterOutputModule->addSubscriber(createHistMod);

	analyzer.addInputModule(fileInputMod);
	analyzer.addProcessingModule(hslFilterOutputModule);
	analyzer.addOutputModule(videoOutput);
	analyzer.addOutputModule(createHistMod);

	std::cout << "Setup complete\n";

	time_t duration = 10;
	time_t endTime = std::time(NULL) + duration;

	analyzer.run();
//	int streamOffset = 0;
//	bool cameraDisconnected = false;
//	RealsenseCameraModule camera(0, 1);
//	HSLFilterModule analysis;
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
//			camera = RealsenseCameraModule(streamOffset, streamOffset + 1);
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

	//RealsenseCameraModule camera;
	//HSLFilterModule analysis;
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
