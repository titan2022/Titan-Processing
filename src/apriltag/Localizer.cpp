#include "apriltag/Localizer.hpp"
#include "helper/ConfigReader.hpp"
#include "helper/Vector3D.hpp"

Localizer::Localizer(ConfigReader &config, NetworkingClient &client) : config(config), client(client)
{
    
}

void Localizer::addApriltag(Apriltag &tag)
{

}