#ifndef GRAPHIC_MODULE_H
#define GRAPHIC_MODULE_H

#include "OutputModule.h"

class GraphicModule : public OutputModule
{
public:
	GraphicModule();
	bool checkModuleIsValid(std::shared_ptr<ProcessingModule> processModule) override;
	void getOutput() override;

	inline Outputs getOutputType() const override { return Outputs::GRAPHIC; }
};

#endif