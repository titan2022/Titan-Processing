#include <algorithm>
#include <iostream>
#include <math.h>
#include <string>
#include <thread>

#include "../../include/apriltag/Apriltag.hpp"
#include "../../include/util/Vector3D.hpp"

using namespace titan;

int main(int argc, char const *argv[])
{
	Vector3D invTagPos(3, 4, 1);
	Vector3D invTagRot(0, 0, M_PI / 4.0);

	Apriltag invTag(0, invTagPos, invTagRot, 0);
	Vector3D camPos(0, 0, -2);

	invTag.position += camPos.getRotated(-invTag.rotation.getX(), -invTag.rotation.getY(), -invTag.rotation.getZ());

	std::cout << invTag.position.toString() << std::endl;

	return 0;
}