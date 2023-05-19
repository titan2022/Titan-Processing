#include "apriltag/Apriltag.hpp"
#include "helper/Pose.hpp"
#include "helper/Vector3D.hpp"

Apriltag::Apriltag(int id, Pose3D *pose)
{
    init(id, pose->position, pose->rotation);
}

Apriltag::Apriltag(int id, Vector3D *position, Vector3D *rotation)
{
    init(id, position, rotation);
}

void Apriltag::init(int id, Vector3D *position, Vector3D *rotation)
{
    this->id = id;
    this->position = position;
    this->rotation = rotation;
    this->pose = new Pose3D(position, rotation);
}