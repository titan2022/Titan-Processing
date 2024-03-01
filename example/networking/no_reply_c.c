#include "../../include/networking/Client.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  char *ip = (char *)"127.0.0.1";
  int port = 5800;

  TRBNetworkingClientRef client = TRBNetworkingClientCreate(ip, port);

  TRBVector3D v0 = (TRBVector3D){0, 1, -2};
  TRBNetworkingClientSendVector(client, (char *)"position", v0, false);

  TRBVector3D v1 = (TRBVector3D){1, 3, -2};
  TRBVector3D v2 = (TRBVector3D){5, -1, 9};
  TRBNetworkingClientSendPose(client, (char *)"pose", v1, v2);

  TRBVector3D v3 = (TRBVector3D){4, 5, 6};
  TRBVector3D v4 = (TRBVector3D){7, 8, 9};
  TRBNetworkingClientSendTag(client, (char *)"tag", 4, v3, v4);

  return 0;
}