#include "FileInputModule.h"
#include "VisionProcessor.h"
#include <opencv2/imgcodecs.hpp>

const vector<ImageType> FileInputModule::imgTypes = { ImageType::COLOR };

FileInputModule::FileInputModule(const string& name, const string& path)
	: InputModule(name, imgTypes), dirIterator(path), index(0) {}


void FileInputModule::initialize()
{
	for (const auto& path : dirIterator)
	{
		filePaths.push_back(path.path().string());
	}
}

void FileInputModule::execute()
{
	if (index < filePaths.size())
	{
		outputMatrices[0].second = cv::imread(filePaths[index]);
		++index;
	}
	else
	{
		VisionProcessor::toRun = false;
	}
}

bool FileInputModule::isValid() const
{
	return true;
}