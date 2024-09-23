#ifndef CONFIG_READER
#define CONFIG_READER

#include <opencv2/core.hpp>
#include <string>
#include <unordered_map>
#include <vector>


#include "../apriltag/Apriltag.hpp"
#include "Camera.hpp"

namespace titan
{
/**
 * @brief Interface for config.json, contains camera and tag information. See docs for config structure
 *
 */
class ConfigReader
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

	// Stuff from config.json
	std::unordered_map<int, Apriltag *> tags;
	std::vector<Camera> cameras;

	/**
	 * @brief Reads config.json inside specified directory
	 *
	 * @param path Path for directory
	 * @return int Error code
	 */
	int readFromFile(std::string path);

  private:
	std::string configPath;
};
} // namespace titan

#endif