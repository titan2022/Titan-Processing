#include "util/Camera.hpp"
#include "util/Config.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/aruco_board.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>
#include <opencv2/objdetect/charuco_detector.hpp>
#include <opencv2/opencv.hpp>
#include <utility>
#include <vector>

using namespace titan;

constexpr const double SQUARE_LENGTH = 0.0254 * 1.5;
constexpr const double MARKER_LENGTH = SQUARE_LENGTH * 0.6;
constexpr const int SPACE_KEY = 0x20;
constexpr const int ESC_KEY = 0x1b;

void showImage(cv::Mat image)
{
	cv::imshow("Camera Calibration", image);
}

/**
 * config file required (the path is hard-coded as seen below).
 */
auto main(int argc, char const *argv[]) -> int
{
	if (argc != 2)
	{
		std::cout << "1 argument was expected, " << argc - 1 << " were passed.\n";
		return 1;
	}

	const auto *cam_name = argv[1];
	Config cfg(CONFIG_PATH, TAGS_PATH);

	Camera cam;
	try
	{
		cam = cfg.cameras.at(cam_name);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Camera \"" << cam_name << "\" not found in " << CONFIG_PATH << "\n";
		std::cerr << "Known cameras are:\n";
		for (auto cam : cfg.cameras)
		{
			std::cerr << "    " << cam.first << "\n";
		}
		return 1;
	}
	cv::VideoCapture stream = cam.openStream();
	cv::Size imageSize(stream.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH),
					   stream.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT));

	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_1000);
	cv::aruco::CharucoBoard board(cv::Size(5, 7), SQUARE_LENGTH, MARKER_LENGTH, dictionary);
	cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
	detectorParams.cornerRefinementMethod = cv::aruco::CornerRefineMethod::CORNER_REFINE_SUBPIX;
	detectorParams.aprilTagQuadDecimate = 1;
	detectorParams.aprilTagQuadSigma = 2;
	cv::aruco::CharucoDetector detector(board, cv::aruco::CharucoParameters(), detectorParams);

	int imageCounter = 0;

	std::vector<cv::Mat> allCorners;
	std::vector<cv::Mat> allIds;
	std::vector<std::vector<cv::Point2f>> allImgPoints;
	std::vector<std::vector<cv::Point3f>> allObjPoints;

	while (stream.isOpened())
	{
		cv::Mat image;
		stream >> image;
		int key = cv::waitKey(1);
		if (key == ESC_KEY)
		{
			break;
		}

		std::vector<std::vector<cv::Point2f>> markerCorners;

		cv::Mat gray;
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
		imageSize = gray.size();
		cv::Mat curCorners;
		cv::Mat curIds;
		detector.detectBoard(gray, curCorners, curIds);

		if (curCorners.total() < 4)
		{
			showImage(image);
			if (key == ESC_KEY)
			{ // ESC key
				break;
			}
			continue;
		}

		std::vector<cv::Point3f> curObjPoints;
		std::vector<cv::Point2f> curImgPoints;

		board.matchImagePoints(curCorners, curIds, curObjPoints, curImgPoints);

		if (curObjPoints.empty() || curImgPoints.empty())
		{
			showImage(image);
			continue;
		}

		assert(allImgPoints.size() == allObjPoints.size());

		cv::aruco::drawDetectedCornersCharuco(image, curCorners);

		if (key == SPACE_KEY)
		{
			allCorners.push_back(curCorners);
			allIds.push_back(curIds);
			allObjPoints.push_back(curObjPoints);
			allImgPoints.push_back(curImgPoints);
			imageCounter += 1;
			std::cout << "Image " << imageCounter << " added\n";
		}

		showImage(image);
	}

	assert(allObjPoints.size() > 0);

	cv::Mat cameraMat;
	cv::Mat distCoeffs;
	cv::calibrateCamera(allObjPoints, allImgPoints, imageSize, cameraMat, distCoeffs, cv::noArray(), cv::noArray());

	cam.width = stream.get(cv::CAP_PROP_FRAME_WIDTH);
	cam.height = stream.get(cv::CAP_PROP_FRAME_HEIGHT);
	cam.cameraMat = cameraMat;
	cam.distCoeffs = distCoeffs;

	cfg.cameras.at(cam.name) = cam;

	cfg.write(CONFIG_PATH, TAGS_PATH);
	std::cout << "New config written to " << CONFIG_PATH << "\n";

	return 0;
}