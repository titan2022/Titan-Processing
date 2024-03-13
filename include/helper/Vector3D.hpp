#ifndef VECTOR
#define VECTOR

#include <string>
#include <vector>
#include <array>

#if 0
#include <opencv2/core.hpp>
#endif

class Vector3D
{
    public:
        Vector3D(double x, double y, double z);
        #if 0
        Vector3D(cv::Vec3d v);
        #endif
        Vector3D(std::vector<double> v);
        Vector3D(double (&v)[]);
        Vector3D(char* d);
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
        #if 0
        cv::Vec<double, 3> toCV();
        #endif
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

    private:
        double x;
        double y;
        double z;
};

#endif