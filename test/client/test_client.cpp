#include <gmock/gmock.h>
#include "networking/Client.h"
#include "util/ConfigReader.hpp"
#include <future>
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <thread>

namespace fs = std::filesystem;


void execServerTest(std::promise<std::string> &&execPromise) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("timeout 5 java -cp ../bin/test/ServerTest.jar ServerTest", "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    execPromise.set_value(result);
}

TEST(ConfigTest, IOTest) {
    fs::path configFolderPath = fs::current_path().parent_path() / "example";
    ConfigReader config;

    NetworkingClient client(config.ip, config.port);
    
    std::promise<std::string> execPromise;
    auto execFuture = execPromise.get_future();
    std::thread serverTestThread(&execServerTest, std::move(execPromise));
    serverTestThread.join();
    std::string result = execFuture.get();

    ASSERT_THAT(result, ::testing::HasSubstr("test passed")) << "Unknwon UDP socket error.";
}