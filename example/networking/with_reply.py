from TRBNetworking import Vector3D, load_lib

if __name__ == "__main__":
    lib = load_lib()

    ip = b"127.0.0.1"
    port = 5800
    v = Vector3D(0, 1, 2)

    client = lib.TRBNetworkingClientCreate(ip, port)
    reply = lib.TRBNetworkingClientSendVector(client, b"test", v, True)

    print(
        f"Vector <{v.x}, {v.y}, {v.z}> sent to {ip}:{port}, reply is <{reply.x}, {reply.y}, {reply.z}>s"
    )
