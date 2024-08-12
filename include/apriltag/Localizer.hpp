#ifndef LOCALIZER
#define LOCALIZER

#include <string>
#include <vector>

#include "apriltag/Apriltag.hpp"
#include "../util/ConfigReader.hpp"
#include "../util/Vector3D.hpp"
#include "../networking/Client.h"
#include "../physics/PoseFilter.hpp"

class Localizer
{
    public:
        Localizer(ConfigReader &config, NetworkingClient &client, NetworkingClient &dashboardClient, PoseFilter &filter);
        void addApriltag(int id, int camId, cv::Vec3d &tvec, cv::Vec3d &rvec, int size, double dt);
        void step(double dt);
        Vector3D position;
        Vector3D rotation;

    private:
        Apriltag* getGlobalTag(int id);
        ConfigReader &config;
        NetworkingClient &client;
        NetworkingClient &dashboardClient;
        PoseFilter filter;

};

#endif