
#include "OutputModule.h"

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

