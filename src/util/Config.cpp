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
	this->robotSize = configData["robotSize"];

	this->debugCameras = configData["debugCameras"];
	this->prodCameras = configData["prodCameras"];

	for (auto &tagObj : configData["cameras"])
	{
		Camera cam = Camera::fromJson(tagObj);
		this->cameras.insert({cam.name, cam});
	}

	auto field = tagData["field"];
	this->fieldLength = field["length"];
	this->fieldWidth = field["width"];

	for (auto &tagObj : tagData["tags"])
	{
		json pose = tagObj["pose"];
		auto trans = pose["translation"];
		std::vector<double> posArr = {trans["x"], trans["y"], trans["z"]};
		double size = 0.1651;
		int id = tagObj["ID"];

		Translation pos = Translation::fromWPILibPosition(trans["x"], trans["y"], trans["z"], this->fieldLength, this->fieldWidth);

		auto quat = pose["rotation"]["quaternion"];
		EulerAngles rot = 
			RotationQuaternion(quat["W"], quat["X"], quat["Y"], quat["Z"], CoordinateSystem::WPILIB)
			.convertToCoordinateSystem(CoordinateSystem::THREEJS)
			.toEulerAngles();

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
		json_cams.push_back(cam.toJson());
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
		{"robotSize", robotSize},
	};
	outConfig << configData;

	std::vector<json> json_tags = {};
	for (auto tag_pair : tags)
	{
		auto tag = tag_pair.second;
		RotationQuaternion q = tag.rotation.toRotationQuaternion();
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
						   {"W", q.w},
						   {"X", q.x},
						   {"Y", q.y},
						   {"Z", q.z},
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
	outTag << tagData;

	return 0;
}
