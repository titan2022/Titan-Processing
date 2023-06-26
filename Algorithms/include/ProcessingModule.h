#ifndef PROCESSING_MODULE_H
#define PROCESSING_MODULE_H

#include <unordered_map>
#include <string>
#include <vector>

#include <opencv2/core/cuda.hpp>

#include "Module.h"

/// <summary>
/// Abstract class for different vision processing tasks 
/// </summary>
class ProcessingModule : public Module
{
protected:
	std::vector<std::string> requirements;
public:
	inline const std::vector<std::string>& getRequirements() const { return requirements; }
	std::unordered_map<std::string, cv::Mat> matrices;
};

#endif