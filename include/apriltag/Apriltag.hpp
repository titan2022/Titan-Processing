#ifndef APRILTAG
#define APRILTAG

#include <string>

#include "../helper/Pose.hpp"
#include "../helper/Vector3D.hpp"

class Apriltag
{
    public:
        Vector3D &position;
        Vector3D &rotation;
        Pose3D *pose;
        int id;
        Apriltag(int id, Vector3D &position, Vector3D &rotation);
};

#endif