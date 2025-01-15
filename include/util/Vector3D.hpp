#ifndef VECTOR
#define VECTOR

#include <array>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

namespace titan
{
class Vector3D
{
  public:
	struct Quaternion
	{
		double w;
		double x;
		double y;
		double z;

		Quaternion operator*(Quaternion);
		static Quaternion fromAxisAngle(Vector3D axis, double angle);
		static Quaternion fromWPILibQuaternion(Quaternion quat);
	};

	Vector3D(double x, double y, double z);
	Vector3D(cv::Vec3d v);
	Vector3D(std::vector<double> v);
	Vector3D(double (&v)[]);
	Vector3D(const char *d);
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
    
	static Vector3D fromWPILibPosition(double x, double y, double z, double fieldLength, double fieldWidth);

	static Vector3D matToVec(double (&mat)[]);
	static Vector3D fromQuaternion(double w, double x, double y, double z);
	static Vector3D fromQuaternion(Vector3D::Quaternion quaternion);
	static Vector3D fromWPILibQuaternion(double w, double x, double y, double z);
	static Vector3D fromWPILibQuaternion(Vector3D::Quaternion quaternion);

	/// Returns a tuple of (w, x, y, z).
	Vector3D::Quaternion toQuaternion();

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

  private:
	double x;
	double y;
	double z;
};
} // namespace titan

#endif