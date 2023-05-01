#ifndef POSE_ESTIMATOR
#define POSE_ESTIMATOR

#include <string>

#include "apriltag/apriltag.hpp"
#include "../helper/pose.hpp"
#include "../helper/vector.hpp"

class PoseEstimator
{
    public:
        PoseEstimator(Pose3D *poses, void (*handle)(const Vector3D &));
        void addApriltag(Apriltag &tag);

    private:
        Pose3D *apriltagPoses;
};

#endif