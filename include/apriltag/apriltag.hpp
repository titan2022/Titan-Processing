#ifndef APRILTAG
#define APRILTAG

#include <string>

#include "helper/pose.hpp"
#include "helper/vector.hpp"

class Apriltag
{
    public:
        Vector3D* position;
        Vector3D* rotation;
        Pose3D* pose;
        int id;
        Apriltag(int id, Pose3D* pose);
        Apriltag(int id, Vector3D* position, Vector3D* rotation);

    private:
        void init(int id, Vector3D* position, Vector3D* rotation);
};

#endif