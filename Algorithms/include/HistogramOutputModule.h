#ifndef HISTOGRAM_OUTPUT_MODULE_H
#define HISTOGRAM_OUTPUT_MODULE_H

#include "OutputModule.h"
#include <opencv2/core/persistence.hpp>

class HistogramOutputModule : public OutputModule
{
public:
	HistogramOutputModule(const std::string& name, std::vector<int> channels, std::vector<int> numOfBins, std::vector<float> rangeBounds, bool loadFIle, const std::string& fileName);
	void initialize() override;
	void execute() override;
	void finalize() override;

protected:
	static const vector<ImageType> imageTypes;
	cv::Mat histogram;
	int dim;
	std::vector<int> channels;
	std::vector<int> numOfBins;
	float** rangeLimits;
	std::string fileName;
	bool loadFile;
};

#endif