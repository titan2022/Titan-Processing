#include <iostream>
#include <iomanip>
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>

#include "apriltag/Apriltag.hpp"
#include "apriltag/ApriltagDetector.hpp"
#include "helper/Unit.hpp"


ApriltagDetector::ApriltagDetector(int streamId, bool showWindow, ConfigReader &config, Localizer &localizer, NetworkingClient &client) : 
config(config), localizer(localizer), streamId(streamId), showWindow(showWindow), client(client)
{
    
}

void ApriltagDetector::startStream()
{
    Camera cam = config.cameras[this->streamId];
    std::cout << cam.name << std::endl;

    std::string cameraPipeline;

    cameraPipeline =    "v4l2src device=/dev/v4l/by-id/" + cam.usbName + " ! "
                        // "videorate ! videoconvert ! videoscale !"
                        // "video/x-raw, format=BGR, width=" + std::to_string(cam.width) + ", height=" + std::to_string(cam.height) + ", pixel-aspect-ratio=1/1, framerate=" + std::to_string(cam.fps) + "/1 ! "
                        "image/jpeg, width=" + std::to_string(cam.width) + ", height=" + std::to_string(cam.height) + ", framerate=" + std::to_string(cam.fps) + "/1 ! "
                        "decodebin ! videoconvert ! appsink";
    cv::VideoCapture cap(cameraPipeline);

    this->cap = cap;
    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open video capture device" << std::endl;
        return;
    }

    std::cout << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << cap.get(cv::CAP_PROP_FPS) << std::endl;
}

void ApriltagDetector::detect()
{
    cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
    detectorParams.cornerRefinementMethod = cv::aruco::CornerRefineMethod::CORNER_REFINE_APRILTAG;
    detectorParams.aprilTagQuadDecimate = config.quadDecimate;
    detectorParams.aprilTagQuadSigma = config.quadSigma;

    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_APRILTAG_36h11);
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);

    double markerLength = 0.1651;
    cv::Mat cameraMatrix = config.cameras[this->streamId].cameraMat;
    cv::Mat distCoeffs = config.cameras[this->streamId].distCoeffs;

    cv::Mat objPoints(4, 1, CV_32FC3);
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength/2.f, -markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength/2.f, -markerLength/2.f, 0);

    auto prevTS = std::chrono::steady_clock::now();
    auto postTS = prevTS;
    double dt = 1.0 / config.cameras[this->streamId].fps;

    // Temporary...
    Vector3D visible(0, 0, 0);

    while (true)
    {
        // Measure delta time
        postTS = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::milliseconds>(postTS - prevTS).count() / 1000.0;
        prevTS = postTS;

        cv::Mat frame, gray, out;
        cap >> frame;

        // Split YUV into channels to get the grayscale image without extra processing
        std::vector<cv::Mat> channels(3);
        split(frame, channels);
        gray = channels[0];
        frame.copyTo(out);

        std::cout << 1.0 / dt << std::endl;

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

        detector.detectMarkers(gray, markerCorners, ids, rejectedCandidates);
        
        if (ids.size() > 0) {
            visible.setX(1);

            cv::aruco::drawDetectedMarkers(out, markerCorners, ids);

            int nMarkers = markerCorners.size();
            std::vector<cv::Vec3d> rVecs(nMarkers), tVecs(nMarkers);

            for (int i = 0; i < nMarkers; i++) {
                cv::solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs, rVecs.at(i), tVecs.at(i), false, cv::SOLVEPNP_IPPE_SQUARE); // SOLVEPNP_P3P
            }

            for (int i = 0; i < tVecs.size(); ++i) {
                cv::Vec3d rVec = rVecs[i];
                cv::Vec3d tVec = tVecs[i];

                localizer.addApriltag(ids[i], this->streamId, tVec, rVec, markerLength, dt);

                if (this->showWindow) {
                    cv::drawFrameAxes(out, cameraMatrix, distCoeffs, rVec, tVec, 0.1);
                }
            }
        } else {
            visible.setX(-1);
        }

        this->client.send_vector("visible", visible, false);

        localizer.step(dt);
        if (this->showWindow) {
            cv::imshow("Apriltag Debug Window", out);
            if (cv::waitKey(1) == 27) { // ESC key
                break;
            }
        }
    }
}