
#include "VisionProcessor.h"	

VisionProcessor::VisionProcessor() {}

void VisionProcessor::initialize()
{
	auto modules = getAllModules();
	for (auto module : modules)
	{
		module->initialize();
	}

	findAllModules();
}

void VisionProcessor::execute()
{
	auto modules = getAllModules();
	for (auto module : modules)
	{
		module->execute();
	}
}

void VisionProcessor::finalize()
{

}

void VisionProcessor::findAllModules()
{
	std::vector<std::shared_ptr<Module>> allModules;
	for (auto pair : inputs)
	{
		allModules.push_back(pair.second);
	}

	// Need to implement incorporating submodules of MultiModule
	for (auto pair : processes)
	{
		allModules.push_back(pair.second);
	}

	for (auto pair : outputs)
	{
		allModules.push_back(pair.second);
	}

	totalModules = allModules;
}

bool VisionProcessor::addInputModule(std::shared_ptr<InputModule> inputModule)
{
	if (inputModule->isValid())
	{
		inputs.insert(std::pair<std::string, std::shared_ptr<InputModule>>(inputModule->getName(), inputModule));
		return true;
	}
	else
	{
		return false;
	}
}

bool VisionProcessor::addOutputModule(std::shared_ptr<OutputModule> output, std::string process)
{
	if (processes.find(process) != processes.end())
	{
		auto processModule = processes[process];
		if (output->checkModuleIsValid(processModule))
		{
			outputs.insert(std::pair<std::string, std::shared_ptr<OutputModule>>(output->getName(), output));
			output->linkProcessingModule(processModule);
		}

		return false;
	}

	return false;
}