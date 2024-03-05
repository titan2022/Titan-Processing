#ifndef CONFIG_READER
#define CONFIG_READER

#include <string>
#include <unordered_map>
#include <vector>
#include <opencv2/core.hpp>

#include "../apriltag/Apriltag.hpp"
#include "Camera.hpp"

class ConfigReader
{
    public:
        // Networking
        std::string ip;
        int port;

        // Performance
        int threads;

        // Apriltag
        int quadDecimate;
        int quadSigma;
        double decodeSharpening;

        // Stuff from config.json
        std::unordered_map<int, Apriltag*> tags;
        std::vector<Camera*> cameras;

        ConfigReader(std::string path);
        ConfigReader();

    private:
        std::string configPath;
        void init(std::string path);
        void readJSONFile(std:: string path);
};

#endif