#ifndef CONFIG
#define CONFIG

#include <array>
#include <map>
#include <opencv2/core.hpp>
#include <string>
#include <string_view>

#include "apriltag/Apriltag.hpp"
#include "util/Camera.hpp"

namespace titan
{
constexpr auto CONFIG_PATH = "./config/config.json";
constexpr auto TAGS_PATH = "./config/apriltags2025.json";

/**
 * @brief Interface for apriltags and config.json, contains camera and tag information. See docs for config structure
 *
 */
class Config
{
  public:
	// Networking
	std::string ip;
	std::string dashboardIp;
	int port;

	// Performance
	int threads;

	// Apriltag
	int quadDecimate;
	int quadSigma;
	double decodeSharpening;

	// Field
	double fieldLength;
	double fieldWidth;

	// Robot
	std::array<double, 3> robotSize;

	// Stuff from config.json
	std::map<int, Apriltag> tags;

	std::vector<std::string> debugCameras;
	std::vector<std::string> prodCameras;
	std::map<std::string, Camera> cameras;

	/**
	 * @brief Reads the main config from `configPath` and the tags from `tagsPath`.
	 */
	Config(std::string_view configPath, std::string_view tagsPath);

	/**
	 * @brief Writes the main config to `mainConfigPath` and the tags to `tagPath`.
	 * @return int error code
	 */
	int write(std::string_view mainConfigPath, std::string_view tagPath);
};
} // namespace titan

#endif
