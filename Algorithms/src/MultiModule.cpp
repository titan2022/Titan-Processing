
#include "MultiModule.h"

void MultiModule::initialize()
{
	//Setting up requirements
	for (auto modulePtr : subModules)
	{
		for (auto requirement : modulePtr->getRequirements())
		{
			if (std::count(requirements.begin(), requirements.end(), requirement) < 1)
				requirements.push_back(requirement);
		}
	}
}
