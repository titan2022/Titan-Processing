#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <map>

#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"
#include "util/Vector3D.hpp"

using namespace titan;

// Source: https://github.com/rlabbe/Kalman-and-Bayesian-Filters-in-Python

template <typename T> std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
}

std::string pm(cv::Mat &m)
{
	std::string result = "[";

	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			double value = std::min(m.at<double>(i, j), 999.0);
			result += to_string_with_precision(value, 1);
			result += ", ";
		}
		result += "\n";
	}

	result += "]";

	return result;
}

PoseFilter::PoseFilter(Config &config) : config(config), position(), rotation()
{
	// Initialize covariance matrices
	for (auto &tag : config.tags)
	{
		P[tag.second.id] = cv::Mat::eye(12, 12, CV_64FC1);
	}

	// Initialize process noise matrix
	Q = cv::Mat::eye(12, 12, CV_64FC1);
	for (int i = 0; i < 12; i++)
	{
		Q.at<double>(i, i) = 1;
	}

	// Initialize system noise offset matrix
	R = cv::Mat::eye(6, 6, CV_64FC1);
	for (int i = 0; i < 6; i++)
	{
		R.at<double>(i, i) = 0.25;
	}

	// Initialize control input at default (zero == unknown)
	u = cv::Mat::eye(1, 1, CV_64FC1);
	B = cv::Mat::eye(1, 1, CV_64FC1);

	// Initialize measurement function
	// Reduces matrix with extra variables (velocity)
	// to get residual from measurements
	H = cv::Mat::eye(6, 12, CV_64FC1);

	// Initialize a 12x12 identity matrix for covariance update
	I = cv::Mat::eye(12, 12, CV_64FC1);
}

void PoseFilter::predict(double dt)
{
	if (!init)
	{
		return;
	}

	// Process model matrix
	// Keeps velocity constant and integrates position
	// using the constant velocity (with respect to delta time)
	cv::Mat F = cv::Mat::eye(12, 12, CV_64FC1);
	for (int i = 0; i < 12; i++)
	{
		if (i < 6)
		{
			F.at<double>(i, i + 6) = dt;
		}
	}

	// Predicting means

	x = F * x; // + B * u;

	test2 = x.at<double>(2, 0);

	position.setX(x.at<double>(0, 0));
	position.setY(x.at<double>(1, 0));
	position.setZ(x.at<double>(2, 0));
	rotation.x = (x.at<double>(3, 0));
	rotation.y = (x.at<double>(4, 0));
	rotation.z = (x.at<double>(5, 0));

	for (auto &tag : config.tags)
	{
		// Predicting covariance
		P[tag.second.id] = F * P[tag.second.id] * F.t() + Q;
	}
}

void PoseFilter::updateTag(Apriltag &tag, double tagDist, double dt)
{
	double xPos = tag.position.getX();
	double yPos = tag.position.getY();
	double zPos = tag.position.getZ();
	double xRot = tag.rotation.x;
	double yRot = tag.rotation.y;
	double zRot = tag.rotation.z;

	if (!init)
	{
		// Initialize mean matrix
		cv::Mat x_init(12, 1, CV_64FC1);
		x = x_init;
		for (int i = 0; i < 3; i++)
		{
			x.at<double>(i, 0) = tag.position[i];
			x.at<double>(i + 3, 0) = tag.rotation.coerceToVector3D()[i];
			x.at<double>(i + 6, 0) = 0;
			x.at<double>(i + 9, 0) = 0;
		}

		init = true;
	}

	// Measured values matrix
	cv::Mat z(6, 1, CV_64FC1);
	z.at<double>(0, 0) = xPos;
	z.at<double>(1, 0) = yPos;
	z.at<double>(2, 0) = zPos;
	z.at<double>(3, 0) = xRot;
	z.at<double>(4, 0) = yRot;
	z.at<double>(5, 0) = zRot;

	// Increase uncertainty based on distance
	for (int i = 0; i < 3; i++)
	{
		R.at<double>(i, i) = tagDist * tagDist;
	}

	// System uncertenty
	cv::Mat S = H * P[tag.id] * H.t() + R;

	// Kalman gain
	cv::Mat K = P[tag.id] * H.t() * S.inv();

	// Getting the residual between mean prediction and measurement
	cv::Mat y = z - H * x;

	// Updating mean (final result)
	x = x + K * y;

	position.setX(x.at<double>(0, 0));
	position.setY(x.at<double>(1, 0));
	position.setZ(x.at<double>(2, 0));
	rotation.x = (x.at<double>(3, 0));
	rotation.y = (x.at<double>(4, 0));
	rotation.z = (x.at<double>(5, 0));

	test1 = zPos;

	// Updating covariance
	P[tag.id] = (I - K * H) * P[tag.id];
}

void PoseFilter::addControlInput(double x_vel, double y_vel, double rot_vel)
{
}