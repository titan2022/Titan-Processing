#ifndef LOCALIZER
#define LOCALIZER

#include <string>
#include <vector>

#include "apriltag/Apriltag.hpp"
#include "../helper/ConfigReader.hpp"
#include "../helper/Vector3D.hpp"
#include "../networking/Client.hpp"

class Localizer
{
    public:
        Localizer(ConfigReader &config, NetworkingClient &client);
        void addApriltag(int id, cv::Vec3d &tvec, cv::Vec3d &rvec);
        Vector3D position;
        Vector3D rotation;

    private:
        ConfigReader &config;
        NetworkingClient &client;

        Apriltag calculatePose(Apriltag &relative, Apriltag &global);
};

#endif