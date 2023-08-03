#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <bitset>
#include <climits>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "networking/Client.hpp"
#include "helper/Vector3D.hpp"

// Aligned on linux x86 GCC
// TODO: test on ARM
struct VectorData
{
    char type; // 8 bytes (1 + 7)
    double x;  // 8 bytes
    double y;  // 8 bytes
    double z;  // 8 bytes
    std::string name;
};

NetworkingClient::NetworkingClient(std::string ip, uint16_t port)
{
    sock = ::socket(AF_INET, SOCK_DGRAM, 0);

    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(ip.c_str());
}

double *NetworkingClient::send_vector(std::string msg, bool withReply, Vector3D &v)
{
    VectorData data;

    data.type = 'd';
    data.x = v.getX();
    data.y = v.getY();
    data.z = v.getZ();
    data.name = msg + '\n';

    const void* buffer = static_cast<void*>(&data);
    ::sendto(sock, buffer, sizeof(data), 0, reinterpret_cast<sockaddr *>(&destination), sizeof(destination));

    if (withReply)
    {
        char replyBuffer[1024];
        ::recvfrom(sock, replyBuffer, 1024, 0, reinterpret_cast<sockaddr *>(&destination), (socklen_t *)sizeof(destination));

        double replyVector[3];

        for (uint8_t i = 0; i < 3; i++)
        {
            char vectorBuffer[8];
            memcpy(vectorBuffer + i * 8, replyBuffer, 8); // TODO: transition away from C style memory functions...
            replyVector[i] = bytes_to_double(vectorBuffer);
        }

        return replyVector;
    }

    return NULL;
}

double NetworkingClient::bytes_to_double(char *b)
{
    double d;
    memcpy(&d, b, sizeof(double));
    return this->change_endian(d);
}

template <class T>
T NetworkingClient::change_endian(T in)
{
    char *const p = reinterpret_cast<char *>(&in);
    for (size_t i = 0; i < sizeof(T) / 2; ++i)
        std::swap(p[i], p[sizeof(T) - i - 1]);
    return in;
}