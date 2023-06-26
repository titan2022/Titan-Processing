#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include <vector>
#include "ProcessingModule.h"

/// <summary>
/// The container of ProcessingFrames which distributes cv::Mats to
/// </summary>
class InputModule : public Module
{
protected:
	std::vector<std::string> capacities;
	std::unordered_map<std::string, cv::Mat> sourceMatrices;
	//Subscribers that the input module will push data to
	std::vector<std::shared_ptr<ProcessingModule>> processModules;
public:
	bool checkModuleIsValid(std::shared_ptr<ProcessingModule> module) const;
	void pushData() const;
	inline const std::vector<std::string>& getCapacities() { return capacities; }
	void registerProcessingModule(std::shared_ptr<ProcessingModule> process);
	//Checks if able to get frames properly
	virtual bool isValid() const = 0;
};

#endif