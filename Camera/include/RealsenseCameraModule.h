#ifndef REALSENSE_CAMERA_MODULE_H
#define REALSENSE_CAMERA_MODULE_H

#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_pipeline.hpp>
#include "InputModule.h"

class RealsenseCameraModule : public InputModule
{
public:
	RealsenseCameraModule(const std::string& name, int depthIndex, int colorIndex);

	static const size_t xResolution, yResolution;
	static const int frameRate;

	//Iteration
	void initialize() override;
	void config();
	void execute() override;
	bool isValid() const override;
protected:
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

	static const vector<ImageType> types;
};

#endif