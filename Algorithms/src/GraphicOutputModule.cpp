

#include <opencv2/highgui.hpp>
#include "GraphicOutputModule.h"

const vector<ImageType> GraphicOutputModule::graphicType = { ImageType::COLOR };

GraphicOutputModule::GraphicOutputModule(const std::string& name) 
	: OutputModule(name, graphicType) {}

//bool GraphicOutputModule::setProcessingModule(const std::shared_ptr<ProcessingModule>& processModule)
//{
//	auto graphicModulePointer = std::dynamic_pointer_cast<IGraphicOutputType>(processModule);
//	if (graphicModulePointer)
//	{
//		graphicProcessingModule = graphicModulePointer;
//		actualProcessingModule = processModule;
//		return true;
//	}
//	else
//	{
//		throw std::invalid_argument(processModule->getName() + " does not implement IGraphicOutputType");
//		return false;
//	}
//}

void GraphicOutputModule::execute()
{
	cv::imshow(name + " GraphicOutputModule Output", *(inputMatrices[0].second));
	cv::waitKey(1);
}