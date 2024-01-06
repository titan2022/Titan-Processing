#ifndef APRILTAG
#define APRILTAG

#include <string>

#include "../helper/Vector3D.hpp"

class Apriltag
{
    public:
        Apriltag(int id, Vector3D &position, Vector3D &rotation, double size);
        Apriltag& operator=(const Apriltag& other);
        Vector3D position;
        Vector3D rotation;
        double size;
        int id;
};

#endif