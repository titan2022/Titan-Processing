

#include <opencv2/highgui.hpp>
#include "GraphicOutputModule.h"

#include "IGraphicOutputType.h"

GraphicOutputModule::GraphicOutputModule(const std::string& name) : OutputModule(name) {}

bool GraphicOutputModule::setProcessingModule(const std::shared_ptr<ProcessingModule>& processModule)
{
	auto graphicModulePointer = std::dynamic_pointer_cast<IGraphicOutputType>(processModule);
	if (graphicModulePointer)
	{
		graphicProcessingModule = graphicModulePointer;
		actualProcessingModule = processModule;
		return true;
	}
	else
	{
		throw std::invalid_argument(processModule->getName() + " does not implement IGraphicOutputType");
		return false;
	}
}

void GraphicOutputModule::execute()
{
	auto data = graphicProcessingModule->getData();
	cv::imshow(actualProcessingModule->getName() + "'s graphic output", data);
	cv::waitKey(1);
}