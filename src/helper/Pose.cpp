#include "helper/Pose.hpp"
#include "helper/Vector3D.hpp"

Pose3D::Pose3D(Vector3D *position, Vector3D *rotation)
{
    this->position = position;
    this->rotation = rotation;
}