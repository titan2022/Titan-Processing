#ifndef CLIENT
#define CLIENT

#include <string>

#include <netinet/in.h>

#include "../helper/Vector3D.hpp"

class NetworkingClient
{
    public:
        NetworkingClient(std::string ip, uint16_t port);
        Vector3D send_vector(std::string msg, Vector3D &v, bool withReply);
        void send_pose(std::string msg, Vector3D &pos, Vector3D &rot);
        void send_tag(std::string msg, int id, Vector3D &pos, Vector3D &rot);

    private:
        int sock;
        sockaddr_in destination;
};

#endif
