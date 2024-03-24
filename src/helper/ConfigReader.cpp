#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "helper/ConfigReader.hpp"
#include "helper/Unit.hpp"

ConfigReader::ConfigReader(std::string path)
{
    this->init(path);
}

ConfigReader::ConfigReader()
{
    this->init("processing.cfg");
}

void ConfigReader::init(std::string path)
{
    this->configPath = path;
    readJSONFile(path + "/config.json");
}

void ConfigReader::readJSONFile(std::string path)
{
    std::ifstream in(path);
    json data = json::parse(in);

    this->ip = data["ip"];
    this->dashboardIp = data["dashboardIp"];
    this->port = data["port"];
    this->threads = data["threads"];
    this->quadDecimate = data["quadDecimate"];
    this->quadSigma = data["quadSigma"];
    this->decodeSharpening = data["decodeSharpening"];

    for (auto &tagObj : data["cameras"])
    {
        Camera cam;

        cam.name = tagObj["name"];
        cam.usbName = tagObj["usbName"];
        cam.width = tagObj["width"];
        cam.height = tagObj["height"];
        cam.fps = tagObj["fps"];
        cam.exposure = tagObj["exposure"];
        cam.focalX = tagObj["focalX"];
        cam.focalY = tagObj["focalY"];
        cam.centerX = tagObj["centerX"];
        cam.centerY = tagObj["centerY"];

        std::vector<double> posArr = tagObj["position"];
        std::vector<double> rotArr = tagObj["rotation"];
        Vector3D pos(posArr);
        Vector3D rot(rotArr);
        rot *= Unit::DEG;
        cam.position = pos;
        cam.rotation = rot;

        cam.cameraMat = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
        cam.cameraMat.at<double>(0, 0) = cam.focalX;
        cam.cameraMat.at<double>(1, 1) = cam.focalY;
        cam.cameraMat.at<double>(0, 2) = cam.centerX;
        cam.cameraMat.at<double>(1, 2) = cam.centerY;
        cam.cameraMat.at<double>(2, 2) = 1;

        cam.distCoeffs = cv::Mat(5, 1, CV_64FC1, cv::Scalar::all(0));
        cam.distCoeffs.at<double>(0, 0) = tagObj["k1"];
        cam.distCoeffs.at<double>(0, 1) = tagObj["k2"];
        cam.distCoeffs.at<double>(0, 2) = tagObj["p1"];
        cam.distCoeffs.at<double>(0, 3) = tagObj["p2"];
        cam.distCoeffs.at<double>(0, 4) = tagObj["k3"];

        this->cameras.push_back(cam);
    }

    for (auto &tagObj : data["apriltags"])
    {
        std::vector<double> posArr = tagObj["position"];
        std::vector<double> rotArr = tagObj["rotation"];
        double size = tagObj["size"];
        int id = tagObj["id"];

        Vector3D pos(posArr);
        Vector3D rot(rotArr);
        rot *= Unit::DEG;

        Apriltag *tag = new Apriltag(id, pos, rot, size);
        this->tags.insert({id, tag});
    }
}