#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <util/FileImageStream.hpp>
#include <filesystem>

namespace fs = std::filesystem;
using namespace titan;

int FileImageStream::initStream() {
    if (config == nullptr)
	{
        std::cerr << "No config in `FileImageStream` object defined." << std::endl;
        return 1;
	}

    if (folderPath.length() == 0)
	{
        std::cerr << "No folder path in `FileImageStream` object defined." << std::endl;
        return 1;
	}

    fs::path folderPathObj((fs::path(folderPath)));
    if (!fs::exists(folderPathObj))
    {
        std::cerr << "Folder path \"" << folderPath << "\" not found." << std::endl;
        return 1;
    }

    for (const auto & image : fs::directory_iterator(folderPathObj)) {
        imagePaths.push_back(image.path().string());
    }

    if (imagePaths.size() == 0)
    {
        std::cerr << "Folder path \"" << folderPath << "\" contains no images." << std::endl;
        return 1;
    }

    return 0;
}

cv::Mat FileImageStream::getNextFrame()
{
    cv::Mat frame;
    frame = cv::imread(imagePaths[imageIndex]);
    if (frame.data == NULL) {
        std::cerr << "Image file \"" << imagePaths[imageIndex] << "\" could not be loaded.";
        imageIndex = imagePaths.size(); // End stream
    } else {
        imageIndex++;
        imageWidth = frame.cols;
        imageHeight = frame.rows;
    }
    return frame;
}

bool FileImageStream::isOpened() {
    return imageIndex >= imagePaths.size() - 1;
}

int FileImageStream::getWidth() {
    return imageWidth;
}

int FileImageStream::getHeight() {
    return imageHeight;
}

int FileImageStream::getFPS() {
    return 0;
}