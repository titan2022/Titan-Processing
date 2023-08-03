#include <iostream>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

#include "apriltag/Localizer.hpp"
#include "helper/ConfigReader.hpp"
#include "helper/Vector3D.hpp"
#include "helper/Unit.hpp"

// Translates position origin from camera to tag
Apriltag correctPerspective(int id, cv::Vec3d &tvec, cv::Vec3d &rvec)
{
    cv::Mat R(3, 3, CV_64FC1);
    cv::Rodrigues(rvec, R);

    cv::Mat R_T = R.t();
    cv::Mat posMat = R_T * -tvec;
    cv::Vec3d posVec = posMat;

    cv::Mat rpyMat;
    cv::Mat placeholder; // OpenCV really wants me to keep this...
    cv::Vec3d rotVec = cv::RQDecomp3x3(R_T, rpyMat, placeholder);

    Vector3D pos(posVec);
    Vector3D rot(rotVec);
    rot *= Unit::DEG;

    Apriltag newTag(id, pos, rot);

    return newTag;
}

Localizer::Localizer(ConfigReader &config, NetworkingClient &client) : config(config), client(client)
{
    
}

void Localizer::addApriltag(int id, cv::Vec3d &tvec, cv::Vec3d &rvec)
{
    // TODO: Apriltag camPose = calculatePose(tag, this->config.tags[tag.id - 1]);

    // TODO: add to relative tag hash map
    // TODO: add to absolute tag hash map

    if (id == 5) {
        Apriltag invTag = correctPerspective(id, tvec, rvec);
        this->position = invTag.position;
        this->rotation = invTag.rotation;

        client.send_vector("pos", false, this->position);
        client.send_vector("rot", false, this->rotation);
    }
}

Apriltag Localizer::calculatePose(Apriltag &relative, Apriltag &global) {
    Vector3D newPos = global.position - relative.position;
    Vector3D newRot = global.rotation - relative.rotation;

    Apriltag result(relative.id, newPos, newRot);
    return result;
}