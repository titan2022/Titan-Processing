from ctypes import (
    CDLL,
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

    def __repr__(self):
        return f"<{self.x}, {self.y}, {self.z}>"


def load_lib(pathToLibTitanProcessing = "lib/libTitanProcessing.so") -> CDLL:
    "Loads the `libTitanProcessing` library and sets functions signatures"
    lib = cdll.LoadLibrary(str(pathToLibTitanProcessing))

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

class Client:
    def __init__(self, lib: CDLL, ip, port):
        self.lib = lib
        self.raw_client = lib.TRBNetworkingClientCreate(ip, port)
    
    def sendVector(self, msg, v, withReply=False) -> Vector3D:
        return self.lib.TRBNetworkingClientSendVector(self.raw_client, msg, v, withReply)
    
    def sendPose(self, msg, pos, rot):
        return self.lib.TRBNetworkingClientSendPose(self.raw_client, msg, pos, rot)
    
    def sendTag(self, msg, id, pos, rot):
        return self.lib.TRBNetworkingClientSendTag(self.raw_client, msg, id, pos, rot)
    
    def __repr__(self):
        return f"<TRBNetworkingClient connected to {self.ip}:{self.port}>"