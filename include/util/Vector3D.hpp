#ifndef VECTOR
#define VECTOR

#include <array>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

struct TRBVector3D;

namespace titan
{

class Vector3D
{
  public:
	Vector3D(double x, double y, double z);
	Vector3D(const cv::Vec3d &v);
	Vector3D(const std::vector<double> &v);
	Vector3D(const std::array<double, 3> &v);
	Vector3D(const TRBVector3D &v);
	Vector3D();

	[[nodiscard]] auto getX() const -> double;
	[[nodiscard]] auto getY() const -> double;
	[[nodiscard]] auto getZ() const -> double;
	[[nodiscard]] auto getRotatedX(double angle) const -> Vector3D;
	[[nodiscard]] auto getRotatedY(double angle) const -> Vector3D;
	[[nodiscard]] auto getRotatedZ(double angle) const -> Vector3D;
	[[nodiscard]] auto getRotated(double xAngle, double yAngle, double zAngle) const -> Vector3D;
	[[nodiscard]] auto getMagnitude() const -> double;
	[[nodiscard]] auto getNormalized() const -> Vector3D;
	[[nodiscard]] auto toString() const -> std::string;
	[[nodiscard]] auto toCV() const -> cv::Vec<double, 3>;
	[[nodiscard]] auto toArray() const -> std::array<double, 3>;
	[[nodiscard]] auto toFfi() const -> TRBVector3D;

	static auto fromQuaternion(double w, double x, double y, double z) -> Vector3D;
	/// Returns a tuple of (w, x, y, z).
	[[nodiscard]] auto toQuaternion() const -> std::tuple<double, double, double, double>;

	auto setX(double value) -> double;
	auto setY(double value) -> double;
	auto setZ(double value) -> double;
	auto normalize() -> Vector3D;
	void rotateX(double angle);
	void rotateY(double angle);
	void rotateZ(double angle);
	void rotate(double xAngle, double yAngle, double zAngle);
	void rotate(const Vector3D &vec);

	auto operator+(const Vector3D &v) const -> Vector3D;
	auto operator-(const Vector3D &v) const -> Vector3D;
	void operator+=(const Vector3D &v);
	void operator-=(const Vector3D &v);
	auto operator*(double s) const -> Vector3D;
	auto operator/(double s) const -> Vector3D;
	void operator*=(double s);
	void operator/=(double s);
	auto operator*(const Vector3D &v) const -> double;
	auto operator==(const Vector3D &v) const -> bool;
	auto operator!=(const Vector3D &v) const -> bool;
	auto operator[](int index) const -> double;

  private:
	double x;
	double y;
	double z;
};
} // namespace titan

#endif