#include "RealsenseCamera.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "RealsenseFrame.h"

const double RealsenseCamera::exposure = 100;
const double RealsenseCamera::gain = 64;
const double RealsenseCamera::brightness = 5;
const double RealsenseCamera::contrast = 54;
const double RealsenseCamera::gamma = 300;
const double RealsenseCamera::hue = 17;
const double RealsenseCamera::saturation = 52;
const double RealsenseCamera::sharpness = 50;
const double RealsenseCamera::whiteBalance = 4600;
const double RealsenseCamera::maximumDistance = 20;
const size_t RealsenseCamera::xResolution = 640;
const size_t RealsenseCamera::yResolution = 480;
const int RealsenseCamera::frameRate = 15;
const double RealsenseCamera::timeOut = 1000;

const rs2::align RealsenseCamera::frameAligner(RS2_STREAM_DEPTH);
const int RealsenseCamera::alignmentType = RS2_STREAM_DEPTH;

RealsenseCamera::RealsenseCamera(const std::string& name, int depthIndex, int colorIndex) :
	InputModule(name),
	depthSensor(context.query_all_sensors()[depthIndex]),
	colorSensor(context.query_all_sensors()[colorIndex])
{
	sourceMatrices = 
	{
		std::pair<InputType, cv::Mat>(InputType::DEPTH, cv::Mat()),
		std::pair<InputType, cv::Mat>(InputType::COLOR, cv::Mat()),
		std::pair<InputType, cv::Mat>(InputType::POSITION, cv::Mat())
	};

	createMatrixSubscribers();
}


void RealsenseCamera::initialize()
{
	config();
	pipeline.start();
}

void RealsenseCamera::config()
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

void RealsenseCamera::nextFrame()
{
	currentFrameSet = pipeline.wait_for_frames(timeOut);
}

RealsenseFrame RealsenseCamera::getFrame()
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

bool RealsenseCamera::isValid() const
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

void RealsenseCamera::execute()
{
	nextFrame();
	auto frame = getFrame();

	sourceMatrices[0].second = frame.getDepthMatrix();
	sourceMatrices[1].second = frame.getColorMatrix();
	sourceMatrices[2].second = frame.getPositionMatrix();

	InputModule::execute();

	//for (auto depth : depthSubscribers)
	//{
	//	sourceMatrices[InputType::DEPTH].copyTo(depth->inputMatrices[InputType::DEPTH]);
	//}

	//for (auto color : colorSubscribers)
	//{
	//	sourceMatrices[InputType::COLOR].copyTo(color->inputMatrices[InputType::COLOR]);
	//	processingModules[0]->inputMatrices[InputType::COLOR];
	//}

	//for (auto position : positionSubscribers)
	//{
	//	sourceMatrices[InputType::POSITION].copyTo(position->inputMatrices[InputType::POSITION]);
	//}
}
//
//void RealsenseCamera::addProcessingModule(const std::shared_ptr<ProcessingModule>& processingModule)
//{
//	InputModule::addProcessingModule(processingModule);
//	for (auto pair : processingModule->inputMatrices)
//	{
//		if (pair.first == InputType::DEPTH)
//		{
//			depthSubscribers.push_back(processingModule);
//			processingModule->inputMatricesLinked[InputType::DEPTH] = true;
//		} 
//		else if (pair.first == InputType::COLOR)
//		{
//			colorSubscribers.push_back(processingModule);
//			processingModule->inputMatricesLinked[InputType::COLOR] = true;
//		} 
//		else if (pair.first == InputType::POSITION)
//		{
//			positionSubscribers.push_back(processingModule);
//			processingModule->inputMatricesLinked[InputType::POSITION] = true;
//		}
//	}
//}