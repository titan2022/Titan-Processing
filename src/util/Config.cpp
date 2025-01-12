#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "apriltag/Apriltag.hpp"
#include "util/Camera.hpp"
#include "util/Config.hpp"
#include "util/Unit.hpp"
#include "util/Vector3D.hpp"

using namespace titan;
namespace fs = std::filesystem;
using json = nlohmann::json;

Config::Config(std::string_view configPath, std::string_view tagsPath)
{
	std::ifstream inConfig((fs::path(configPath)));
	std::ifstream inTag((fs::path(tagsPath)));

	if (!inConfig)
	{
		throw std::runtime_error(std::vformat("Error opening {}", std::make_format_args(configPath)));
	}
	if (!inTag)
	{
		throw std::runtime_error(std::vformat("Error opening {}", std::make_format_args(tagsPath)));
	}

	json configData = json::parse(inConfig);
	json tagData = json::parse(inTag);

	this->ip = configData["ip"];
	this->dashboardIp = configData["dashboardIp"];
	this->port = configData["port"];
	this->threads = configData["threads"];
	this->quadDecimate = configData["quadDecimate"];
	this->quadSigma = configData["quadSigma"];
	this->decodeSharpening = configData["decodeSharpening"];

	for (auto &tagObj : configData["cameras"])
	{
		auto focalX = tagObj["focalX"];
		auto focalY = tagObj["focalY"];
		auto centerX = tagObj["centerX"];
		auto centerY = tagObj["centerY"];

		std::vector<double> posArr = tagObj["position"];
		std::vector<double> rotArr = tagObj["rotation"];
		Vector3D pos(posArr);
		Vector3D rot(rotArr);
		rot *= Unit::DEG;

		auto cameraMat = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
		cameraMat.at<double>(0, 0) = focalX;
		cameraMat.at<double>(1, 1) = focalY;
		cameraMat.at<double>(0, 2) = centerX;
		cameraMat.at<double>(1, 2) = centerY;
		cameraMat.at<double>(2, 2) = 1;

		auto distCoeffs = cv::Mat(5, 1, CV_64FC1, cv::Scalar::all(0));
		distCoeffs.at<double>(0, 0) = tagObj["k1"];
		distCoeffs.at<double>(0, 1) = tagObj["k2"];
		distCoeffs.at<double>(0, 2) = tagObj["p1"];
		distCoeffs.at<double>(0, 3) = tagObj["p2"];
		distCoeffs.at<double>(0, 4) = tagObj["k3"];

		Camera cam = {tagObj["name"], tagObj["usbName"],  pos,	  rot,	  tagObj["width"], tagObj["height"],
					  tagObj["fps"],  tagObj["exposure"], focalX, focalY, centerX,		   centerY,
					  cameraMat,	  distCoeffs};

		this->cameras.insert({cam.name, cam});
	}

	for (auto &tagObj : tagData["tags"])
	{
		json pose = tagObj["pose"];
		auto trans = pose["translation"];
		std::vector<double> posArr = {trans["x"], trans["y"], trans["z"]};
		double size = 0.1651;
		int id = tagObj["ID"];

		Vector3D pos(posArr);

		auto quat = pose["rotation"]["quaternion"];
		Vector3D rot = Vector3D::fromQuaternion(quat["W"], quat["X"], quat["Y"], quat["Z"]);

		Apriltag tag(id, pos, rot, size);
		this->tags.insert({id, tag});
	}
}

int Config::write(std::string_view configPath, std::string_view tagsPath)
{

	fs::path fileConfigPathObj((fs::path(configPath)));
	fs::path fileTagPathObj((fs::path(tagsPath)));
	std::ofstream outConfig(fileConfigPathObj);
	std::ofstream outTag(fileTagPathObj);
	if (!outConfig || !outTag)
	{
		return 5;
	}

	std::vector<json> json_cams = {};
	for (auto cam_tuple : cameras)
	{
		Camera cam = get<1>(cam_tuple);
		json json_cam = {
			{"name", cam.name},
			{"usbName", cam.usbName},
			{"width", cam.width},
			{"height", cam.height},
			{"fps", cam.fps},
			{"exposure", cam.exposure},
			{"focalX", cam.focalX},
			{"focalY", cam.focalY},
			{"centerX", cam.centerX},
			{"centerY", cam.centerY},
			{"position", {cam.position.getX(), cam.position.getY(), cam.position.getZ()}},
			{"rotation",
			 {cam.rotation.getX() * Unit::RAD, cam.position.getY() * Unit::RAD, cam.position.getZ() * Unit::RAD}}};
		json_cams.push_back(json_cam);
	}

	json configData = {
		{"ip", ip},
		{"dashboardIp", dashboardIp},
		{"port", port},
		{"threads", threads},
		{"quadDecimate", quadDecimate},
		{"quadSigma", quadSigma},
		{"decodeSharpening", decodeSharpening},
		{"cameras", json_cams},
	};

	std::vector<json> json_tags = {};
	for (auto tag_pair : tags)
	{
		auto tag = tag_pair.second;
		auto q = tag.rotation.toQuaternion();
		auto x = std::get<3>(q);
		json json_tag = {
			{"ID", tag.id},
			{"pose",
			 {
				 {"translation",
				  {
					  {"x", tag.position.getX()},
					  {"y", tag.position.getY()},
					  {"z", tag.position.getZ()},
				  }},
				 {"rotation",
				  {
					  {"quaternion",
					   {
						   {"w", std::get<0>(q)},
						   {"x", std::get<1>(q)},
						   {"y", std::get<2>(q)},
						   {"z", std::get<3>(q)},
					   }},
				  }},
			 }},

		};
		json_tags.push_back(json_tag);
	}
	json tagData = {
		{"tags", json_tags},
		{"field",
		 {
			 {"length", fieldLength},
			 {"width", fieldWidth},
		 }},
	};

	return 0;
}
