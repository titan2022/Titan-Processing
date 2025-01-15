#include "networking/Client.h"
#include "util/Config.hpp"
#include <future>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <thread>

using namespace titan;

void execServerTest(std::string cmd, std::promise<std::string> execPromise)
{
	std::array<char, 128> buffer{};
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(("timeout 5 " + cmd).c_str(), "r"), pclose);
	if (!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}

	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}

	execPromise.set_value(result);
}

TEST(ClientTest, ServerClientAllTypes)
{
	// std::cout << "bruh" << std::endl;
	Config config(CONFIG_PATH, TAGS_PATH);

	NetworkingClient client(config.ip, config.port);

	std::promise<std::string> execPromise;
	auto execFuture = execPromise.get_future();
	std::thread serverTestThread(&execServerTest, "java -cp ./bin/test/ServerTest.jar ServerTest",
								 std::move(execPromise));
	serverTestThread.join();
	std::string result = execFuture.get();

	ASSERT_THAT(result, ::testing::HasSubstr("test passed")) << "Unknown C++ UDP socket error.";
}

// TEST(ClientTest, PythonServerClientAllTypes) {
//     fs::path configFolderPath = fs::current_path().parent_path() / "example";
//     ConfigReader config;

//     NetworkingClient client(config.ip, config.port);

//     std::promise<std::string> execPromise;
//     auto execFuture = execPromise.get_future();
//     std::thread serverTestThread(&execServerTest, "python3 ", std::move(execPromise));
//     serverTestThread.join();
//     std::string result = execFuture.get();

//     ASSERT_THAT(result, ::testing::HasSubstr("test passed")) << "Unknown Python UDP socket error.";
// }