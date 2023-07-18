#ifndef LOCALIZER
#define LOCALIZER

#include <string>

#include "apriltag/Apriltag.hpp"
#include "../helper/ConfigReader.hpp"
#include "../helper/Vector3D.hpp"
#include "../networking/Client.hpp"

class Localizer
{
    public:
        Localizer(ConfigReader &config, NetworkingClient &client);
        void addApriltag(Apriltag &tag);

    private:
        ConfigReader &config;
        NetworkingClient &client;
};

#endif