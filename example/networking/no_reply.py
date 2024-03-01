from common import Vector3D, load_lib


if __name__ == "__main__":
    lib = load_lib()
    ip = b"127.0.0.1"
    port = 5800
    client = lib.TRBNetworkingClientCreate(ip, port)

    v0 = Vector3D(0, 1, -2)
    lib.TRBNetworkingClientSendVector(client, b"position", v0, False)

    v1 = Vector3D(1, 3, -2)
    v2 = Vector3D(5, -1, 9)
    lib.TRBNetworkingClientSendPose(client, b"pose", v1, v2)

    v3 = Vector3D(4, 5, 6)
    v4 = Vector3D(7, 8, 9)
    lib.TRBNetworkingClientSendTag(client, b"tag", 4, v3, v4)
