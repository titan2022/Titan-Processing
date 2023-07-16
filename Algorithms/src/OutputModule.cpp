
#include "OutputModule.h"

bool OutputModule::checkModuleIsValid(std::shared_ptr<ProcessingModule> processModule) const
{
	auto outputTypes = processModule->getOutputTypes();
	if (std::find(outputTypes.begin(), outputTypes.end(), getOutputType()) != outputTypes.end())
		return true;
	else
		return false;
}

bool OutputModule::linkProcessingModule(std::shared_ptr<ProcessingModule> processModule)
{
	if (checkModuleIsValid(processModule))
	{
		this->processModule = processModule;
		return true;
	}
	else
		return false;
}
