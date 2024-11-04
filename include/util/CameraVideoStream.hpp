#ifndef CAMERA_VIDEO_STREAM
#define CAMERA_VIDEO_STREAM

#include "util/ConfigReader.hpp"
#include "util/VideoStream.hpp"
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace titan {
class CameraVideoStream : public VideoStream {
public:
  std::shared_ptr<ConfigReader> config;
  int initStream();
  cv::Mat getNextFrame();

private:
  cv::VideoCapture cap;
};
} // namespace titan

#endif