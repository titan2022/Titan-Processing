
#include "InputModule.h"

InputModule::InputModule(const std::string& name) : Module(name) 
{

}

void InputModule::addProcessingModule(std::shared_ptr<ProcessingModule>& processingModule)
{
	for (auto& matPair : processingModule->inputMatrices)
	{
		for (int i = 0; i < sourceMatrices.size(); ++i)
		{
			if (matPair.first == sourceMatrices[i].first)
			{
				matrixSubscribers[i].push_back(&matPair.second);
				break;
			}
		}
	}

	processingModules.push_back(processingModule);
}

void InputModule::execute()
{
	for (int i = 0; i < sourceMatrices.size(); ++i)
	{
		for (int j = 0; j < matrixSubscribers[i].size(); ++j)
		{
			sourceMatrices[i].second.copyTo(*matrixSubscribers[i][j]);
		}
	}
}

void InputModule::createMatrixSubscribers()
{
	for (int i = 0; i < sourceMatrices.size(); ++i)
	{
		matrixSubscribers.push_back(std::vector<cv::Mat*>());
	}
}