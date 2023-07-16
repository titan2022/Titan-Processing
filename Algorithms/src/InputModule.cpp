
#include "InputModule.h"

using std::pair;

//std::map<std::string, bool> capacities()
//{
//	std::pair<std::string, bool>("Depth", true),
//	std::pair<std::string, bool>("Color", true),
//	std::pair<std::string, bool>("Position", true),
//}

void InputModule::linkProcessingModule(std::shared_ptr<ProcessingModule> process)
{
	if (checkModuleIsValid(process))
		processModules.push_back(process);
	else
		throw new std::invalid_argument("Invalid processing module!");
}

bool InputModule::checkModuleIsValid(std::shared_ptr<ProcessingModule> module) const
{
	auto capacities = getCapacities();
	for (auto requirement : module->getRequirements())
	{
		if (std::find(capacities.begin(), capacities.end(), requirement) == capacities.end())
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
			sourceMatrices.at(requirement).copyTo(modulePtr->inputMatrices[requirement]);
		}
	}
}
