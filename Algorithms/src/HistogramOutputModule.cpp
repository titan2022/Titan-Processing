
#include "HistogramOutputModule.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

const vector<ImageType> HistogramOutputModule::imageTypes = { ImageType::COLOR, ImageType::MASK };

HistogramOutputModule::HistogramOutputModule(const std::string& name, std::vector<int> channels, std::vector<int> numOfBins, std::vector<float> rangeBounds, bool loadFile, const std::string& fileName)
	: OutputModule(name, imageTypes), dim(channels.size()), channels(channels), numOfBins(numOfBins), fileName(fileName), loadFile(loadFile) 
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

	//std::cout << rangeLimits[0][0] << ' ' << rangeLimits[0][1] << '\n';
	//std::cout << rangeLimits[1][0] << ' ' << rangeLimits[1][1] << '\n';
}

void HistogramOutputModule::initialize()
{
	if (loadFile)
	{
		cv::FileStorage file(fileName, cv::FileStorage::READ);
		histogram = file[name].mat();

		file.release();

		histogram *= 0.5;
	}
	else
	{
		histogram = Mat(dim, numOfBins.data(), CV_32F, cv::Scalar(0));
	}
}

void HistogramOutputModule::execute()
{
	//float test1[2] = { 1, 2 };
	//const float* test[]	
	//std::cout << inputMatrices[0]->type() << '\n';
	//Mat subMatrix = (*(inputMatrices[0].second))(cv::Range(200, 440), cv::Range(120, 360));
	//Mat subMask = (*(inputMatrices[1].second))(cv::Range(200, 440), cv::Range(120, 360));
	//cv::imshow("submatrix", subMatrix);
	//cv::imshow("submask", subMask);
	//cv::threshold(subMask, subMask, 128, 255, cv::THRESH_BINARY);
	//cv::waitKey(1);
	Mat img;
	cv::cvtColor(*(inputMatrices[0].second), img, cv::COLOR_BGR2HLS);
	cv::calcHist(&img, 1, channels.data(), *(inputMatrices[1].second), histogram, dim, numOfBins.data(), (const float**)rangeLimits, true, true);
}

void HistogramOutputModule::finalize()
{
	cv::FileStorage file(fileName, cv::FileStorage::WRITE);
	file << name << histogram;
	file.release();
}