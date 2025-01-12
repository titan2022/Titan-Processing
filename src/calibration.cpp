#include "util/ConfigReader.hpp"
#include "util/CameraVideoStream.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/charuco_detector.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace titan;

inline constexpr double SQUARE_LENGTH = 0.0254 * 1.5;
inline constexpr double MARKER_LENGTH = SQUARE_LENGTH * 0.6;


void showImage(cv::Mat image) {
    cv::imshow("Camera Calibration", image);
}

/**
 * Translation of the Python ChArUco calibration script from Titan Processing
 * https://github.com/titan2022/Titan-Processing/blob/main/tools/charuco_calibration.py
 *
 * config file required (the path is hard-coded as seen below).
 */
int main(int argc, char const *argv[])
{
    fs::path configPath = fs::current_path().parent_path() / "config" / "config.json";
    fs::path tagsPath = fs::current_path().parent_path() / "config" / "apriltags2025.json";
    ConfigReader config;
    config.readFromFile(configPath.string(), tagsPath.string());

    CameraVideoStream stream;
    stream.config = std::make_shared<ConfigReader>(config);
    stream.cameraIndex = 0;
    stream.initStream();

    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_1000);
    cv::aruco::CharucoBoard board(cv::Size(5, 7), SQUARE_LENGTH, MARKER_LENGTH, dictionary);
    cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
	detectorParams.cornerRefinementMethod = cv::aruco::CornerRefineMethod::CORNER_REFINE_SUBPIX;
	detectorParams.aprilTagQuadDecimate = 1;
	detectorParams.aprilTagQuadSigma = 2;
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);

    int imageCounter = 0;
    cv::Mat cameraMatrix, distCoeffs;
    cv::Size imageSize;

    std::vector<std::vector<cv::Point2f>> cornersList;
    std::vector<std::vector<int>> idList;

    while (stream.isOpened()) {
        cv::Mat image = stream.getNextFrame();
        int key = cv::waitKey(1);

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        imageSize = gray.size();
        detector.detectMarkers(gray, markerCorners, ids, rejectedCandidates);

        if (markerCorners.size() == 0) {
            showImage(image);
            if (key == 27) { // ESC key
                break;
            }
            continue;
        }

        std::vector<cv::Point2f> charucoCorners;
        std::vector<int> charucoIds;

        cv::aruco::interpolateCornersCharuco(markerCorners, ids, image, &board, charucoCorners, charucoIds, cameraMatrix, distCoeffs);

        if (charucoIds.size() == 0) {
            showImage(image);
            if (key == 27) { // ESC key
                break;
            }
            continue;
        }

        cv::aruco::drawDetectedCornersCharuco(image, charucoCorners);

        if (key == 32) { // Space key
            if (charucoIds.size() > 20) {
                cornersList.push_back(charucoCorners);
                idList.push_back(charucoIds);
                imageCounter++;
                std::cout << "Image added: " << imageCounter << std::endl;
            }
        } else if (key == 27) {
            break;
        }

        showImage(image);
    }

    std::vector<cv::Mat> rvecs, tvecs;
    double repError = cv::aruco::calibrateCameraCharuco(cornersList, idList, &board, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);

    json data;
    data["cameras"] = json::array({{
        {"width", stream.getWidth()},
        {"height", stream.getHeight()},
        {"focalX", cameraMatrix.at<float>(0, 0)},
        {"focalY", cameraMatrix.at<float>(1, 1)},
        {"centerX", cameraMatrix.at<float>(0, 2)},
        {"centerY", cameraMatrix.at<float>(1, 2)},
        {"k1", distCoeffs.at<float>(0, 0)},
        {"k2", distCoeffs.at<float>(0, 1)},
        {"p1", distCoeffs.at<float>(0, 2)},
        {"p2", distCoeffs.at<float>(0, 3)},
        {"k3", distCoeffs.at<float>(0, 4)},
    }});

    std::ofstream file(configPath.string());
    file << data;

	return 0;
}