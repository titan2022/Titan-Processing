#ifndef POSE_ESTIMATOR
#define POSE_ESTIMATOR

#include <string>

#include "apriltag/Apriltag.hpp"
#include "../helper/Pose.hpp"
#include "../helper/Vector3D.hpp"

class PoseEstimator
{
    public:
        PoseEstimator(Pose3D *poses, void (*handle)(const Vector3D &));
        void addApriltag(Apriltag &tag);

    private:
        Pose3D *apriltagPoses;
};

#endif