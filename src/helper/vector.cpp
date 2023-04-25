#include <cmath>

#include "helper/vector.hpp"

Vector3D::Vector3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3D::getX()
{
    return this->x;
}

double Vector3D::getY()
{
    return this->y;
}

double Vector3D::getZ()
{
    return this->z;
}

double Vector3D::getMagnitude()
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3D Vector3D::getNormalized()
{
    double mag = this->getMagnitude();
    Vector3D normalized(this->x / mag, this->y / mag, this->z / mag);
    return normalized;
}

Vector3D Vector3D::normalize() {
    Vector3D normalized = this->getNormalized();
    this->x = normalized.getX();
    this->y = normalized.getY();
    this->z = normalized.getZ();
    return normalized;
}

Vector3D Vector3D::scale(double scalar)
{
    Vector3D scaled(this->x * scalar, this->y * scalar, this->z * scalar);
    return scaled;
}

std::string Vector3D::toString()
{
    std::string s = "(";
    s += std::to_string(this->x) + ", ";
    s += std::to_string(this->y) + ", ";
    s += std::to_string(this->z);
    s += ")";
    return s;
}

double Vector3D::dotProduct(Vector3D v)
{
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vector3D& Vector3D::operator+(Vector3D const &v)
{
    Vector3D result(x + v.x, y + v.y, z + v.z);
    return result;
}

Vector3D& Vector3D::operator-(Vector3D const &v)
{
    Vector3D result(x - v.x, y - v.y, z - v.z);
    return result;
}

Vector3D& Vector3D::operator+=(Vector3D const &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}

Vector3D& Vector3D::operator-=(Vector3D const &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
}

double& Vector3D::operator[](int index) {
    switch (index)
    {
        case 0:
            return this->x;
            break;
        case 1:
            return this->y;
            break;
        case 2:
            return this->z;
            break;
        default:
            break;
    }

    return this->x;
}