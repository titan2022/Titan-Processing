#ifndef LOCALIZER
#define LOCALIZER

#include <string>
#include <vector>

#include "apriltag/Apriltag.hpp"
#include "../helper/ConfigReader.hpp"
#include "../helper/Vector3D.hpp"
#include "../networking/Client.hpp"
#include "../physics/PoseFilter.hpp"

class Localizer
{
    public:
        Localizer(ConfigReader &config, NetworkingClient &client, PoseFilter &filter);
        void addApriltag(int id, cv::Vec3d &tvec, cv::Vec3d &rvec, double dt);
        void step(double dt);
        Vector3D position;
        Vector3D rotation;

    private:
        ConfigReader &config;
        NetworkingClient &client;
        PoseFilter filter;
};

#endif