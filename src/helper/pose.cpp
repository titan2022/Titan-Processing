#include "helper/pose.hpp"
#include "helper/vector.hpp"

Pose3D::Pose3D(Vector3D* position, Vector3D* rotation)
{
    this->position = position;
    this->rotation = rotation;
}