#include <stdio.h>
#include "../../include/networking/Client.h"

constexpr _Bool NO = 0;
constexpr _Bool YES = 1;

int main(int argc, char const *argv[])
{
    char* ip = "127.0.0.1";
    int port = 5800;

    TRBNetworkingClientRef client = TRBNetworkingClientCreate(ip, port);

    TRBVector3D v0 = (TRBVector3D){0, 1, -2};
    TRBNetworkingClientSendVector(client, "position", v0, NO);

    TRBVector3D v1 = (TRBVector3D){1, 3, -2};
    TRBVector3D v2 = (TRBVector3D){5, -1, 9};
    TRBNetworkingClientSendPose(client, "pose", v1, v2);
    
    TRBVector3D v1 = (TRBVector3D){4, 5, 6};
    TRBVector3D v2 = (TRBVector3D){7, 8, 9};
    TRBNetworkingClientSendTag(client, "tag", 4, v3, v4);

    std::cout << "Things sent to " << ip << ":" << port << std::endl;

    return 0;
}