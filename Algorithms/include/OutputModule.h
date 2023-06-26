#ifndef OUTPUT_MODULE_H
#define OUTPUT_MODULE_H

#include "ProcessingModule.h"

/// <summary>
/// Abstract class for output modules which will specify how to output
/// processing module information
/// </summary>
class OutputModule : public Module
{
protected:
	std::shared_ptr<ProcessingModule> processModule;
public:
	virtual bool checkModuleIsValid(std::shared_ptr<ProcessingModule> processModule) const = 0;
	bool linkProcessingModule(std::shared_ptr<ProcessingModule> processModule);
	virtual void getOutput() const = 0;
};

#endif