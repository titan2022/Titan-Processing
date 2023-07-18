#include <fstream>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "helper/ConfigReader.hpp"


ConfigReader::ConfigReader(std::string path)
{
    this->init(path);
}

ConfigReader::ConfigReader()
{
    this->init("processing.cfg");
}

// Hash and operator overloading for easy switch statements
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
    this->configPath = path;
    readConfigFile(path + "/processing.cfg");
    readTagsFile(path + "/apriltags.json");
}

void ConfigReader::readConfigFile(std::string path) {
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
            case "threads"_:
                this->threads = std::stoi(value);
                break;
            case "width"_:
                this->width = std::stoi(value);
                break;
            case "height"_:
                this->height = std::stoi(value);
                break;
            case "fps"_:
                this->fps = std::stoi(value);
                break;
            case "focal_x"_:
                this->focalX = std::stod(value);
                break;
            case "focal_y"_:
                this->focalY = std::stod(value);
                break;
            case "center_x"_:
                this->centerX = std::stod(value);
                break;
            case "center_y"_:
                this->centerY = std::stod(value);
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
            case "quad_decimate"_:
                this->quadDecimate = std::stoi(value);
                break;
            case "quad_sigma"_:
                this->quadSigma = std::stoi(value);
                break;
            case "decode_sharpening"_:
                this->decodeSharpening = std::stod(value);
                break;
        }
    }
}

void ConfigReader::readTagsFile(std::string path) {
    std::ifstream in(path);
    json data = json::parse(in);

    for (auto &tagObj : data) {
        int id = tagObj["id"];
        std::vector<double> posArr = tagObj["position"];
        std::vector<double>  rotArr = tagObj["rotation"];

        Vector3D pos(posArr);
        Vector3D rot(rotArr);

        Apriltag tag(id, pos, rot);
        this->tags.push_back(tag);
    }
}