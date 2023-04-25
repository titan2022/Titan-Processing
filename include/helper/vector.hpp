#ifndef VECTOR
#define VECTOR

#include <string>
#include <librealsense2/rs.hpp>

class Vector3D
{
    public:
        Vector3D(double x, double y, double z);
        double getX();
        double getY();
        double getZ();
        double getMagnitude();
        Vector3D getNormalized();
        Vector3D normalize();
        Vector3D scale(double scalar);
        std::string toString();
        rs2_vector toRS2();
        
        double dotProduct(Vector3D v);

        Vector3D& operator+(const Vector3D& v);
        Vector3D& operator-(const Vector3D& v);
        Vector3D& operator+=(const Vector3D& v);
        Vector3D& operator-=(const Vector3D& v);
        double& operator[](int index);
        
    private:
        double x;
        double y;
        double z;
};

#endif