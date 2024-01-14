#include "RealsenseCameraModule.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "RealsenseFrame.h"

const double RealsenseCameraModule::exposure = 156;
const double RealsenseCameraModule::gain = 55;
const double RealsenseCameraModule::brightness = 0;
const double RealsenseCameraModule::contrast = 45;
const double RealsenseCameraModule::gamma = 300;
const double RealsenseCameraModule::hue = 10;
const double RealsenseCameraModule::saturation = 31;
const double RealsenseCameraModule::sharpness = 50;
const double RealsenseCameraModule::whiteBalance = 4100;
const double RealsenseCameraModule::maximumDistance = 20;
const size_t RealsenseCameraModule::xResolution = 640;
const size_t RealsenseCameraModule::yResolution = 480;
const int RealsenseCameraModule::frameRate = 15;
const double RealsenseCameraModule::timeOut = 1000;
const rs2::align RealsenseCameraModule::frameAligner(RS2_STREAM_DEPTH);
const int RealsenseCameraModule::alignmentType = RS2_STREAM_DEPTH;
const vector<ImageType> RealsenseCameraModule::types = { ImageType::DEPTH, ImageType::COLOR, ImageType::POSITION };

RealsenseCameraModule::RealsenseCameraModule(const std::string& name, int depthIndex, int colorIndex) 
	: InputModule(name, types), depthSensor(context.query_all_sensors()[depthIndex]), colorSensor(context.query_all_sensors()[colorIndex]) {}


void RealsenseCameraModule::initialize()
{
	config();
	pipeline.start();
}

void RealsenseCameraModule::config()
{
	rs2::config configuration;
	//configuration.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_ANY, 6);
	configuration.enable_stream(RS2_STREAM_DEPTH, xResolution, yResolution, RS2_FORMAT_ANY, frameRate);
	configuration.enable_stream(RS2_STREAM_COLOR, xResolution, yResolution, RS2_FORMAT_ANY, frameRate);
	configuration.disable_stream(RS2_STREAM_GYRO);
	configuration.disable_stream(RS2_STREAM_ACCEL);
	colorSensor.set_option(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, false);
	colorSensor.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, false);
	colorSensor.set_option(RS2_OPTION_BRIGHTNESS, brightness);
	colorSensor.set_option(RS2_OPTION_CONTRAST, contrast);
	colorSensor.set_option(RS2_OPTION_EXPOSURE, exposure);
	colorSensor.set_option(RS2_OPTION_GAIN, gain);
	colorSensor.set_option(RS2_OPTION_GAMMA, gamma);
	colorSensor.set_option(RS2_OPTION_HUE, hue);
	colorSensor.set_option(RS2_OPTION_SATURATION, saturation);
	colorSensor.set_option(RS2_OPTION_SHARPNESS, sharpness);
	colorSensor.set_option(RS2_OPTION_WHITE_BALANCE, whiteBalance);
	//depthSensor.set_option(RS2_OPTION_MAX_DISTANCE, maximumDistance);
}

void RealsenseCameraModule::nextFrame()
{
	currentFrameSet = pipeline.wait_for_frames(timeOut);
}

RealsenseFrame RealsenseCameraModule::getFrame()
{	
	//Aligns the depth and video frame together
	rs2::frameset alignedFrame = frameAligner.process(currentFrameSet);

	//Gets the aligned data and stores it in the OpenCV Matrices
	rs2::video_frame videoFrame = alignedFrame.get_color_frame();
	rs2::depth_frame depthFrame = alignedFrame.get_depth_frame();
	rs2::pointcloud pointCloudCalculator;
	rs2::points points = pointCloudCalculator.calculate(depthFrame);

	auto depthMatrix = cv::Mat(cv::Size(xResolution, yResolution), CV_16UC1, (void*) depthFrame.get_data());
	depthMatrix.convertTo(depthMatrix, CV_64FC1);
	depthMatrix = depthMatrix * depthFrame.get_units();
	auto colorMatrix = cv::Mat(cv::Size(xResolution, yResolution), CV_8UC3, (void*) videoFrame.get_data());
	cv::cvtColor(colorMatrix, colorMatrix, cv::COLOR_RGB2BGR);
	cv::Mat positionMatrix;
	cv::Mat(cv::Size(xResolution, yResolution), CV_32FC3, (void*) points.get_data()).copyTo(positionMatrix);
	/*cv::imshow("Position", *publicPositionMatrix);
	cv::waitKey(1);*/

	return RealsenseFrame(depthMatrix, colorMatrix, positionMatrix);
}

bool RealsenseCameraModule::isValid() const
{
	rs2::context checkerContext;
	if (checkerContext.query_devices().size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void RealsenseCameraModule::execute()
{
	nextFrame();
	auto frame = getFrame();

	outputMatrices[0].second = frame.getDepthMatrix();
	outputMatrices[1].second = frame.getColorMatrix();
	outputMatrices[2].second = frame.getPositionMatrix();
	//for (auto depth : depthSubscribers)
	//{
	//	outputMatrices[ImageType::DEPTH].copyTo(depth->inputMatrices[ImageType::DEPTH]);
	//}

	//for (auto color : colorSubscribers)
	//{
	//	outputMatrices[ImageType::COLOR].copyTo(color->inputMatrices[ImageType::COLOR]);
	//	processingModules[0]->inputMatrices[ImageType::COLOR];
	//}

	//for (auto position : positionSubscribers)
	//{
	//	outputMatrices[ImageType::POSITION].copyTo(position->inputMatrices[ImageType::POSITION]);
	//}
}
//
//void RealsenseCameraModule::addProcessingModule(const std::shared_ptr<ProcessingModule>& processingModule)
//{
//	InputModule::addProcessingModule(processingModule);
//	for (auto pair : processingModule->inputMatrices)
//	{
//		if (pair.first == ImageType::DEPTH)
//		{
//			depthSubscribers.push_back(processingModule);
//			processingModule->inputMatricesLinked[ImageType::DEPTH] = true;
//		} 
//		else if (pair.first == ImageType::COLOR)
//		{
//			colorSubscribers.push_back(processingModule);
//			processingModule->inputMatricesLinked[ImageType::COLOR] = true;
//		} 
//		else if (pair.first == ImageType::POSITION)
//		{
//			positionSubscribers.push_back(processingModule);
//			processingModule->inputMatricesLinked[ImageType::POSITION] = true;
//		}
//	}
//}