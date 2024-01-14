#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>

using std::vector;
using cv::Mat;

/// <summary>
/// Abstract class to be loaded into VisionProcessor
/// </summary>
class Module
{
public:
	inline const std::string& getName() { return name; }
	inline virtual void initialize() { }
	inline virtual void execute() { }
	inline virtual void finalize() { }
protected:
	Module(const std::string& i_name);
	std::string name;
};

#endif