
#include "InputModule.h"

InputModule::InputModule(const std::string& name) : Module(name) 
{
}

void InputModule::addProcessingModule(const std::shared_ptr<ProcessingModule>& processingModule)
{
	processingModules.push_back(processingModule);
}