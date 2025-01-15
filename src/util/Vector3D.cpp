#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "networking/Client.h"
#include "util/Vector3D.hpp"
#include <opencv2/core.hpp>

using namespace titan;

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z)
{
}

Vector3D::Vector3D(const cv::Vec3d &v) : x(v[0]), y(v[1]), z(v[2])
{
}

Vector3D::Vector3D(const std::vector<double> &v) : x(v.at(0)), y(v.at(1)), z(v.at(2))
{
}

Vector3D::Vector3D(const std::array<double, 3> &v) : x(v.at(0)), y(v.at(1)), z(v.at(2))
{
}

Vector3D::Vector3D(const TRBVector3D &v) : x(v.x), y(v.y), z(v.z)
{
}

Vector3D::Vector3D() : x(0), y(0), z(0)
{
}

auto Vector3D::getX() const -> double
{
	return x;
}

auto Vector3D::getY() const -> double
{
	return y;
}

auto Vector3D::getZ() const -> double
{
	return z;
}

/**
 * @brief Returns rotated vector, all units are radians
 *
 * @param angle In Radians
 * @return Vector3D Rotated Vector
 */
auto Vector3D::getRotatedX(double angle) const -> Vector3D
{
	return {x, (y * cos(angle)) - (z * sin(angle)), (y * sin(angle)) + (z * cos(angle))};
}

/**
 * @brief Returns rotated vector, all units are radians
 *
 * @param angle In Radians
 * @return Vector3D Rotated Vector
 */
auto Vector3D::getRotatedY(double angle) const -> Vector3D
{
	return {(x * cos(angle)) + (z * sin(angle)), y, (x * -sin(angle)) + (z * cos(angle))};
}

/**
 * @brief Returns rotated vector, all units are radians
 *
 * @param angle In Radians
 * @return Vector3D Rotated Vector
 */
auto Vector3D::getRotatedZ(double angle) const -> Vector3D
{
	return {(x * cos(angle)) - (y * sin(angle)), (x * sin(angle)) + (y * cos(angle)), z};
}

/**
 * @brief Returns rotated vector in order of ZYX, all units are raiduans
 *
 * @param xAngle In Radians
 * @param yAngle In Radians
 * @param zAngle In Radians
 * @return Vector3D Rotated Vector
 */
auto Vector3D::getRotated(double xAngle, double yAngle, double zAngle) const -> Vector3D
{
	return getRotatedZ(zAngle).getRotatedY(yAngle).getRotatedX(xAngle);
}

/**
 * @brief Returns magnitude or absolute length of vector
 *
 * @return double
 */
double Vector3D::getMagnitude() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

/**
 * @brief Returns normalized vector (unit length of 1)
 *
 * @return Vector3D
 */
auto Vector3D::getNormalized() const -> Vector3D
{
	double mag = getMagnitude();
	Vector3D normalized(x / mag, y / mag, z / mag);
	return normalized;
}

auto Vector3D::toString() const -> std::string
{
	std::string s = "(";
	s += std::to_string(getX());
	s += ", ";
	s += std::to_string(getY());
	s += ", ";
	s += std::to_string(getZ());
	s += ")";
	return s;
}

auto Vector3D::toCV() const -> cv::Vec<double, 3>
{
	return cv::Vec<double, 3>{x, y, z};
}

auto Vector3D::toArray() const -> std::array<double, 3>
{
	return {x, y, z};
}

auto Vector3D::toFfi() const -> TRBVector3D
{
	return {x, y, z};
}

