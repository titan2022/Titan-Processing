#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include <vector>
#include "Constants.h"
#include "ProcessingModule.h"

/// <summary>
/// The container of ProcessingFrames which distributes cv::Mats to
/// </summary>
class InputModule : public Module
{
protected:
	std::unordered_map<Requirements, cv::Mat> sourceMatrices;
	//Subscribers that the input module will push data to
	std::vector<std::shared_ptr<ProcessingModule>> processModules;
public:
	bool checkModuleIsValid(std::shared_ptr<ProcessingModule> module) const;
	void pushData() const;
	virtual const std::vector<Requirements>& getCapacities() const = 0;
	void linkProcessingModule(std::shared_ptr<ProcessingModule> process);
	//Checks if able to get frames properly
	virtual bool isValid() const = 0;

	inline const std::vector<std::shared_ptr<ProcessingModule>>& getProcessingModules() const { return processModules; }
};

#endif