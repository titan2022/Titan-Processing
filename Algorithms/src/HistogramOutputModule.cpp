
#include "HistogramOutputModule.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

const vector<ImageType> HistogramOutputModule::imageTypes = { ImageType::COLOR, ImageType::MASK };

HistogramOutputModule::HistogramOutputModule(const std::string& name, std::vector<int> channels, std::vector<int> numOfBins, std::vector<float> rangeBounds, const std::string& fileName)
	: OutputModule(name, imageTypes), dim(channels.size()), channels(channels), numOfBins(numOfBins), fileName(fileName)
{
	//auto histOutput = std::make_shared<HistogramOutputModule>(name, name);

	//outputModules =
	//{
	//	std::pair<bool, std::shared_ptr<OutputModule>>(false, histOutput)
	//};

	//this->rangeBounds = new const float*[dim]();

	float** tempArray = new float*[(rangeBounds.size() / 2)];
	rangeLimits = tempArray;

	for (int i = 0; i < rangeBounds.size(); i += 2)
	{
		float* parameterBound = new float[2] { rangeBounds[i], rangeBounds[i+1]};
		rangeLimits[i / 2] = parameterBound;
	}

	histogram = Mat(dim, numOfBins.data(), CV_32F, cv::Scalar(0));
	//std::cout << rangeLimits[0][0] << ' ' << rangeLimits[0][1] << '\n';
	//std::cout << rangeLimits[1][0] << ' ' << rangeLimits[1][1] << '\n';
}

void HistogramOutputModule::execute()
{
	//float test1[2] = { 1, 2 };
	//const float* test[]	
	//std::cout << inputMatrices[0]->type() << '\n';
	Mat subMatrix = (*(inputMatrices[0].second))(cv::Range(200, 440), cv::Range(120, 360));
	Mat subMask = (*(inputMatrices[1].second))(cv::Range(200, 440), cv::Range(120, 360));
	cv::imshow("submatrix", subMatrix);
	cv::imshow("submask", subMask);
	cv::waitKey(1);
	cv::cvtColor(subMatrix, subMatrix, cv::COLOR_BGR2HLS);
	cv::calcHist(&subMatrix, 1, channels.data(), subMask, histogram, dim, numOfBins.data(), (const float**)rangeLimits, true, true);
}

void HistogramOutputModule::finalize()
{
	cv::normalize(histogram, histogram, 0, 1, cv::NORM_MINMAX, -1, Mat());
	cv::FileStorage file(fileName, cv::FileStorage::WRITE);
	file << name << histogram;
	file.release();
}