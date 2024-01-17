#ifndef VISION_PROCESSOR_H
#define VISION_PROCESSOR_H

#include <vector>
#include "InputModule.h"
#include "OutputModule.h"

using std::string;

class VisionProcessor
{
public:
	vector<Module*> totalModules;
	vector<InputModule*> inputs;
	vector<OutputModule*> outputs;
	vector<ProcessingModule*> processes;

	VisionProcessor();

	void initialize();
	void execute();
	void finalize();

	/// <summary>
	/// Runs the actual program
	/// </summary>
	void run();

	//inline std::shared_ptr<InputModule> getInputModule(std::string inputModule) { return inputs[inputModule]; }
	//inline std::shared_ptr<ProcessingModule> getProcessingModule(std::string processModule) { return processes[processModule]; }
	//inline std::shared_ptr<OutputModule> getOutputModule(std::string outputModule) { return outputs[outputModule]; }

	void addInputModule(InputModule* inputModule);
	void addOutputModule(OutputModule* outputModule);
	void addProcessingModule(ProcessingModule* processingModule);

	static bool toRun;
};

#endif