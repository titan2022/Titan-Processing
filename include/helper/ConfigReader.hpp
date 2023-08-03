#ifndef CONFIG_READER
#define CONFIG_READER

#include <string>
#include <vector>
#include <opencv2/core.hpp>

#include "../apriltag/Apriltag.hpp"

class ConfigReader
{
    public:
        // Networking
        std::string ip;
        uint16_t port;

        // Performance
        uint8_t threads;

        // Camera
        uint16_t width;
        uint16_t height;
        uint8_t fps;
        double focalX;
        double focalY;
        double centerX;
        double centerY;
        cv::Mat cameraMat;
        cv::Mat distCoeffs;

        // RealSense
        uint8_t hueShift;
        uint8_t satShift;
        uint8_t valShift;

        // Apriltag
        uint8_t quadDecimate;
        uint8_t quadSigma;
        double decodeSharpening;

        // Tags from apriltags.json
        std::vector<Apriltag> tags;

        ConfigReader(std::string path);
        ConfigReader();

    private:
        std::string configPath;
        void init(std::string path);
        void readConfigFile(std:: string path);
        void readTagsFile(std:: string path);
};

#endif