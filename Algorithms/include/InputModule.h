#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include <vector>
#include "ProcessingModule.h"


/// <summary>
/// The container of ProcessingFrames which distributes cv::Mats to
/// </summary>
class InputModule : public Module
{
public:
	InputModule(const std::string& name);
	virtual void addProcessingModule(std::shared_ptr<ProcessingModule>& process);
	//Checks if able to get frames properly
	virtual bool isValid() const = 0;

	virtual void execute() override;
	//inline const std::vector<std::shared_ptr<ProcessingModule>>& getProcessingModules() const { return processModules; }
protected:
	std::vector<std::shared_ptr<ProcessingModule>> processingModules;
	std::vector<std::vector<cv::Mat*>> matrixSubscribers;
	std::vector<std::pair<InputType, cv::Mat>> sourceMatrices;
	////Subscribers that the input module will push data to
	void createMatrixSubscribers();

};

#endif