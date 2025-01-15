#include <iostream>
#include <string>

#include "../../include/networking/Client.h"
#include "../../include/util/Vector3D.hpp"

using namespace titan;

auto main(int argc, char const *argv[]) -> int
{
	std::string ip = "127.0.0.1";
	int port = 5800;
	Vector3D v(0, 1, 2);

	NetworkingClient client(ip, port);
	client.sendVector("test", v, false);

	std::cout << "Vector " << v.toString() << " sent to " << ip << ":" << port << std::endl;

	return 0;
}