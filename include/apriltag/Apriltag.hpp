#ifndef APRILTAG
#define APRILTAG

#include <string>

#include "../helper/Vector3D.hpp"

class Apriltag
{
    public:
        Vector3D &position;
        Vector3D &rotation;
        int id;
        Apriltag(int id, Vector3D &position, Vector3D &rotation);
};

#endif