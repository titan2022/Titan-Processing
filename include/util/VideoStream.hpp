#ifndef VIDEO_STREAM
#define VIDEO_STREAM

#include <opencv2/core.hpp>

namespace titan
{
class VideoStream
{
  public:
    int id = -1;
    virtual int initStream() { return 0; };
	virtual cv::Mat getNextFrame() { return cv::Mat(); };
};
} // namespace titan

#endif