#include "networking/Client.h"
#include <string>

using namespace titan;

auto main(int argc, char const *argv[]) -> int
{
	std::string ip = "127.0.0.1";
	int port = 5801;
	NetworkingClient client(ip, port);

	Vector3D v0(0, 1, 2);
	Vector3D v1(3, 4, 5);
	Vector3D v2(6, 7, 8);

	for (int i = 0; i < 10; i++)
	{
		client.sendVector("vec", v0, false);
		client.sendPose("pose", v0, v0);
		client.sendTag("tag", 2, v1, v2);
	}

	return 0;
}
