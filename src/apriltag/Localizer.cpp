#include <iostream>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

#include "apriltag/Localizer.hpp"
#include "helper/ConfigReader.hpp"
#include "helper/Vector3D.hpp"
#include "helper/Unit.hpp"

// Translates position origin from camera to tag
Apriltag correctPerspective(int id, cv::Vec3d &tvec, cv::Vec3d &rvec, int size)
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

    Apriltag newTag(id, pos, rot, size);

    return newTag;
}

Apriltag toGlobalPose(Apriltag &relative, Apriltag &global) {
    Vector3D newPos = global.position - relative.position;
    Vector3D newRot = global.rotation - relative.rotation;

    Apriltag result(relative.id, newPos, newRot, relative.size);
    return result;
}

Apriltag* Localizer::getGlobalTag(int id) {
    for (auto tagPair : this->config.tags) {
        if (tagPair.second->id == id) {
            return tagPair.second;
        }
    }
    return nullptr;
}

Localizer::Localizer(ConfigReader &config, NetworkingClient &client, PoseFilter &filter) :
config(config), client(client), filter(filter)
{
    
}

void Localizer::addApriltag(int id, cv::Vec3d &tvec, cv::Vec3d &rvec, int size, double dt)
{
    // TODO: add to relative tag hash map
    // TODO: add to absolute tag hash map

    Apriltag invTag = correctPerspective(id, tvec, rvec, size);
    
    auto globTag = getGlobalTag(id);

    invTag.position *= -1;
    invTag.position.setZ(-invTag.position.getZ());
    invTag.rotation.setY(-invTag.rotation.getY());

    invTag.position += globTag->position;
    invTag.rotation += globTag->rotation;

    filter.updateTag(invTag, dt);
}

void Localizer::step(double dt) {
    // client.send_vector("pos", filter.position, false);
    // client.send_vector("rot", filter.rotation, false);

    // Vector3D test(filter.test1, filter.test2, 0);
    // client.send_vector("test", test, false);
    
    client.send_pose("pose", filter.position, filter.rotation);

    filter.predict(dt);
}