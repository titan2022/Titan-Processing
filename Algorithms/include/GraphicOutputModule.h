#ifndef GRAPHIC_OUTPUT_MODULE_H
#define GRAPHIC_OUTPUT_MODULE_H

#include <opencv2/core/mat.hpp>
#include "OutputModule.h"

class GraphicOutputModule : public OutputModule
{
public:
	GraphicOutputModule(const std::string& name);
	virtual void execute() override;
private:
	static const vector<ImageType> graphicType;
};

#endif