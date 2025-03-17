#include <string>
#include <thread>

#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);

	config.writeTagsConverted(TAGS_THREEJS_PATH, CoordinateSystems::THREEjs());
	return 0;
}