auto Vector3D::fromQuaternion(double w, double x, double y, double z) -> Vector3D
{
	// code from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code_2

	// x-axis
	double sinr_cosp = 2 * (w * x + y * z);
	double cosr_cosp = 1 - (2 * (x * x + y * y));
	double roll = std::atan2(sinr_cosp, cosr_cosp);

	// y-axis
	double sinp = std::sqrt(1 + (2 * (w * y - x * z)));
	double cosp = std::sqrt(1 - (2 * (w * y - x * z)));
	double pitch = (2 * std::atan2(sinp, cosp)) - M_PI / 2;

	// z-axis
	double siny_cosp = 2 * (w * z + x * y);
	double cosy_cosp = 1 - (2 * (y * y + z * z));
	double yaw = std::atan2(siny_cosp, cosy_cosp);

	Vector3D result(roll, pitch, yaw);
	return result;
}
auto Vector3D::toQuaternion() const -> std::tuple<double, double, double, double>
{
	// code from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code

	double cr = cos(x / 2);
	double sr = sin(x / 2);
	double cp = cos(y / 2);
	double sp = sin(y / 2);
	double cy = cos(z / 2);
	double sy = sin(z / 2);

	double w = (cr * cp * cy) + (sr * sp * sy);
	double x = (sr * cp * cy) - (cr * sp * sy);
	double y = (cr * sp * cy) + (sr * cp * sy);
	double z = (cr * cp * sy) - (sr * sp * cy);

	return {w, x, y, z};
}

double Vector3D::setX(const double value)
{
	x = value;
	return x;
}

double Vector3D::setY(const double value)
{
	y = value;
	return y;
}

double Vector3D::setZ(const double value)
{
	z = value;
	return z;
}

/**
 * @brief Normalizes the vector (turns magnitude to 1)
 *
 * @return Vector3D Normalized vector
 */
Vector3D Vector3D::normalize()
{
	Vector3D normalized = getNormalized();
	x = normalized.getX();
	y = normalized.getY();
	z = normalized.getZ();
	return normalized;
}

void Vector3D::rotateX(double angle)
{
	*this = getRotatedX(angle);
}

void Vector3D::rotateY(double angle)
{
	*this = getRotatedY(angle);
}

void Vector3D::rotateZ(double angle)
{
	*this = getRotatedZ(angle);
}

/**
 * @brief Rotates vector in order of ZYX, all units are radians
 *
 * @param xAngle In Radians
 * @param yAngle In Radians
 * @param zAngle In Radians
 * @return Vector3D Rotated Vector
 */
void Vector3D::rotate(double xAngle, double yAngle, double zAngle)
{
	rotateZ(zAngle);
	rotateY(yAngle);
	rotateX(xAngle);
}

/**
 * @brief Rotates vector in order of ZYX, all units are radians
 *
 * @param vec Rotation vector
 * @return Vector3D Rotated Vector
 */
void Vector3D::rotate(const Vector3D &vec)
{
	rotateZ(vec.getZ());
	rotateY(vec.getY());
	rotateX(vec.getX());
}

auto Vector3D::operator+(const Vector3D &v) const -> Vector3D
{
	return {x + v.x, y + v.y, z + v.z};
}

auto Vector3D::operator-(const Vector3D &v) const -> Vector3D
{
	return {x - v.x, y - v.y, z - v.z};
}

void Vector3D::operator+=(Vector3D const &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector3D::operator-=(Vector3D const &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

auto Vector3D::operator*(const double s) const -> Vector3D
{
	return {x * s, y * s, z * s};
}

auto Vector3D::operator/(const double s) const -> Vector3D
{
	return {x / s, y / s, z / s};
}

void Vector3D::operator*=(const double s)
{
	x *= s;
	y *= s;
	z *= s;
}

void Vector3D::operator/=(const double s)
{
	x /= s;
	y /= s;
	z /= s;
}

auto Vector3D::operator*(const Vector3D &v) const -> double
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

auto Vector3D::operator==(const Vector3D &v) const -> bool
{
	return x == v.x && y == v.y && z == v.z;
}

auto Vector3D::operator!=(const Vector3D &v) const -> bool
{
	return !(*this == v);
}

/**
 * @brief Gets value. 0 = x, 1 = y, 2 = z
 *
 * @param index From 0 to 2
 * @return const double
 */
auto Vector3D::operator[](const int index) const -> double
{
	switch (index)
	{
	case 0:
		return x;

	case 1:
		return y;

	case 2:
		return z;
	default:
		throw std::runtime_error("Invalid Vector3D index");
	}
}