#ifndef TRB_CLIENT_H
#define TRB_CLIENT_H

#include <netinet/in.h>
#include <sys/types.h>
#include <stdbool.h>

// All C APIs are prefixed with TRB, which stands for Titan RoBotics

#if __cplusplus
#define TRB_BEGIN_EXTERN_C                                                                                             \
	extern "C"                                                                                                         \
	{
#define TRB_END_EXTERN_C }
#else
#define TRB_BEGIN_EXTERN_C
#define TRB_END_EXTERN_C
#endif

TRB_BEGIN_EXTERN_C
typedef struct TRBVector3D
{
	double x; // 8 bytes
	double y; // 8 bytes
	double z; // 8 bytes
} TRBVector3D;
TRB_END_EXTERN_C

#if __cplusplus
#include "../util/Vector3D.hpp"
#include <string>

namespace titan
{
class NetworkingClient
{
  public:
	NetworkingClient(std::string ip, uint16_t port);
	Vector3D send_vector(std::string msg, Vector3D &v, bool withReply);
	void send_pose(std::string msg, Vector3D &pos, Vector3D &rot);
	void send_tag(std::string msg, int id, Vector3D &pos, Vector3D &rot);

	TRBVector3D send_vector_c(char *msg, TRBVector3D v, bool withReply);
	void send_pose_c(char *msg, TRBVector3D pos, TRBVector3D rot);
	void send_tag_c(char *msg, int id, TRBVector3D pos, TRBVector3D rot);

  private:
	int sock;
	sockaddr_in destination;
};
} // namespace titan

typedef titan::NetworkingClient *TRBNetworkingClientRef;
#else
typedef struct TRBNetworkingClient *TRBNetworkingClientRef;
#endif // __cplusplus

TRB_BEGIN_EXTERN_C

TRBVector3D TRBVector3DMake(double x, double y, double z);
TRBNetworkingClientRef TRBNetworkingClientCreate(char *ip, uint16_t port);
TRBVector3D TRBNetworkingClientSendVector(TRBNetworkingClientRef self, char *msg, TRBVector3D v, bool withReply);
void TRBNetworkingClientSendPose(TRBNetworkingClientRef self, char *msg, TRBVector3D pos, TRBVector3D rot);
void TRBNetworkingClientSendTag(TRBNetworkingClientRef self, char *msg, int id, TRBVector3D pos, TRBVector3D rot);

TRB_END_EXTERN_C

#endif