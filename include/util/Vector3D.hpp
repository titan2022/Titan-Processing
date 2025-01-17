#ifndef VECTOR
#define VECTOR

#include <array>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

namespace titan
{

enum CoordinateSystem
{
	UNKNOWN,
	THREEJS, // Used throughout most of the Titan-Processing codebase
			// Field: Y up, X length, Z width
			// Local: Y up, X left, Z forward
	OPENCV, // Result of cv::solvePnP. Converted to THREEJS as soon as possible.
			// Not used for global (field) coordinates.
			// Local: Y down, X right, Z forward
	WPILIB, // Used in most of the Java code, by WPILib, by vendors,
			// and when reading apriltags2025.json.
			// Field: Z up, X length, Y width
			// Local: Z up, Y left, X forward
};

extern std::string nameOfCoordinateSystem(CoordinateSystem coordinateSystem);

struct RotationQuaternion;
struct RotationMatrix;
struct EulerAngles;
struct Transform;
// class Translation;

class Vector3D
{
  public:
	Vector3D(double x, double y, double z);
	Vector3D(double x, double y, double z, CoordinateSystem coordinateSystem);
	Vector3D(cv::Vec3d v);
	Vector3D(cv::Vec3d v, CoordinateSystem coordinateSystem);
	Vector3D(std::vector<double> v);
	Vector3D(std::vector<double> v, CoordinateSystem coordinateSystem);
	Vector3D(double (&v)[]);
	Vector3D(double (&v)[], CoordinateSystem coordinateSystem);
	Vector3D(const char *d);
	Vector3D(const char *d, CoordinateSystem coordinateSystem);
	Vector3D();

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

	// Abuse of Vector3D as euler angles...
	EulerAngles coerceToEulerAngles();

	// static Vector3D fromQuaternion(double w, double x, double y, double z);
	// static Vector3D fromQuaternion(RotationQuaternion quaternion);
	// static Vector3D fromWPILibQuaternion(double w, double x, double y, double z);
	// static Vector3D fromWPILibQuaternion(RotationQuaternion quaternion);

	/// Returns a tuple of (w, x, y, z).
	// RotationQuaternion toQuaternion();

	// cv::Mat toRotationMatrix();
	// static Vector3D fromRotationMatrix(const cv::Mat &R);
	// static cv::Mat makeTransform(Vector3D position, Vector3D orientation);
	// static Vector3D positionFromTransform(const cv::Mat &T);
	// static Vector3D orientationFromTransform(const cv::Mat &T);

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

	// New translations code
	Vector3D convertToCoordinateSystem(CoordinateSystem target);

	static Vector3D fromWPILibPosition(double x, double y, double z, double fieldLength, double fieldWidth);

  public:
	double x;
	double y;
	double z;

	CoordinateSystem coordinateSystem = UNKNOWN;
};

using Translation = Vector3D;

// Rotation matrix
// The rotation matrix is created using Rz * Ry * Rx, regardless of coordinate system.
struct RotationMatrix
{
	cv::Mat data;
	CoordinateSystem coordinateSystem = UNKNOWN;

	RotationMatrix convertToCoordinateSystem(CoordinateSystem target);
	static RotationMatrix fromRotationVector(cv::Vec3d rvec, CoordinateSystem coordinateSystem);
	EulerAngles toEulerAngles();
};

struct RotationQuaternion
{
	double w;
	double x;
	double y;
	double z;
	CoordinateSystem coordinateSystem = UNKNOWN;

	RotationQuaternion operator*(RotationQuaternion);
	double dotProduct(RotationQuaternion other);

	bool isEquivalent(RotationQuaternion other);

	RotationQuaternion convertToCoordinateSystem(CoordinateSystem target);
	static RotationQuaternion fromAxisAngle(Vector3D axis, double angle);
	EulerAngles toEulerAngles();
};

struct EulerAngles
{
	double x;
	double y;
	double z;
	CoordinateSystem coordinateSystem = UNKNOWN;

	EulerAngles convertToCoordinateSystem(CoordinateSystem target);
	RotationQuaternion toRotationQuaternion();
	RotationMatrix toRotationMatrix();
	Vector3D coerceToVector3D();
};

struct Transform
{
	cv::Mat data;
	CoordinateSystem coordinateSystem = UNKNOWN;

	Transform(Translation position, RotationMatrix orientation);
	Transform(cv::Mat data, CoordinateSystem coordinateSystem);

	Translation getPosition();
	RotationMatrix getOrientation();

	Transform inv();
	Transform operator*(Transform other);
};

// class Translation : Vector3D
// {
//   public:
	
// };

} // namespace titan

#endif