#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
#include <opencv2/core/core.hpp>
#include "Constants.h"
#include "Input.h"

using std::vector;
using std::pair;
using cv::Mat;

class Output
{
public:
	vector<pair<ImageType, Mat>> outputMatrices;

	inline Output(const vector<ImageType>& matrixList)
	{
		for (auto imgType : matrixList)
		{
			outputMatrices.push_back(pair<ImageType, Mat>(imgType, Mat()));
		}
	}
	inline void addSubscriber(Input* subscriber)
	{
		for (auto& outputPair : outputMatrices)
		{
			for (auto& inputPair : subscriber->inputMatrices)
			{
				if (inputPair.first == outputPair.first)
				{
					inputPair.second = &(outputPair.second);
				}
			}
		}
	}

};

#endif