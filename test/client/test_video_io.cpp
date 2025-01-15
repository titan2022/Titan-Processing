#include <gtest/gtest.h>
#include <opencv2/videoio.hpp>

#include "util/Config.hpp"

using namespace titan;

TEST(VideoIOTest, FileVideoInput)
{
}

TEST(VideoIOTest, FileImageInput)
{
	Config config(CONFIG_PATH, TAGS_PATH);
	cv::VideoCapture stream("./test/client/util/img/%01d.png");

	ASSERT_TRUE(stream.isOpened());
	EXPECT_EQ(stream.get(cv::CAP_PROP_FRAME_WIDTH), 527) << "Error: wrong image width.";
	EXPECT_EQ(stream.get(cv::CAP_PROP_FRAME_HEIGHT), 867) << "Error: wrong image height.";
	// EXPECT_EQ(stream.get(cv::CAP_PROP_FPS), 0) << "Error: Wrong FPS (not zero).";
}