#include <chrono>

#include <cstddef>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "apriltag/ApriltagDetector.hpp"

using namespace titan;

ApriltagDetector::ApriltagDetector(cv::VideoCapture stream, bool showWindow, const Config &config, Camera &cam,
								   Localizer &localizer)
	: stream(stream), cam(cam), showWindow(showWindow), quadDecimate(config.quadDecimate), quadSigma(config.quadSigma),
	  localizer(localizer)
{
}

void ApriltagDetector::detect()
{
	cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
	detectorParams.cornerRefinementMethod = cv::aruco::CornerRefineMethod::CORNER_REFINE_APRILTAG;
	detectorParams.aprilTagQuadDecimate = quadDecimate;
	detectorParams.aprilTagQuadSigma = quadSigma;

	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_APRILTAG_36h11);
	cv::aruco::ArucoDetector detector(dictionary, detectorParams);

	double markerLength = 0.1651;

	cv::Mat objPoints(4, 1, CV_64FC3);
	objPoints.at<cv::Vec3d>(0, 0) = cv::Vec3d(-markerLength / 2, markerLength / 2, 0);
	objPoints.at<cv::Vec3d>(0, 1) = cv::Vec3d(markerLength / 2, markerLength / 2, 0);
	objPoints.at<cv::Vec3d>(0, 2) = cv::Vec3d(markerLength / 2, -markerLength / 2, 0);
	objPoints.at<cv::Vec3d>(0, 3) = cv::Vec3d(-markerLength / 2, -markerLength / 2, 0);

	auto prevTS = std::chrono::steady_clock::now();
	auto postTS = prevTS;
	double dt = 1.0 / cam.fps;

	while (stream.isOpened())
	{
		// Measure delta time
		postTS = std::chrono::steady_clock::now();
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(postTS - prevTS).count() / 1000.0;
		prevTS = postTS;

		cv::Mat frame;
		cv::Mat gray;
		cv::Mat out;
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
		std::vector<std::vector<cv::Point2f>> markerCorners;
		std::vector<std::vector<cv::Point2f>> rejectedCandidates;

		detector.detectMarkers(gray, markerCorners, ids, rejectedCandidates);

		if (ids.size() > 0)
		{
			if (this->showWindow)
			{
				cv::aruco::drawDetectedMarkers(out, markerCorners, ids);
			}

			size_t nMarkers = markerCorners.size();
			std::vector<cv::Vec3d> rVecs(nMarkers);
			std::vector<cv::Vec3d> tVecs(nMarkers);

			for (size_t i = 0; i < nMarkers; i++)
			{
				cv::solvePnP(objPoints, markerCorners.at(i), cam.cameraMat, cam.distCoeffs, rVecs.at(i), tVecs.at(i),
							 false, cv::SOLVEPNP_IPPE_SQUARE); // SOLVEPNP_P3P
			}

			for (int i = 0; i < tVecs.size(); ++i)
			{
				// if (!config.tags.count(ids[i]))
				// {
				// 	continue;
				// }

				cv::Vec3d rVec = rVecs[i];
				cv::Vec3d tVec = tVecs[i];

				//	localizer.addApriltag(ids[i], cam, tVec, rVec, markerLength, dt);

				if (this->showWindow)
				{
					cv::drawFrameAxes(out, cam.cameraMat, cam.distCoeffs, rVec, tVec, 0.1);
				}
			}
		}

		localizer.step(dt);
		if (this->showWindow)
		{
			if (out.cols > 0 && out.rows > 0)
			{
				cv::imshow("Apriltag Debug Window", out);
			}
			else
			{
				std::cout << "[ApriltagDetector] WARNING: Image is " << out.rows << "x" << out.cols << "!" << std::endl;
			}

			if (cv::waitKey(1) == 27)
			{ // ESC key
				break;
			}
		}
	}
}