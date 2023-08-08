#ifndef GRAPHIC_OUTPUT_MODULE_H
#define GRAPHIC_OUTPUT_MODULE_H

#include "IGraphicOutputType.h"
#include "OutputModule.h"

class GraphicOutputModule : public OutputModule
{
public:
	GraphicOutputModule(const std::string& name);
	void execute() override;
	bool setProcessingModule(const std::shared_ptr<ProcessingModule>& processModule) override;

private:
	std::shared_ptr<IGraphicOutputType> graphicProcessingModule;
};

#endif