#include <cmath>
#include <string>
#include <opencv2/core.hpp>

#include "helper/Vector3D.hpp"

Vector3D::Vector3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D::Vector3D(cv::Vec<double, 3> v)
{
    x = v[0];
    y = v[1];
    z = v[2];
}

Vector3D::Vector3D(cv::Vec<float, 3> v)
{
    x = v[0];
    y = v[1];
    z = v[2];
}

Vector3D::Vector3D(cv::Vec<int, 3> v)
{
    x = v[0];
    y = v[1];
    z = v[2];
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
    return getRotatedX(xAngle).getRotatedY(yAngle).getRotatedZ(zAngle);
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
    s += std::to_string(x) + ", ";
    s += std::to_string(y) + ", ";
    s += std::to_string(z);
    s += ")";
    return s;
}

cv::Vec<double, 3> Vector3D::toCV()
{
    return cv::Vec<double, 3>{x, y, z};
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

const Vector3D Vector3D::operator*(const double s)
{
    return Vector3D(x * s, y * s, z * s);
}

const Vector3D Vector3D::operator/(const double s)
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