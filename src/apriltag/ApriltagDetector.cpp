#include <chrono>
#include <iostream>

#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "apriltag/ApriltagDetector.hpp"

using namespace titan;

ApriltagDetector::ApriltagDetector(cv::VideoCapture stream, bool showWindow, Config &config, Camera &cam, Localizer &localizer)
	: stream(stream), config(config), cam(cam), localizer(localizer), showWindow(showWindow)
{
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

	cv::Mat objPoints(4, 1, CV_32FC3);
	objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength / 2.f, markerLength / 2.f, 0);
	objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength / 2.f, markerLength / 2.f, 0);
	objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength / 2.f, -markerLength / 2.f, 0);
	objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength / 2.f, -markerLength / 2.f, 0);

	auto prevTS = std::chrono::steady_clock::now();
	auto postTS = prevTS;
	double dt = 1.0 / cam.fps;

	std::string window = std::string("Apriltag Detection for ") + cam.name;

	while (stream.isOpened())
	{
		// Measure delta time
		postTS = std::chrono::steady_clock::now();
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(postTS - prevTS).count() / 1000.0;
		prevTS = postTS;

		cv::Mat frame, gray, out;
		stream >> frame;

		// Split YUV into channels to get the grayscale image without extra processing
		// std::vector<cv::Mat> channels(3);
		// split(frame, channels);
		// gray = channels[0];
		// frame.copyTo(out);

		// Convert MJPEG to grayscale
		cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);

		frame.copyTo(out);

		// std::cout << 1.0 / dt << std::endl;

		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

		detector.detectMarkers(gray, markerCorners, ids, rejectedCandidates);

		std::vector<Localizer::AddApriltag> apriltags;

		if (ids.size() > 0)
		{
            if (this->showWindow) {
			    cv::aruco::drawDetectedMarkers(out, markerCorners, ids);
            }

			int nMarkers = markerCorners.size();
			std::vector<cv::Vec3d> rVecs(nMarkers), tVecs(nMarkers);

			for (int i = 0; i < nMarkers; i++)
			{
				cv::solvePnP(objPoints, markerCorners.at(i), cam.cameraMat, cam.distCoeffs, rVecs.at(i), tVecs.at(i),
							 false, cv::SOLVEPNP_IPPE_SQUARE); // SOLVEPNP_P3P
			}

			for (int i = 0; i < tVecs.size(); ++i)
			{
                if (!config.tags.count(ids[i])) {
                    continue;
                }

				cv::Vec3d rVec = rVecs[i];
				cv::Vec3d tVec = tVecs[i];

				apriltags.push_back({ids[i], cam, tVec, rVec, markerLength});

				// printf is thread-safe, cout is not
				// printf("[ApriltagDetector] %s can see apriltag %d\n", cam.name.c_str(), ids[i]);

				if (this->showWindow)
				{
					cv::drawFrameAxes(out, cam.cameraMat, cam.distCoeffs, rVec, tVec, 0.1);
				}
			}
		}

		localizer.submitStepCommand({apriltags, postTS});
		if (this->showWindow)
		{
			cv::imshow(window, out);

			if (cv::waitKey(1) == 27)
			{ // ESC key
				break;
			}
		}
	}
}