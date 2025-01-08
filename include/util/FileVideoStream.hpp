#ifndef FILE_VIDEO_STREAM
#define FILE_VIDEO_STREAM

#include "util/ConfigReader.hpp"
#include "util/VideoStream.hpp"
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

/**
 * @brief Reads video from specified file.
 * Requires `filePath` and `config` to be specified before initialization.
 */
namespace titan
{
class FileVideoStream : public VideoStream
{
  public:
	std::shared_ptr<ConfigReader> config;
	int initStream();
	cv::Mat getNextFrame();
	bool isOpened();
	int getWidth();
	int getHeight();
	int getFPS();
    
	std::string filePath = "";

  private:
	cv::VideoCapture cap;
};
} // namespace titan

#endif