#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "util/Config.hpp"

namespace fs = std::filesystem;
using namespace titan;

TEST(ConfigTest, IO)
{
	Config config((fs::current_path().parent_path() / CONFIG_PATH).string(),
				  (fs::current_path().parent_path() / TAGS_PATH).string());

	ASSERT_EQ(config.ip, "127.0.0.1") << "Config Error: incorrect IP parsing.";
	ASSERT_EQ(config.dashboardIp, "127.0.0.1") << "Config Error: incorrect IP parsing.";

	// TODO: test the rest of the members
}
