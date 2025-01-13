#include <filesystem>
#include <gtest/gtest.h>

#include "util/ConfigReader.hpp"
#include "util/FileImageStream.hpp"

namespace fs = std::filesystem;
using namespace titan;

TEST(VideoIOTest, FileVideoInput) {
    
}

TEST(VideoIOTest, FileImageInput) {
    fs::path configFolderPath = fs::current_path().parent_path() / "config";
	fs::path configPath = configFolderPath / "config.json";
    fs::path tagsPath = configFolderPath / "apriltags2025.json";
    fs::path imageFolderPath = fs::current_path().parent_path() / "test" / "client" / "util" / "img";

    ConfigReader config;
    config.readFromFile(configPath.string(), tagsPath.string());

    FileImageStream stream;
    stream.config = std::make_shared<ConfigReader>(config);
    stream.folderPath = imageFolderPath.string();

    int error = stream.initStream();
    ASSERT_EQ(error, 0);

    ASSERT_TRUE(stream.isOpened());
    EXPECT_EQ(stream.getWidth(), 527) << "Error: wrong image width.";
    EXPECT_EQ(stream.getHeight(), 867) << "Error: wrong image height.";
    EXPECT_EQ(stream.getFPS(), 0) << "Error: Wrong FPS (not zero).";
}