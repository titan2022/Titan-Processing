#include <iostream>
#include <iomanip>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>

#include "apriltag/Apriltag.hpp"
#include "apriltag/ApriltagDetector.hpp"
#include "helper/Unit.hpp"


ApriltagDetector::ApriltagDetector(int streamId, bool showWindow, ConfigReader &config, Localizer &localizer) : 
config(config), localizer(localizer), streamId(streamId), showWindow(showWindow)
{
    
}

void ApriltagDetector::startStream()
{
    cv::VideoCapture cap(this->streamId);
    this->cap = cap;
    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open video capture device" << std::endl;
        return;
    }

    cap.set(cv::CAP_PROP_FPS, config.fps);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, config.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.height);
}

void ApriltagDetector::detect()
{
    cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_APRILTAG_16h5);
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);

    double markerLength = 6 * Unit::INCH;
    cv::Mat cameraMatrix = config.cameraMat;
    cv::Mat distCoeffs = config.distCoeffs;

    cv::Mat objPoints(4, 1, CV_32FC3);
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength/2.f, -markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength/2.f, -markerLength/2.f, 0);

    while (true)
    {
        cv::Mat frame, gray, out;
        cap >> frame;

        // Split YUV into channels to get the grayscale image without extra processing
        std::vector<cv::Mat> channels(3);
        split(frame, channels);
        gray = channels[0];
        frame.copyTo(out);

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

        detector.detectMarkers(frame, markerCorners, ids, rejectedCandidates);
        
        if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(out, markerCorners, ids);

            int nMarkers = markerCorners.size();
            std::vector<cv::Vec3d> rVecs(nMarkers), tVecs(nMarkers);

            for (int i = 0; i < nMarkers; i++) {
                cv::solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs, rVecs.at(i), tVecs.at(i), false, cv::SOLVEPNP_IPPE_SQUARE);
            }

            for (int i = 0; i < tVecs.size(); ++i) {
                cv::Vec3d rVec = rVecs[i];
                cv::Vec3d tVec = tVecs[i];

                localizer.addApriltag(ids[i], tVec, rVec);

                cv::drawFrameAxes(out, cameraMatrix, distCoeffs, rVec, tVec, 0.1);
            }
        }

        if (this->showWindow) {
            cv::imshow("Apriltag Debug Window", out);
            if (cv::waitKey(1) == 27) { // ESC key
                break;
            }
        }
    }
}