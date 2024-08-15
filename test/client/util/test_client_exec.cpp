#include "networking/Client.h"
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
	std::string ip = "127.0.0.1";
	int port = 5801;
	NetworkingClient client(ip, port);

	Vector3D v0(0, 1, 2);
	Vector3D v1(3, 4, 5);
	Vector3D v2(6, 7, 8);

	for (int i = 0; i < 10; i++)
	{
		client.send_vector("vec", v0, false);
		client.send_pose("pose", v0, v0);
		client.send_tag("tag", 2, v1, v2);
	}

	return 0;
}
