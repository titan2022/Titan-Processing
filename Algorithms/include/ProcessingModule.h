#ifndef PROCESSING_MODULE_H
#define PROCESSING_MODULE_H

#include <map>
#include <string>
#include <vector>
#include <opencv2/core/cuda.hpp>

#include "Constants.h"
#include "Module.h"
#include "OutputModule.h"
#include "GraphicOutputModule.h"

/// <summary>
/// Abstract class for different vision processing tasks 
/// </summary>
class ProcessingModule : public Module
{
public:
	std::vector<std::pair<InputType, cv::Mat>> inputMatrices;

	/// <summary>
	/// Checks if all the inputMatrices are properly linked
	/// </summary>
	//virtual void initialize() override;

	//WARNING: Load constructor after initialization of inputMatrices to properly create inputMatricesLinked variable!
	ProcessingModule(const std::string& name);
	void activateOutputModule(int num);
	inline std::shared_ptr<OutputModule>& getOutputModule(int num) { return outputModules[num].second; }
protected:
	std::vector<std::pair<bool,std::shared_ptr<OutputModule>>> outputModules;
};

#endif