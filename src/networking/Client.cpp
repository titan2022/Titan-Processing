#include <bitset>
#include <climits>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "util/Vector3D.hpp"
#include "networking/Client.h"

using namespace titan;


const int NAME_LEN = 15;

// Aligned on linux x86 GCC
// TODO: test on ARM
struct VectorData {
  char type;
  char name[NAME_LEN] = {0};
  double x;
  double y;
  double z;
};

struct PoseData {
  char type;
  char name[NAME_LEN] = {0};
  std::array<double, 3> pos;
  std::array<double, 3> rot;
};

struct TagData {
  char type;
  char name[NAME_LEN] = {0};
  std::array<double, 3> pos;
  std::array<double, 3> rot;
  int id;
};

NetworkingClient::NetworkingClient(std::string ip, uint16_t port) {
  sock = ::socket(AF_INET, SOCK_DGRAM, 0);

  destination.sin_family = AF_INET;
  destination.sin_port = htons(port);
  destination.sin_addr.s_addr = inet_addr(ip.c_str());
}

Vector3D NetworkingClient::send_vector(std::string msg, Vector3D &v,
                                       bool withReply) {
  VectorData data;

  data.type = 'v';

  strncpy(data.name, msg.c_str(), NAME_LEN);
  data.x = v.getX();
  data.y = v.getY();
  data.z = v.getZ();

  size_t dataLength = sizeof(data);

  const void *buffer = static_cast<void *>(&data);
  ::sendto(sock, buffer, dataLength, 0,
           reinterpret_cast<sockaddr *>(&destination), sizeof(destination));

  if (withReply) {
    // Untested code
    char replyBuffer[24];
    ::recvfrom(sock, replyBuffer, 24, 0,
               reinterpret_cast<sockaddr *>(&destination),
               (socklen_t *)sizeof(destination));
    Vector3D replyVector(replyBuffer);
    return replyVector;
  }

  return Vector3D();
}

void NetworkingClient::send_pose(std::string msg, Vector3D &pos,
                                 Vector3D &rot) {
  PoseData data;

  data.type = 'p';
  strncpy(data.name, msg.c_str(), NAME_LEN);
  data.pos = pos.toArray();
  data.rot = rot.toArray();

  size_t dataLength = sizeof(data);

  const void *buffer = static_cast<void *>(&data);
  ::sendto(sock, buffer, dataLength, 0,
           reinterpret_cast<sockaddr *>(&destination), sizeof(destination));
}

void NetworkingClient::send_tag(std::string msg, int id, Vector3D &pos,
                                Vector3D &rot) {
  TagData data;

  data.type = 't';
  strncpy(data.name, msg.c_str(), NAME_LEN);
  data.pos = pos.toArray();
  data.rot = rot.toArray();
  data.id = id;

  size_t dataLength = sizeof(data);

  const void *buffer = static_cast<void *>(&data);
  ::sendto(sock, buffer, dataLength, 0,
           reinterpret_cast<sockaddr *>(&destination), sizeof(destination));
}

TRBVector3D TRBVector3DMake(double x, double y, double z) {
  return (TRBVector3D){
      .x = x,
      .y = y,
      .z = z,
  };
}

TRBNetworkingClientRef TRBNetworkingClientCreate(char *ip, uint16_t port) {
  return new NetworkingClient(ip, port);
}

TRBVector3D TRBNetworkingClientSendVector(TRBNetworkingClientRef self,
                                          char *msg, TRBVector3D v,
                                          bool withReply) {
  return self->send_vector_c(msg, v, withReply);
}

TRBVector3D NetworkingClient::send_vector_c(char *msg, TRBVector3D v,
                                            bool withReply) {
  VectorData data;

  data.type = 'v';
  strncpy(data.name, msg, NAME_LEN);
  data.x = v.x;
  data.y = v.y;
  data.z = v.z;

  size_t dataLength = sizeof(data);

  const void *buffer = static_cast<void *>(&data);
  ::sendto(sock, buffer, dataLength, 0,
           reinterpret_cast<sockaddr *>(&destination), sizeof(destination));

  if (withReply) {
    // Untested code
    char replyBuffer[24];
    ::recvfrom(sock, replyBuffer, 24, 0,
               reinterpret_cast<sockaddr *>(&destination),
               (socklen_t *)sizeof(destination));
    TRBVector3D *structVec = reinterpret_cast<TRBVector3D *>(replyBuffer);
    return *structVec;
  }

  return (TRBVector3D){0, 0, 0};
}

std::array<double, 3> trbVectorToArray(TRBVector3D vec) {
  std::array<double, 3> arr;
  arr[0] = vec.x;
  arr[1] = vec.y;
  arr[2] = vec.z;
  return arr;
}

void TRBNetworkingClientSendPose(TRBNetworkingClientRef self, char *msg,
                                 TRBVector3D pos, TRBVector3D rot) {
  self->send_pose_c(msg, pos, rot);
}

void NetworkingClient::send_pose_c(char *msg, TRBVector3D pos,
                                   TRBVector3D rot) {
  PoseData data;

  data.type = 'p';
  strncpy(data.name, msg, NAME_LEN);
  data.pos = trbVectorToArray(pos);
  data.rot = trbVectorToArray(rot);

  size_t dataLength = sizeof(data);

  const void *buffer = static_cast<void *>(&data);
  ::sendto(sock, buffer, dataLength, 0,
           reinterpret_cast<sockaddr *>(&destination), sizeof(destination));
}

void TRBNetworkingClientSendTag(TRBNetworkingClientRef self, char *msg, int id,
                                TRBVector3D pos, TRBVector3D rot) {
  self->send_tag_c(msg, id, pos, rot);
}

void NetworkingClient::send_tag_c(char *msg, int id, TRBVector3D pos,
                                  TRBVector3D rot) {
  TagData data;

  data.type = 't';
  strncpy(data.name, msg, NAME_LEN);
  data.pos = trbVectorToArray(pos);
  data.rot = trbVectorToArray(rot);
  data.id = id;

  size_t dataLength = sizeof(data);

  const void *buffer = static_cast<void *>(&data);
  ::sendto(sock, buffer, dataLength, 0,
           reinterpret_cast<sockaddr *>(&destination), sizeof(destination));
}