from ctypes import (
    Structure,
    c_bool,
    c_char_p,
    c_double,
    c_int,
    c_uint16,
    c_void_p,
    cdll,
)


class Vector3D(Structure):
    _fields_ = [
        ("x", c_double),
        ("y", c_double),
        ("z", c_double),
    ]


def load_lib():
    "Loads the `libTitanProcessing` library and sets the signatures of functions"
    lib = cdll.LoadLibrary("lib/libTitanProcessing.so")

    lib.TRBVector3DMake.argtypes = [c_double, c_double, c_double]
    lib.TRBVector3DMake.restype = Vector3D

    lib.TRBNetworkingClientCreate.argtypes = [c_char_p, c_uint16]
    lib.TRBNetworkingClientCreate.restype = c_void_p

    lib.TRBNetworkingClientSendVector.argtypes = [c_void_p, c_char_p, Vector3D, c_bool]
    lib.TRBNetworkingClientSendVector.restype = Vector3D

    lib.TRBNetworkingClientSendPose.argtypes = [c_void_p, c_char_p, Vector3D, Vector3D]
    lib.TRBNetworkingClientSendPose.restype = None

    lib.TRBNetworkingClientSendTag.argtypes = [
        c_void_p,
        c_char_p,
        c_int,
        Vector3D,
        Vector3D,
    ]
    lib.TRBNetworkingClientSendTag.restype = None

    return lib


if __name__ == "__main__":
    lib = load_lib()
    ip = b"127.0.0.1"
    port = 5800
    client_ptr = lib.TRBNetworkingClientCreate(ip, port)

    v0 = Vector3D(0, 1, -2)
    lib.TRBNetworkingClientSendVector(client_ptr, b"position", v0, False)

    v1 = Vector3D(1, 3, -2)
    v2 = Vector3D(5, -1, 9)
    lib.TRBNetworkingClientSendPose(client_ptr, b"pose", v1, v2)

    v3 = Vector3D(4, 5, 6)
    v4 = Vector3D(7, 8, 9)
    lib.TRBNetworkingClientSendTag(client_ptr, b"tag", 4, v3, v4)
