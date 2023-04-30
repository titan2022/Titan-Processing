#ifndef POSE
#define POSE

#include "helper/vector.hpp"

class Pose3D
{
    public:
        Vector3D *position;
        Vector3D *rotation;
        Pose3D(Vector3D *position, Vector3D *rotation);
};

#endif