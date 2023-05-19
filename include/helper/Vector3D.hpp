#ifndef VECTOR
#define VECTOR

#include <string>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>

class Vector3D
{
    public:
        Vector3D(double x, double y, double z);
        Vector3D(cv::Vec<float, 3> v);
        Vector3D(cv::Vec<double, 3> v);
        Vector3D(cv::Vec<int, 3> v);
        Vector3D(rs2_vector v);
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
        rs2_vector toRS2();
        cv::Vec<double, 3> toCV();

        Vector3D normalize();
        void rotateX(double angle);
	    void rotateY(double angle);
	    void rotateZ(double angle);
	    void rotate(double xAngle, double yAngle, double zAngle);

        const Vector3D operator+(const Vector3D &v);
        const Vector3D operator-(const Vector3D &v);
        Vector3D &operator+=(const Vector3D &v);
        Vector3D &operator-=(const Vector3D &v);
        const Vector3D operator*(const double s);
        const Vector3D operator/(const double s);
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

#endif