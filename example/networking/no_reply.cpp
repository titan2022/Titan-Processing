#include <iostream>
#include <string>

#include "../../include/networking/Client.h"
#include "../../include/util/Vector3D.hpp"

using namespace titan;

auto main(int argc, char const *argv[]) -> int
{
	std::string ip = "127.0.0.1";
	int port = 5800;

	NetworkingClient client(ip, port);

	Vector3D v0(0, 1, -2);
	client.sendVector("position", v0, false);

	Vector3D v1(1, 3, -2);
	Vector3D v2(5, -1, 9);
	client.sendPose("pose", v1, v2);

	Vector3D v3(4, 5, 6);
	Vector3D v4(7, 8, 9);
	client.sendTag("tag", 4, v3, v4);

	std::cout << "Things sent to " << ip << ":" << port << std::endl;

	return 0;
}