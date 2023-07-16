
#include "GraphicModule.h"

GraphicModule::GraphicModule() {}

bool GraphicModule::checkModuleIsValid(std::shared_ptr<ProcessingModule> processModule)
{
	auto outputTypes = processModule->getOutputTypes();
	if (std::find(outputTypes.begin(), outputTypes.end(), getOutputType()) == outputTypes.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void GraphicModule::getOutput()
{

}