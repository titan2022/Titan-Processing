#ifndef VISION_PROCESSOR_H
#define VISION_PROCESSOR_H

#include <vector>
#include "InputModule.h"
#include "OutputModule.h"

class VisionProcessor
{
private:
	std::vector<std::shared_ptr<Module>> totalModules;
	std::unordered_map<std::string, std::shared_ptr<InputModule>> inputs;
	std::unordered_map<std::string, std::shared_ptr<OutputModule>> outputs;

	/// <summary>
	/// Will be run at the end initialize();
	/// </summary>
	void findAllModules();
	//For each run loop of the Vision Processor
	void execute();
	void finalize();
public:
	std::unordered_map<std::string, std::shared_ptr<ProcessingModule>> processes;

	VisionProcessor();
	void initialize();

	/// <summary>
	/// Runs the actual program
	/// </summary>
	void run();

	inline std::shared_ptr<OutputModule> getOutputModule(std::string outputModule) { return outputs[outputModule]; }

	bool addInputModule(std::shared_ptr<InputModule> inputModule);
	bool addOutputModule(std::shared_ptr<OutputModule> output, std::string process);

	/// <summary>
	/// Returns them in the order of input, processing, output
	/// </summary>
	/// <returns></returns>
	inline const std::vector<std::shared_ptr<Module>>& getAllModules() { return totalModules; }
};

#endif