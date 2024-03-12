#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <string>
#include <math.h>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/helper/Vector3D.hpp"

int main(int argc, char const *argv[])
{
    Vector3D invTagPos(3, 4, 0);
    Vector3D invTagRot(0, 0, 3.0 / 4.0 * M_PI);

    Apriltag invTag(0, invTagPos, invTagRot, 1);
    Vector3D camPos(0, 2, 0);
    Vector3D camRot(0, 0, 0);

    invTag.position += camPos.getRotated(-invTag.rotation.getX(), -invTag.rotation.getY(), -invTag.rotation.getZ());

    std::cout << invTag.position.toString() << std::endl;

    return 0;
}