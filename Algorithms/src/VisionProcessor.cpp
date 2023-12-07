
#include "VisionProcessor.h"	

VisionProcessor::VisionProcessor() {}

void VisionProcessor::initialize()
{
	for (auto pair : inputs)
	{
		if (!pair.second->isValid())
		{
			throw std::logic_error(pair.second->getName() + " device is not loaded properly!");
		}
		else
		{
			std::cout << "All input devices are properly loaded\n";
		}
	}
	for (auto pair : inputs)
	{
		pair.second->initialize();
	}

	for (auto pair : processes)
	{
		pair.second->initialize();
	}

	for (auto pair : outputs)
	{
		pair.second->initialize();
	}

	std::cout << "Modules successfully initialized\n";
}

void VisionProcessor::execute()
{
	for (auto pair : inputs)
	{
		pair.second->execute();
	}

	for (auto pair : processes)
	{
		pair.second->execute();
	}

	for (auto pair : outputs)
	{
		pair.second->execute();
	}
}

void VisionProcessor::finalize()
{
	for (auto pair : inputs)
	{
		pair.second->finalize();
	}

	for (auto pair : processes)
	{
		pair.second->finalize();
	}

	for (auto pair : outputs)
	{
		pair.second->finalize();
	}
}

void VisionProcessor::run()
{
	initialize();
	while (true)
	{
		execute();
		finalize();
	}
}


bool VisionProcessor::addInputModule(std::shared_ptr<InputModule> inputModule)
{
	if (inputModule->isValid())
	{
		totalModules.insert(std::pair<std::string, std::shared_ptr<Module>>(inputModule->getName(), inputModule));
		inputs.insert(std::pair<std::string, std::shared_ptr<InputModule>>(inputModule->getName(), inputModule));
		return true;
	}
	else
	{
		return false;
	}
}

bool VisionProcessor::addOutputModule(std::shared_ptr<OutputModule> outputModule)
{
	totalModules.insert(std::pair<std::string, std::shared_ptr<Module>>(outputModule->getName(), outputModule));
	outputs.insert(std::pair<std::string, std::shared_ptr<OutputModule>>(outputModule->getName(), outputModule));
	return true;
}

bool VisionProcessor::addProcessingModule(std::shared_ptr<ProcessingModule> processModule)
{
	totalModules.insert(std::pair<std::string, std::shared_ptr<Module>>(processModule->getName(), processModule));
	processes.insert(std::pair<std::string, std::shared_ptr<ProcessingModule>>(processModule->getName(), processModule));
	return true;
}

//bool VisionProcessor::checkInputModuleDependencies(std::shared_ptr<InputModule> input)
//{
//	for (auto processingModule : input->getProcessingModules())
//	{
//		if (std::find(processes.begin(), processes.end(), processingModule) == processes.end())
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//bool VisionProcessor::checkOutputModuleDependencies(std::shared_ptr<OutputModule> output)
//{
//	if (std::find(processes.begin(), processes.end(), output->getProcessingModule()) == processes.end())
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
//}