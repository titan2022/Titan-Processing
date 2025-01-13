#ifndef VIDEO_STREAM
#define VIDEO_STREAM

#include <opencv2/core.hpp>

namespace titan
{
class VideoStream
{
  public:
	virtual int initStream() = 0;
	virtual cv::Mat getNextFrame() = 0;
	virtual bool isOpened() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getFPS() = 0;
};
} // namespace titan

#endif