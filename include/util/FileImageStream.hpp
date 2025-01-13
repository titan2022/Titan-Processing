#ifndef FILE_IMAGE_STREAM
#define FILE_IMAGE_STREAM

#include "util/ConfigReader.hpp"
#include "util/VideoStream.hpp"
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

/**
 * @brief Reads images from specified folder.
 * Requires `folderPath` and `config` to be specified before initialization.
 */
namespace titan
{
class FileImageStream : public VideoStream
{
  public:
	std::shared_ptr<ConfigReader> config;
	int initStream();
	cv::Mat getNextFrame();
	bool isOpened();
	int getWidth();
	int getHeight();
	int getFPS();
    
    // Path to folder with images.
	std::string folderPath = "";

  private:
    std::vector<std::string> imagePaths;
    int imageIndex = 0;
    int imageWidth = 0;
    int imageHeight = 0;
};
} // namespace titan

#endif