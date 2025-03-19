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

	this->udp_roborio_ip = configData["udp_roborio_ip"];
	this->udp_dashboard_ip = configData["udp_dashboard_ip"];
	this->udp_port = configData["udp_port"];
	this->nt_server_ip = configData["nt_server_ip"];
	this->nt_port = configData["nt_port"];
	this->threads = configData["threads"];
	this->quadDecimate = configData["quadDecimate"];
	this->quadSigma = configData["quadSigma"];
	this->decodeSharpening = configData["decodeSharpening"];
    this->rejectDistance = configData["rejectDistance"];
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

		Translation3d pos = Translation3d{
			units::meter_t{trans["x"]},
			units::meter_t{trans["y"]},
			units::meter_t{trans["z"]}
		};

		auto quat = pose["rotation"]["quaternion"];
		Rotation3d rot = Rotation3d{
			Quaternion{quat["W"], quat["X"], quat["Y"], quat["Z"]}
		};

		Apriltag tag(id, pos, rot, size);
		this->tags.insert({id, tag});
	}
}

int Config::write(std::string_view configPath, std::string_view tagsPath)
{
	int retval_config = writeConfig(configPath);
	int retval_tags = writeTagsConverted(tagsPath, CoordinateSystems::WPILib());

	return std::max(retval_tags, retval_config);
}

int Config::writeConfig(std::string_view configPath)
{
	fs::path fileConfigPathObj((fs::path(configPath)));
	std::ofstream outConfig(fileConfigPathObj);
	if (!outConfig)
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
		{"udp_roborio_ip", udp_roborio_ip},
		{"udp_dashboard_ip", udp_dashboard_ip},
		{"udp_port", udp_port},
		{"nt_server_ip", nt_server_ip},
		{"nt_port", nt_port},
		{"threads", threads},
		{"quadDecimate", quadDecimate},
		{"quadSigma", quadSigma},
		{"decodeSharpening", decodeSharpening},
        {"rejectDistance", rejectDistance},
		{"cameras", json_cams},
		{"robotSize", robotSize},
	};
	outConfig << configData;

	return 0;
}

int Config::writeTagsConverted(std::string_view tagsPath, CoordinateSystem coordinateSystem)
{
	fs::path fileTagPathObj((fs::path(tagsPath)));
	std::ofstream outTag(fileTagPathObj);
	if (!outTag)
	{
		return 5;
	}

	std::vector<json> json_tags = {};
	for (auto tag_pair : tags)
	{
		auto tag_raw = tag_pair.second;
		Transform3d pose = CoordinateSystem::Convert(
			Transform3d{tag_raw.position, tag_raw.rotation},
			CoordinateSystems::standard(),
			coordinateSystem);
		Quaternion q = pose.Rotation().GetQuaternion();
		json json_tag = {
			{"ID", tag_raw.id},
			{"pose",
			 {
				 {"translation",
				  {
					  {"x", pose.Translation().X().value()},
					  {"y", pose.Translation().Y().value()},
					  {"z", pose.Translation().Z().value()},
				  }},
				 {"rotation",
				  {
					  {"quaternion",
					   {
						   {"W", q.W()},
						   {"X", q.X()},
						   {"Y", q.Y()},
						   {"Z", q.Z()},
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
