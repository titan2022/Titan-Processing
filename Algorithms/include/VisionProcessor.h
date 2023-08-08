#ifndef VISION_PROCESSOR_H
#define VISION_PROCESSOR_H

#include <vector>
#include "InputModule.h"
#include "OutputModule.h"

class VisionProcessor
{
private:
	std::unordered_map<std::string, std::shared_ptr<Module>> totalModules;

	//For each run loop of the Vision Processor
	void execute();
	//void finalize();

public:
	std::unordered_map<std::string, std::shared_ptr<InputModule>> inputs;
	std::unordered_map<std::string, std::shared_ptr<OutputModule>> outputs;
	std::unordered_map<std::string, std::shared_ptr<ProcessingModule>> processes;

	VisionProcessor();
	void initialize();

	/// <summary>
	/// Runs the actual program
	/// </summary>
	void run();

	inline std::shared_ptr<InputModule> getInputModule(std::string inputModule) { return inputs[inputModule]; }
	inline std::shared_ptr<ProcessingModule> getProcessingModule(std::string processModule) { return processes[processModule]; }
	inline std::shared_ptr<OutputModule> getOutputModule(std::string outputModule) { return outputs[outputModule]; }

	bool addInputModule(std::shared_ptr<InputModule> inputModule);
	bool addOutputModule(std::shared_ptr<OutputModule> outputModule);
	bool addProcessingModule(std::shared_ptr<ProcessingModule> processingModule);

};

#endif