#ifndef CLIENT
#define CLIENT

#include <string>

#include <netinet/in.h>

#include "../helper/vector.hpp"

class NetworkingClient
{
    public:
        NetworkingClient(std::string ip, uint16_t port);
        double *send_data(std::string msg, bool withReply, Vector3D &v);

    private:
        int sock;
        sockaddr_in destination;
        char *double_to_bytes(double d);
        double bytes_to_double(char *b);
        template <class T>
        T change_endian(T in);
};

#endif