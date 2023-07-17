#include "apriltag/Apriltag.hpp"
#include "helper/Pose.hpp"
#include "helper/Vector3D.hpp"

Apriltag::Apriltag(int id, Vector3D &position, Vector3D &rotation) : id(id), position(position), rotation(rotation)
{
    this->pose = new Pose3D(&position, &rotation);
}