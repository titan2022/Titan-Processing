#ifndef VISION_PROCESSOR_H
#define VISION_PROCESSOR_H

#include <vector>
#include "InputModule.h"
#include "OutputModule.h"

class VisionProcessor
{
private:
	std::vector<std::shared_ptr<Module>> totalModules;

	//For each run loop of the Vision Processor
	void execute();
	void finalize();

	bool checkInputModuleDependencies(std::shared_ptr<InputModule> input);
	bool checkOutputModuleDependencies(std::shared_ptr<OutputModule> output);

public:
	std::vector<std::shared_ptr<InputModule>> inputs;
	std::vector<std::shared_ptr<OutputModule>> outputs;
	std::vector<std::shared_ptr<ProcessingModule>> processes;

	VisionProcessor();
	void initialize();

	/// <summary>
	/// Runs the actual program
	/// </summary>
	void run();

	inline std::shared_ptr<OutputModule> getOutputModule(std::string outputModule) { return outputs[outputModule]; }

	bool addInputModule(std::shared_ptr<InputModule> inputModule);
	bool addOutputModule(std::shared_ptr<OutputModule> output);
	bool addProcessingModule(std::shared_ptr<ProcessingModule> processingModule);

	/// <summary>
	/// Returns them in the order of input, processing, output
	/// </summary>
	/// <returns></returns>
	inline const std::vector<std::shared_ptr<Module>>& getAllModules() { return totalModules; }
};

#endif