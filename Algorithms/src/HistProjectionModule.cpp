#include "HistProjectionModule.h"
#include "VisionFunctions.h"
#include <opencv2/core/persistence.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

const vector<ImageType> HistProjectionModule::inputTypes = { ImageType::COLOR };
const vector<ImageType> HistProjectionModule::outputTypes = { ImageType::COLOR };

HistProjectionModule::HistProjectionModule(const std::string& name, const vector<int>& channels, const vector<float> rangeBounds, double minWeight, const std::string& fileName)
	: ProcessingModule(name, inputTypes, outputTypes), channels(channels), fileName(fileName), minWeight(minWeight)
{
	float** tempArray = new float*[(rangeBounds.size() / 2)];
	rangeLimits = tempArray;

	for (int i = 0; i < rangeBounds.size(); i += 2)
	{
		float* parameterBound = new float[2] { rangeBounds[i], rangeBounds[i + 1]};
		rangeLimits[i / 2] = parameterBound;
	}
}

void HistProjectionModule::initialize()
{
	cv::FileStorage file(fileName, cv::FileStorage::READ);
	histogram = file[name].mat();
	cv::normalize(histogram, histogram, 0, 100, cv::NORM_MINMAX, -1, Mat());
}

void HistProjectionModule::execute()
{
	Mat histOutput;
	Mat hsl;
	cv::imshow("Test color", *inputMatrices[0].second);
	cv::waitKey(1);
	cv::cvtColor(*inputMatrices[0].second, hsl, cv::COLOR_BGR2HLS);
	//cv::imshow("hsl mat", hsl);
	//cv::waitKey(1);
	cv::calcBackProject(&hsl, 1, channels.data(), histogram, histOutput, (const float**) rangeLimits);
	//std::cout << histOutput;
	//histOutput *= 255;
	cv::threshold(histOutput, histOutput, minWeight, 255, cv::THRESH_BINARY);
	cv::imshow("Hist mask", histOutput);
	cv::waitKey(1);
	//cv::bitwise_not(histOutput, histOutput);
	outputMatrices[0].second = Mat(640, 480, CV_8UC3, cv::Scalar(0, 0, 0));
	inputMatrices[0].second->copyTo(outputMatrices[0].second, histOutput);

} 