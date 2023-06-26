#include "./NetworkingClient.hpp"
#include <iostream>

int NetworkingClientTest()
{
    NetworkingClient client("172.21.0.1", 3002);

    double testVector[3] = {10, 20.34, -30};

    for (int i = 0; i < 10; i++)
    {
        client.sendData("cone", false, testVector);
    }

    double* orientation = client.sendData("robot", true, testVector);
    std::cout << "orientation: " << orientation[0] << ", " << orientation[1] << ", " << orientation[2] << std::endl;

    return 0;
}
