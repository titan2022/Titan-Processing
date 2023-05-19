#ifndef POSE
#define POSE

#include "../helper/Vector3D.hpp"

class Pose3D
{
    public:
        Vector3D *position;
        Vector3D *rotation;
        Pose3D(Vector3D *position, Vector3D *rotation);
};

#endif