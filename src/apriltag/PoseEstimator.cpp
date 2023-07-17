#include "apriltag/PoseEstimator.hpp"
#include "helper/Pose.hpp"
#include "helper/Vector3D.hpp"

PoseEstimator::PoseEstimator(Pose3D *poses, void (*handle)(const Vector3D &))
{
    this->apriltagPoses = poses;
}

void PoseEstimator::addApriltag(Apriltag &tag)
{

    delete &tag;
}