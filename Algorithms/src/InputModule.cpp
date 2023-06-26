
#include "InputModule.h"

using std::pair;

//std::map<std::string, bool> capacities()
//{
//	std::pair<std::string, bool>("Depth", true),
//	std::pair<std::string, bool>("Color", true),
//	std::pair<std::string, bool>("Position", true),
//}

void InputModule::registerProcessingModule(std::shared_ptr<ProcessingModule> process)
{
	if (checkModuleIsValid(process))
		processModules.push_back(process);
	else
		throw new std::invalid_argument("Invalid processing module!");
}

bool InputModule::checkModuleIsValid(std::shared_ptr<ProcessingModule> module) const
{
	for (auto requirement : module->getRequirements())
	{
		if (std::count(capacities.begin(), capacities.end(), requirement) < 1)
			return false;
	}

	return true;
}

void InputModule::pushData() const
{
	for (auto modulePtr : processModules)
	{
		for (auto requirement : modulePtr->getRequirements())
		{
			sourceMatrices[requirement].copyTo(modulePtr->matrices[requirement]);
		}
	}
}

