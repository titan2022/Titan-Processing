
#include "VisionProcessor.h"	

VisionProcessor::VisionProcessor() {}

void VisionProcessor::initialize()
{
	for (auto module : totalModules)
	{
		module->initialize();
	}
}

void VisionProcessor::execute()
{
	for (auto module : totalModules)
	{
		module->execute();
	}
}

void VisionProcessor::finalize()
{
	for (auto module : totalModules)
	{
		module->finalize();
	}
}

bool VisionProcessor::addInputModule(std::shared_ptr<InputModule> inputModule)
{
	if (inputModule->isValid())
	{
		totalModules.push_back(inputModule);
		inputs.push_back(inputModule);
		return true;
	}
	else
	{
		return false;
	}
}

bool VisionProcessor::addOutputModule(std::shared_ptr<OutputModule> output)
{
	totalModules.push_back(output);
	outputs.push_back(output);
	return true;
}

bool VisionProcessor::addProcessingModule(std::shared_ptr<ProcessingModule> process)
{
	totalModules.push_back(process);
	processes.push_back(process);
	return true;
}

bool VisionProcessor::checkInputModuleDependencies(std::shared_ptr<InputModule> input)
{
	for (auto processingModule : input->getProcessingModules())
	{
		if (std::find(processes.begin(), processes.end(), processingModule) == processes.end())
		{
			return false;
		}
	}

	return true;
}
bool VisionProcessor::checkOutputModuleDependencies(std::shared_ptr<OutputModule> output)
{
	if (std::find(processes.begin(), processes.end(), output->getProcessingModule()) == processes.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}