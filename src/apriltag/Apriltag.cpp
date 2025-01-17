#include "apriltag/Apriltag.hpp"
#include "util/Vector3D.hpp"

using namespace titan;

// Apriltag::Apriltag(int id, Vector3D &position, Vector3D &rotation, double size)
// 	: id(id), position(position), rotation(rotation), size(size)
// {
// 	size = -1;
// }

Apriltag::Apriltag(int id, Translation position, EulerAngles rotation, double size)
	: id(id), position(position), rotation(rotation), size(size)
{
	size = -1;
}

// Buggy operator= which is unnecessary

// Apriltag &Apriltag::operator=(const Apriltag &other)
// {
// 	if (this != &other)
// 	{
// 		id = other.id;
// 		position = other.position;
// 		rotation = other.rotation;
// 		size = other.size;
// 	}

// 	return *this;
// }