#ifndef MULTI_MODULE_H
#define MULTI_MODULE_H

#include "OutputModule.h"
#include "ProcessingModule.h"

class MultiModule : public ProcessingModule
{
protected:
	std::vector<std::shared_ptr<ProcessingModule>> subModules;
public:
	void initialize() override;
};

#endif