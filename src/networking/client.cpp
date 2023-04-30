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

#include "networking/client.hpp"
#include "helper/vector.hpp"

NetworkingClient::NetworkingClient(std::string ip, uint16_t port)
{
    sock = ::socket(AF_INET, SOCK_DGRAM, 0);

    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(ip.c_str());
}

double *NetworkingClient::send_data(std::string msg, bool withReply, Vector3D &v)
{
    char vectorBuffer[24];

    for (uint8_t i = 0; i < 3; i++)
    {
        char *doubleBytes = double_to_bytes(v[i]);
        memcpy(vectorBuffer + i * 8, doubleBytes, 8);
    }

    size_t msgSize = msg.length();
    char nameBuffer[msgSize];
    strcpy(nameBuffer, msg.c_str());

    char dataBuffer[24 + msgSize];

    memcpy(dataBuffer, vectorBuffer, 24);
    memcpy(dataBuffer + 24, nameBuffer, msgSize);

    ::sendto(sock, dataBuffer, 24 + msgSize, 0, reinterpret_cast<sockaddr *>(&destination), sizeof(destination));

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

char *NetworkingClient::double_to_bytes(double d)
{
    double reversed = this->change_endian(d);
    char *bytes = (char *)&reversed;
    return bytes;
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