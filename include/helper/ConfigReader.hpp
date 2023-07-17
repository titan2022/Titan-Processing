#ifndef CONFIG_READER
#define CONFIG_READER

#include <string>

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

        // RealSense
        uint8_t hueShift;
        uint8_t satShift;
        uint8_t valShift;

        // Apriltag
        uint8_t quadDecimate;
        uint8_t quadSigma;
        double decodeSharpening;

        ConfigReader(std::string path);
        ConfigReader();

    private:
        std::string configPath;
        void init(std::string path);
};

#endif