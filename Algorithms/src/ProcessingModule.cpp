
#include "ProcessingModule.h"

ProcessingModule::ProcessingModule(const std::string& name) : Module(name) {}

void ProcessingModule::activateOutputModule(int num)
{
	outputModules[num].first = true;
}