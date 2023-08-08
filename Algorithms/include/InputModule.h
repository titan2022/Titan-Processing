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
	std::vector<std::shared_ptr<ProcessingModule>> processingModules;
	std::unordered_map<InputType, cv::Mat> sourceMatrices;
	////Subscribers that the input module will push data to
	//std::vector<std::shared_ptr<ProcessingModule>> processModules;
public:
	InputModule(const std::string& name);
	virtual void addProcessingModule(const std::shared_ptr<ProcessingModule>& process);
	//Checks if able to get frames properly
	virtual bool isValid() const = 0;

	//inline const std::vector<std::shared_ptr<ProcessingModule>>& getProcessingModules() const { return processModules; }
};

#endif