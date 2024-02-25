#ifndef TRB_CLIENT_H
#define TRB_CLIENT_H

#include <string>

#include <netinet/in.h>
#include <sys/types.h>

// All C APIs are prefixed with TRB, which stands for Titan RoBotics

#if __cplusplus
#define TRB_BEGIN_EXTERN_C extern "C" {
#define TRB_END_EXTERN_C }
#else
#define TRB_BEGIN_EXTERN_C
#define TRB_END_EXTERN_C
#endif

TRB_BEGIN_EXTERN_C
typedef struct TRBVector3D {
    double x;  // 8 bytes
    double y;  // 8 bytes
    double z;  // 8 bytes
} TRBVector3D;
TRB_END_EXTERN_C

#if __cplusplus
#include "../helper/Vector3D.hpp"

class NetworkingClient
{
    public:
        NetworkingClient(std::string ip, uint16_t port);
        Vector3D send_vector(std::string msg, Vector3D &v, bool withReply);
        TRBVector3D send_vector_c(char* msg, TRBVector3D v, bool withReply);

    private:
        int sock;
        sockaddr_in destination;
        double bytes_to_double(char *b);
        template <class T>
        T change_endian(T in);
};

typedef NetworkingClient* TRBNetworkingClientRef;
typedef bool _Bool; // C uses _Bool but C++ uses bool
#else
typedef struct TRBNetworkingClient* TRBNetworkingClientRef;
#endif // __cplusplus

TRB_BEGIN_EXTERN_C

TRBVector3D TRBVector3DMake(double x, double y, double z);
TRBNetworkingClientRef TRBNetworkingClientCreate(char* ip, uint16_t port);
TRBVector3D TRBNetworkingClientSendVector(TRBNetworkingClientRef self, char* msg, TRBVector3D v, _Bool withReply);

TRB_END_EXTERN_C


#endif