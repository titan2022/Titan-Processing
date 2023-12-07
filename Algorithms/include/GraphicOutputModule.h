#ifndef GRAPHIC_OUTPUT_MODULE_H
#define GRAPHIC_OUTPUT_MODULE_H

#include "OutputModule.h"

class GraphicOutputModule : public OutputModule
{
public:
	cv::Mat graphicOutput;

	GraphicOutputModule(const std::string& name);
	void execute() override;
};

#endif