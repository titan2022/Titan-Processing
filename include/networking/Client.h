#ifndef TRB_CLIENT_H
#define TRB_CLIENT_H

#include <netinet/in.h>
#include <sys/types.h>

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
	auto sendVector(std::string msg, const Vector3D &v, bool withReply) -> Vector3D;
	void sendPose(std::string msg, const Vector3D &pos, const Vector3D &rot);
	void sendTag(std::string msg, int id, const Vector3D &pos, const Vector3D &rot);

	auto sendVectorC(char *msg, TRBVector3D v, bool withReply) -> TRBVector3D;
	void sendPoseC(char *msg, TRBVector3D pos, TRBVector3D rot);
	void sendTagC(char *msg, int id, TRBVector3D pos, TRBVector3D rot);

  private:
	int sock;
	sockaddr_in destination;
};
} // namespace titan

using TRBNetworkingClientRef = titan::NetworkingClient *;
typedef bool _Bool; // C uses _Bool but C++ uses bool
#else
typedef struct TRBNetworkingClient *TRBNetworkingClientRef;
#endif // __cplusplus

TRB_BEGIN_EXTERN_C

TRBVector3D TRBVector3DMake(double x, double y, double z);
TRBNetworkingClientRef TRBNetworkingClientCreate(char *ip, uint16_t port);
void TRBNetworkingClientDestroy(TRBNetworkingClientRef self);
TRBVector3D TRBNetworkingClientSendVector(TRBNetworkingClientRef self, char *msg, TRBVector3D v, _Bool withReply);
void TRBNetworkingClientSendPose(TRBNetworkingClientRef self, char *msg, TRBVector3D pos, TRBVector3D rot);
void TRBNetworkingClientSendTag(TRBNetworkingClientRef self, char *msg, int id, TRBVector3D pos, TRBVector3D rot);

TRB_END_EXTERN_C

#endif