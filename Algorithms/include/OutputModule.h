#ifndef OUTPUT_MODULE_H
#define OUTPUT_MODULE_H

#include <vector>
#include <opencv2/core/mat.hpp>
#include "Constants.h"
#include "Input.h"
#include "Module.h"

/// <summary>
/// Abstract class for output modules which will specify how to output
/// processing module information
/// </summary>
class OutputModule : public Module, public Input
{
public:
	//std::vector<cv::Mat> inputMatrices;
	inline OutputModule(const std::string& name, const vector<ImageType>& matrixList) 
		: Module(name), Input(matrixList) {}
	
};

#endif