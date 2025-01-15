#include <array>
#include <climits>
#include <cstring>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "networking/Client.h"
#include "util/Vector3D.hpp"

using namespace titan;

const int NAME_LEN = 15;

// Aligned on linux x86 GCC
// TODO: test on ARM
struct VectorData
{
	char type{};
	std::array<char, NAME_LEN> name{};
	double x{};
	double y{};
	double z{};
};

struct PoseData
{
	char type{};
	std::array<char, NAME_LEN> name{};
	std::array<double, 3> pos{};
	std::array<double, 3> rot{};
};

struct TagData
{
	char type{};
	std::array<char, NAME_LEN> name{};
	std::array<double, 3> pos{};
	std::array<double, 3> rot{};
	int id{};
};

NetworkingClient::NetworkingClient(std::string ip, uint16_t port) : sock(::socket(AF_INET, SOCK_DGRAM, 0))
{
	struct sockaddr_in dest{};
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr = {inet_addr(ip.c_str())};
	destination = dest;
}

auto NetworkingClient::sendVector(std::string msg, const Vector3D &v, bool withReply) -> Vector3D
{
	VectorData data;

	data.type = 'v';

	msg.copy(data.name.begin(), NAME_LEN);
	data.x = v.getX();
	data.y = v.getY();
	data.z = v.getZ();

	size_t dataLength = sizeof(data);

	const void *buffer = static_cast<void *>(&data);
	sendto(sock, buffer, dataLength, 0, reinterpret_cast<const sockaddr *>(&destination), sizeof(destination));

	if (withReply)
	{
		// Untested code
		TRBVector3D replyVec{};
		recvfrom(sock, &replyVec, sizeof(replyVec), 0, nullptr, nullptr);
		return replyVec;
	}

	return {};
}

void NetworkingClient::sendPose(std::string msg, const Vector3D &pos, const Vector3D &rot)
{
	PoseData data;

	data.type = 'p';
	msg.copy(data.name.begin(), NAME_LEN);
	data.pos = pos.toArray();
	data.rot = rot.toArray();

	sendto(sock, &data, sizeof(data), 0, reinterpret_cast<const sockaddr *>(&destination), sizeof(destination));
}

void NetworkingClient::sendTag(std::string msg, int id, const Vector3D &pos, const Vector3D &rot)
{
	TagData data;

	data.type = 't';
	msg.copy(data.name.begin(), NAME_LEN);
	data.pos = pos.toArray();
	data.rot = rot.toArray();
	data.id = id;

	sendto(sock, &data, sizeof(data), 0, reinterpret_cast<const sockaddr *>(&destination), sizeof(destination));
}

auto TRBVector3DMake(double x, double y, double z) -> TRBVector3D
{
	return (TRBVector3D){
		.x = x,
		.y = y,
		.z = z,
	};
}

auto TRBNetworkingClientCreate(char *ip, uint16_t port) -> TRBNetworkingClientRef
{
	return new NetworkingClient(ip, port);
}

auto TRBNetworkingClientSendVector(TRBNetworkingClientRef self, char *msg, TRBVector3D v, bool withReply) -> TRBVector3D
{
	return self->sendVectorC(msg, v, withReply);
}

auto NetworkingClient::sendVectorC(char *msg, TRBVector3D v, bool withReply) -> TRBVector3D
{
	return sendVector(std::string(msg), Vector3D(v), withReply).toFfi();
}

void TRBNetworkingClientSendPose(TRBNetworkingClientRef self, char *msg, TRBVector3D pos, TRBVector3D rot)
{
	self->sendPoseC(msg, pos, rot);
}

void NetworkingClient::sendPoseC(char *msg, TRBVector3D pos, TRBVector3D rot)
{
	sendPose(std::string(msg), pos, rot);
}

void TRBNetworkingClientSendTag(TRBNetworkingClientRef self, char *msg, int id, TRBVector3D pos, TRBVector3D rot)
{
	self->sendTagC(msg, id, pos, rot);
}

void NetworkingClient::sendTagC(char *msg, int id, TRBVector3D pos, TRBVector3D rot)
{
	sendTag(std::string(msg), id, pos, rot);
}