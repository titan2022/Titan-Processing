from ctypes import Structure, c_bool, c_char_p, c_double, cdll


class Vector3D(Structure):
    _fields_ = [
        ("x", c_double),
        ("y", c_double),
        ("z", c_double),
    ]


if __name__ == "__main__":
    lib = cdll.LoadLibrary("lib/libTitanProcessing.so")

    ip = c_char_p(b"127.0.0.1")
    port = 5800
    client_ptr = lib.TRBNetworkingClientCreate(ip, port)
    print(client_ptr)

    v0 = Vector3D(0, 1, -2)

    lib.TRBNetworkingClientSendVector(client_ptr, "position", v0, c_bool(False))

    v1 = Vector3D(1, 3, -2)
    v2 = Vector3D(5, -1, 9)
    lib.TRBNetworkingClientSendPose(client_ptr, "pose", v1, v2)

    v3 = Vector3D(4, 5, 6)
    v4 = Vector3D(7, 8, 9)
    lib.TRBNetworkingClientSendTag(client_ptr, "tag", 4, v3, v4)
