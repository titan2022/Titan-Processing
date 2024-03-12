#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <array>

#include <opencv2/core.hpp>

#include "helper/Vector3D.hpp"

Vector3D::Vector3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D::Vector3D(cv::Vec3d v)
{
    x = v[0];
    y = v[1];
    z = v[2];
}

Vector3D::Vector3D(std::vector<double> v)
{
    x = v[0];
    y = v[1];
    z = v[2];
}

Vector3D::Vector3D(double (&v)[])
{
    x = v[0];
    y = v[1];
    z = v[2];
}

struct StructVector3D
{
    double x;
    double y;
    double z;
};
Vector3D::Vector3D(char* d)
{
    StructVector3D* structVec = reinterpret_cast<StructVector3D *>(d);
    x = structVec->x;
    y = structVec->y;
    z = structVec->z;
}

Vector3D::Vector3D()
{
    x = 0;
    y = 0;
    z = 0;
}

double Vector3D::getX()
{
    return x;
}

double Vector3D::getY()
{
    return y;
}

double Vector3D::getZ()
{
    return z;
}

Vector3D Vector3D::getRotatedX(double angle)
{
    return Vector3D(x, y * cos(angle) - z * sin(angle), y * sin(angle) + z * cos(angle));
}

Vector3D Vector3D::getRotatedY(double angle)
{
    return Vector3D(x * cos(angle) + z * sin(angle), y, x * -sin(angle) + z * cos(angle));
}

Vector3D Vector3D::getRotatedZ(double angle)
{
    return Vector3D(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle), z);
}

Vector3D Vector3D::getRotated(double xAngle, double yAngle, double zAngle)
{
    return getRotatedZ(zAngle).getRotatedY(yAngle).getRotatedX(xAngle);
}

double Vector3D::getMagnitude()
{
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::getNormalized()
{
    double mag = getMagnitude();
    Vector3D normalized(x / mag, y / mag, z / mag);
    return normalized;
}

std::string Vector3D::toString()
{
    std::string s = "(";
    std::string xStr = std::to_string(getX());
    std::string yStr = std::to_string(getY());
    std::string zStr = std::to_string(getZ());
    s += xStr + ", ";
    s += yStr + ", ";
    s += zStr;
    s += ")";
    return s;
}

cv::Vec<double, 3> Vector3D::toCV()
{
    return cv::Vec<double, 3>{x, y, z};
}

std::array<double, 3> Vector3D::toArray() {
    std::array<double, 3> arr;
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
    return arr;
}

Vector3D Vector3D::matToVec(double (&mat)[]) {
    // Source: http://motion.pratt.duke.edu/RoboticSystems/3DRotations.html#Converting-from-a-rotation-matrix
    // Temporarily copied from: https://github.com/titan2022/Jetson-Vision-2023/blob/apriltags/py/detector_client.py
    // 3x3 matrices follow row-major order (index = row * 3 + col)
    double theta = acos((mat[0] + mat[4] + mat[8] - 1) / 2);
    Vector3D pre(0, 0, 0);
    if (sin(theta) < 1e-3) {
        return pre; // TODO: impliment special case of theta == pi
    }

    double rk = (2 * sin(theta));
    pre.setX((mat[4] - mat[2]) / rk); // -5, -7
    pre.setY((mat[8] - mat[6]) / rk); // -1, -3
    pre.setZ((mat[3] - mat[1]) / rk); // 3, 1

    Vector3D result(pre[0] * theta, pre[1] * theta, pre[2] * theta);
    return result;
}

double Vector3D::setX(const double value) {
    x = value;
    return x;
}

double Vector3D::setY(const double value) {
    y = value;
    return y;
}

double Vector3D::setZ(const double value) {
    z = value;
    return z;
}

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

void Vector3D::rotate(double xAngle, double yAngle, double zAngle)
{
    rotateX(xAngle);
    rotateY(yAngle);
    rotateZ(zAngle);
}

void Vector3D::rotate(Vector3D &vec)
{
    rotateX(vec.getX());
    rotateY(vec.getY());
    rotateZ(vec.getZ());
}

Vector3D &Vector3D::operator=(const Vector3D &v)
{
    setX(v.x);
    setY(v.y);
    setZ(v.z);
    return *this;
}

const Vector3D Vector3D::operator+(const Vector3D &v)
{
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

const Vector3D Vector3D::operator-(const Vector3D &v)
{
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D &Vector3D::operator+=(Vector3D const &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3D &Vector3D::operator-=(Vector3D const &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3D Vector3D::operator*(const double s)
{
    return Vector3D(x * s, y * s, z * s);
}

Vector3D Vector3D::operator/(const double s)
{
    return Vector3D(x / s, y / s, z / s);
}

Vector3D &Vector3D::operator*=(const double s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vector3D &Vector3D::operator/=(const double s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

const double Vector3D::operator*(const Vector3D &v)
{
    return x * v.x + y * v.y + z * v.z;
}

const bool Vector3D::operator==(const Vector3D &v)
{
    return x == v.x && y == v.y && z == v.z;
}

const bool Vector3D::operator!=(const Vector3D &v)
{
    return !(*this == v);
}

const double Vector3D::operator[](const int index)
{
    switch (index)
    {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            break;
    }

    return 0;
}