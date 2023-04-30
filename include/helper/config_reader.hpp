#ifndef CONFIG_READER
#define CONFIG_READER

#include <string>

class ConfigReader
{
    public:
        // Networking
        std::string ip;
        uint16_t port;

        // RealSense
        uint8_t hueShift;
        uint8_t satShift;
        uint8_t valShift;

        ConfigReader(std::string path);
        ConfigReader();

    private:
        std::string configPath;
        void init(std::string path);
};

#endif