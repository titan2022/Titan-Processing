#ifndef GRAPHIC_OUTPUT_TYPE_H
#define GRAPHIC_OUTPUT_TYPE_H

#include "IOutputType.h"

class IGraphicOutputType : public IOutputType
{
public:
	inline virtual cv::Mat getData() = 0;
};

#endif