#ifndef HIST_PROJECTION_MODULE_H
#define HIST_PROJECTION_MODULE_H

#include "ProcessingModule.h"

class HistProjectionModule : public ProcessingModule
{
public:
	HistProjectionModule(const std::string& name, const vector<int>& channels, const vector<float> rangeBounds, double minWeight, const std::string& fileName);
	void initialize() override;
	void execute() override;
private:
	Mat histogram;
	double minWeight;
	vector<int> channels;
	float** rangeLimits;
	std::string fileName;
	static const vector<ImageType> inputTypes;
	static const vector<ImageType> outputTypes;
};

#endif