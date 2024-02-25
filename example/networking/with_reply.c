#include <stdio.h>
#include "../../include/networking/Client.h"

constexpr _Bool NO = 0;
constexpr _Bool YES = 1;

int main(int argc, char const *argv[])
{
    char* ip = "127.0.0.1";
    int port = 5800;
    TRBVector3D v = (TRBVector3D){0, 1, 2};

    TRBNetworkingClientRef client = TRBNetworkingClientCreate(ip, port);
    TRBNetworkingClientSendVector(client, "test", v, YES);
    
    printf("Vector <%f, %f, %f> sent to %s:%d\n", v.x, v.y, v.z, ip, port);

    return 0;
}