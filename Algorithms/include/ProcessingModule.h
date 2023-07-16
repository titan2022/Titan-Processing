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
	std::vector<Requirements> getRequirements() const;
	std::vector<Outputs> getOutputTypes() const;


	std::unordered_map<Requirements, cv::Mat> inputMatrices;
	std::unordered_map<Outputs, cv::Mat> outputMatrices;
};

#endif