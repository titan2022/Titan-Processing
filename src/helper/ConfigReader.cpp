#include <fstream>
#include <sstream>
#include <string>

#include "helper/ConfigReader.hpp"

ConfigReader::ConfigReader(std::string path)
{
    this->init(path);
}

ConfigReader::ConfigReader()
{
    this->init("processing.cfg");
}

constexpr unsigned int hash(const char *s, int off = 0)
{
    return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
}

constexpr inline unsigned int operator""_(char const *p, size_t)
{
    return hash(p);
}

void ConfigReader::init(std::string path)
{
    std::ifstream in(path);

    std::string line;
    while (std::getline(in, line))
    {
        if (line.back() == '\r')
        {
            line.pop_back();
        }

        std::istringstream is_line(line);
        std::string key;
        std::string value;

        if (key[0] == '#')
        {
            continue;
        }

        if (!std::getline(is_line, key, '='))
        {
            continue;
        }

        if (!std::getline(is_line, value))
        {
            continue;
        }

        switch (hash(key.c_str(), 0))
        {
            case "ip"_:
                this->ip = value;
                break;
            case "port"_:
                this->port = std::stoi(value);
                break;
            case "hue_shift"_:
                this->hueShift = std::stoi(value);
                break;
            case "sat_shift"_:
                this->satShift = std::stoi(value);
                break;
            case "val_shift"_:
                this->valShift = std::stoi(value);
                break;
        }
    }
}