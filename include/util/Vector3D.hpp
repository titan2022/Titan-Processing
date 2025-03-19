#ifndef VECTOR
#define VECTOR

#include <array>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <frc/geometry/Pose3d.h>
#include <frc/geometry/Transform3d.h>
#include <frc/geometry/Translation3d.h>
#include <frc/geometry/Rotation3d.h>
#include <frc/geometry/CoordinateSystem.h>
#include <frc/geometry/CoordinateAxis.h>

namespace titan
{
using CoordinateSystem = frc::CoordinateSystem;
using CoordinateAxis = frc::CoordinateAxis;
using Transform3d = frc::Transform3d;
using Rotation3d = frc::Rotation3d;
using Translation3d = frc::Translation3d;
// using Pose3d = frc::Pose3d; // We prefer to use Transform3d for poses
using Quaternion = frc::Quaternion;

namespace CoordinateSystems {
	constexpr CoordinateSystem WPILib() {
		return CoordinateSystem{CoordinateAxis::N(), CoordinateAxis::W(), CoordinateAxis::U()};
	}
	constexpr CoordinateSystem OpenCV() {
		return CoordinateSystem{CoordinateAxis::E(), CoordinateAxis::D(), CoordinateAxis::N()};
	}
	constexpr CoordinateSystem THREEjs() {
		return CoordinateSystem{CoordinateAxis::W(), CoordinateAxis::U(), CoordinateAxis::N()};
	}
	constexpr CoordinateSystem standard() {
		return WPILib();
	}
}

constexpr Translation3d standardFromWPILibPosition(Translation3d translation, CoordinateSystem inputSystem, double fieldLength, double fieldWidth) {
	return CoordinateSystem::Convert(translation, inputSystem, CoordinateSystems::standard());
}

constexpr Translation3d oldTHREEjsFromWPILibPosition(Translation3d translation, CoordinateSystem inputSystem, double fieldLength, double fieldWidth) {
	Translation3d point = CoordinateSystem::Convert(translation, inputSystem, CoordinateSystems::WPILib());
	return point + Translation3d{point.X() - units::meter_t{fieldLength}/2.0, point.Z(), -(point.Y() - units::meter_t{fieldWidth}/2.0)};
}

class Vector3D
{
  public:
	Vector3D(double x, double y, double z);
	Vector3D(cv::Vec3d v);
	Vector3D(std::vector<double> v);
	Vector3D(double (&v)[]);
	Vector3D(const char *d);
	Vector3D(Translation3d translation);
	Vector3D(Rotation3d rotation);
	Vector3D();

	constexpr operator Translation3d() const {
		return Translation3d{units::meter_t{x}, units::meter_t{y}, units::meter_t{z}};
	}

	constexpr operator Rotation3d() const {
		return Rotation3d{units::radian_t{x}, units::radian_t{y}, units::radian_t{z}};
	}

	double getX();
	double getY();
	double getZ();
	Vector3D getRotatedX(double angle);
	Vector3D getRotatedY(double angle);
	Vector3D getRotatedZ(double angle);
	Vector3D getRotated(double xAngle, double yAngle, double zAngle);
	double getMagnitude();
	Vector3D getNormalized();
	std::string toString();
	cv::Vec<double, 3> toCV();
	std::array<double, 3> toArray();

	static Vector3D matToVec(double (&mat)[]);

	double setX(const double value);
	double setY(const double value);
	double setZ(const double value);
	Vector3D normalize();
	void rotateX(double angle);
	void rotateY(double angle);
	void rotateZ(double angle);
	void rotate(double xAngle, double yAngle, double zAngle);
	void rotate(Vector3D &vec);

	Vector3D &operator=(const Vector3D &v);
	const Vector3D operator+(const Vector3D &v);
	const Vector3D operator-(const Vector3D &v);
	Vector3D &operator+=(const Vector3D &v);
	Vector3D &operator-=(const Vector3D &v);
	Vector3D operator*(const double s);
	Vector3D operator/(const double s);
	Vector3D &operator*=(const double s);
	Vector3D &operator/=(const double s);
	const double operator*(const Vector3D &v);
	const bool operator==(const Vector3D &v);
	const bool operator!=(const Vector3D &v);
	const double operator[](const int index);

  public:
	double x;
	double y;
	double z;
};

using Translation = Vector3D;

} // namespace titan


#endif