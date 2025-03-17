#include <string>
#include <thread>

#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "networking/Client.h"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"

#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTable.h>
#include <networktables/DoubleTopic.h>

using namespace titan;
using namespace nt;

using frc::Pose3d;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);
	NetworkingClient client(config.udp_roborio_ip, config.udp_port);
	NetworkingClient dashboardClient(config.udp_dashboard_ip, config.udp_port);

	nt::NetworkTableInstance ntInstance = nt::NetworkTableInstance::GetDefault();
	// start a NT4 client
	ntInstance.StartClient4("Titan-Processing");
	// connect to a specific host/port
	ntInstance.SetServer(config.nt_server_ip, config.nt_port);

	// Get the table
	std::shared_ptr<nt::NetworkTable> networkTable = ntInstance.GetTable("TitanProcessing");
	// get a topic from a NetworkTable
	// the topic name in this case is the name within the table;
	// this line and the one above reference the same topic
	nt::DoubleTopic doubleTopic = networkTable->GetDoubleTopic("time");
	nt::DoublePublisher doublePublisher = doubleTopic.Publish();

	while(true) {
        sleep(1);
        doublePublisher.Set(time(NULL));
    }

	return 0;
}
