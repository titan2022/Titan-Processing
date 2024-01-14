#ifndef PROCESSING_MODULE_H
#define PROCESSING_MODULE_H

#include <map>
#include <string>
#include <vector>
#include <opencv2/core/cuda.hpp>

#include "Constants.h"
#include "Module.h"
#include "Input.h"
#include "Output.h"
#include "OutputModule.h"
#include "GraphicOutputModule.h"

/// <summary>
/// Abstract class for different vision processing tasks 
/// </summary>
class ProcessingModule : public Module, public Input, public Output
{
public:
	//std::vector<std::pair<ImageType, cv::Mat>> inputMatrices;

	/// <summary>
	/// Checks if all the inputMatrices are properly linked
	/// </summary>
	//virtual void initialize() override;

	//WARNING: Load constructor after initialization of inputMatrices to properly create inputMatricesLinked variable!
	inline ProcessingModule(const std::string& name, const vector<ImageType>& inputMatrixList, const vector<ImageType>& outputMatrixList) 
		: Module(name), Input(inputMatrixList), Output(outputMatrixList) {}
	//inline void addSubscriber(OutputModule* outputMod)
	//{
	//	for (auto& pair : outputMatrices)
	//	{
	//		if (pair.first == outputMod->outputType)
	//		{
	//			outputMod->inputMatrices.push_back(&(pair.second));
	//		}
	//	} 
	//}
	//void activateOutputModule(int num);
	//virtual void push() = 0;
	//inline std::shared_ptr<OutputModule>& getOutputModule(int num) { return outputModules[num].second; }
	//std::vector<std::pair<bool,std::shared_ptr<OutputModule>>> outputModules;
};

#endif