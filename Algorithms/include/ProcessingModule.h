#ifndef PROCESSING_MODULE_H
#define PROCESSING_MODULE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <opencv2/core/cuda.hpp>

#include "Constants.h"
#include "Module.h"

/// <summary>
/// Abstract class for different vision processing tasks 
/// </summary>
class ProcessingModule : public Module
{
public:
	std::unordered_map<InputType, bool> inputMatricesLinked;
	std::unordered_map<InputType, cv::Mat> inputMatrices;

	/// <summary>
	/// Checks if all the inputMatrices are properly linked
	/// </summary>
	virtual void initialize() override;

	//WARNING: Load constructor after initialization of inputMatrices to properly create inputMatricesLinked variable!
	ProcessingModule(const std::string& name, const std::unordered_map<InputType, cv::Mat>& inputList);
};

#endif