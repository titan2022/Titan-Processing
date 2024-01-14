#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <opencv2/core/core.hpp>
#include "Constants.h"

using std::vector;
using std::pair;
using cv::Mat;

class Input
{
public:
	inline Input(const vector<ImageType>& matrixList)
	{
		for (auto imgType : matrixList)
		{
			inputMatrices.push_back(pair<ImageType, Mat*>(imgType, &(Mat())));
		}
	}
	vector<pair<ImageType, Mat*>> inputMatrices;
};

#endif