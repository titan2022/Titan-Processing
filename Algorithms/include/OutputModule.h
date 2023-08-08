#ifndef OUTPUT_MODULE_H
#define OUTPUT_MODULE_H

#include "ProcessingModule.h"

/// <summary>
/// Abstract class for output modules which will specify how to output
/// processing module information
/// </summary>
class OutputModule : public Module
{
public:
	OutputModule(const std::string& name);
	/// <summary>
	/// To store the processing module casted to the OutputType interface for ease of executing code
	/// </summary>
	/// <param name="processingMod"></param>
	virtual bool setProcessingModule(const std::shared_ptr<ProcessingModule>& processingMod) = 0;

protected:
	std::shared_ptr<ProcessingModule> actualProcessingModule;
	/// <summary>
	/// Points to the processingModule as a module to perform regular methods while the other points to it as an OutputType getData
	/// </summary>
	
};

#endif