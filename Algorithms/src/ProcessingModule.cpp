
#include "ProcessingModule.h"

std::vector<Requirements> ProcessingModule::getRequirements() const
{
	std::vector<Requirements> requirements(inputMatrices.size());
	for (auto pair : inputMatrices)
	{
		requirements.push_back(pair.first);
	}

	return requirements;
}

std::vector<Outputs> ProcessingModule::getOutputTypes() const
{
	std::vector<Requirements> outputsTypes(outputMatrices.size());
	for (auto pair : outputMatrices)
	{
		requirements.push_back(pair.first);
	}

	return outputsTypes;
}