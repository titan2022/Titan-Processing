#include <iostream>
#include <string>

#include "../../include/util/ConfigReader.hpp"

int main(int argc, char const *argv[])
{
    ConfigReader config;
    config.readFromFile("example/processing.cfg");

    std::cout << config.ip;
    std::cout << ":" << std::to_string(config.port) << std::endl;

    return 0;
}