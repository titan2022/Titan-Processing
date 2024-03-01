#include "../../include/networking/Client.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  char *ip = (char *)"127.0.0.1";
  int port = 5800;
  TRBVector3D v = (TRBVector3D){0, 1, 2};

  TRBNetworkingClientRef client = TRBNetworkingClientCreate(ip, port);
  TRBVector3D reply =
      TRBNetworkingClientSendVector(client, (char *)"test", v, true);

  printf("Vector <%f, %f, %f> sent to %s:%d, reply is <%f, %f, %f>\n", v.x, v.y,
         v.z, ip, port, reply.x, reply.y, reply.z);

  return 0;
}