#include <iostream>
#include <string>

#include "../../include/helper/config_reader.hpp"

int main(int argc, char const *argv[])
{
    ConfigReader config("example/processing.cfg");

    std::cout << config.ip;
    std::cout << ":" << std::to_string(config.port) << std::endl;

    return 0;
}