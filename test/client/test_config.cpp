#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "../../include/util/ConfigReader.hpp"

namespace fs = std::filesystem;
using namespace titan;


TEST(ConfigTest, IO) {
    fs::path configFolderPath = fs::current_path().parent_path() / "example";
    ConfigReader config;
    int error = config.readFromFile(configFolderPath.string());

    ASSERT_NE(error, 2) << "IO Error 2: Config file not found inside " << configFolderPath.string();
    ASSERT_EQ(error, 0) << "IO Error " << std::to_string(error) << ": Unknown error with loading config file.";

    ASSERT_EQ(config.ip, "127.0.0.1") << "Config Error: incorrect IP parsing.";
    ASSERT_EQ(config.dashboardIp, "127.0.0.1") << "Config Error: incorrect IP parsing.";

    // TODO: test the rest of the members
}