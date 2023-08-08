
#include "ProcessingModule.h"

ProcessingModule::ProcessingModule(const std::string& name, const std::unordered_map<InputType, cv::Mat>& inputList) : 
	Module(name),
	inputMatrices(inputList)
{
	for (auto pair : inputMatrices)
	{
		inputMatricesLinked.insert(std::pair<InputType, bool>(pair.first, false));
	}
}

void ProcessingModule::initialize()
{
	for (auto pair : inputMatricesLinked)
	{
		if (pair.second == false)
		{
			throw std::logic_error(name + " has not linked to all necessary InputModules!");
		}
	}
}
