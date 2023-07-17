#include <iostream>
#include <string>

#include "../../include/networking/Client.hpp"
#include "../../include/helper/Vector3D.hpp"

int main(int argc, char const *argv[])
{
    std::string ip = "127.0.0.1";
    int port = 5800;
    Vector3D v(0, 1, 2);

    NetworkingClient client(ip, port);
    client.send_data("test", false, v);
    
    std::cout << "Vector " << v.toString() << " sent to " << ip << ":" << port << std::endl;

    return 0;
}