
#include "VisionProcessor.h"	

bool VisionProcessor::toRun = true;

VisionProcessor::VisionProcessor() {}

void VisionProcessor::initialize()
{
	for (auto inputMod : inputs)
	{
		if (!inputMod->isValid())
		{
			throw std::logic_error(inputMod->getName() + " device is not loaded properly!");
		}
		else
		{
			std::cout << "All input devices are properly loaded\n";
		}
	}
	for (auto mod : inputs)
	{
		mod->initialize();
	}

	for (auto mod : processes)
	{
		mod->initialize();
	}

	for (auto mod : outputs)
	{
		mod->initialize();
	}

	std::cout << "Modules successfully initialized\n";
}

void VisionProcessor::execute()
{
	for (auto mod : inputs)
	{
		mod->execute();
	}

	for (auto mod : processes)
	{
		mod->execute();
	}

	for (auto mod : outputs)
	{
		mod->execute();
	}
}

void VisionProcessor::finalize()
{
	for (auto mod : inputs)
	{
		mod->finalize();
	}

	for (auto mod : processes)
	{
		mod->finalize();
	}

	for (auto mod : outputs)
	{
		mod->finalize();
	}
}

void VisionProcessor::run()
{
	initialize();
	while (toRun)
	{
		execute();
	}
	finalize();
}


void VisionProcessor::addInputModule(InputModule* inputModule)
{
	inputs.push_back(inputModule);
}

void VisionProcessor::addOutputModule(OutputModule* outputModule)
{
	outputs.push_back(outputModule);
}

void VisionProcessor::addProcessingModule(ProcessingModule* processModule)
{
	processes.push_back(processModule);
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