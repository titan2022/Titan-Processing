#ifndef REALSENSE_CAMERA_H
#define REALSENSE_CAMERA_H

#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_pipeline.hpp>
#include "InputModule.h"
#include "RealsenseFrame.h"

class RealsenseCamera : public InputModule
{
private:
	/// <summary>
	/// Default camera parameters
	/// </summary>
	static const double exposure; 
	static const double gain;
	static const double brightness;
	static const double contrast;
	static const double gamma;
	static const double hue;
	static const double saturation;
	static const double sharpness;
	static const double whiteBalance;
	static const double maximumDistance;
	static const double timeOut;

	static const rs2::align frameAligner;
	static const int alignmentType;

	rs2::context context;
	rs2::color_sensor colorSensor;
	rs2::depth_sensor depthSensor;
	rs2::pipeline pipeline;
	rs2::frameset currentFrameSet;
	rs2::colorizer colorMap;

public:
	RealsenseCamera(int depthIndex, int colorIndex);
	
	static const size_t xResolution, yResolution;
	static const int frameRate;

	//Iteration
	void initialize() override;
	void config();
	void nextFrame();
	RealsenseFrame getFrame();
	void execute() override;
	bool isValid() const override;
};

#endif
