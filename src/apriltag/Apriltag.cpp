#include "apriltag/Apriltag.hpp"
#include "util/Vector3D.hpp"

using namespace titan;

  Apriltag::Apriltag(int id, const Vector3D &position, const Vector3D &rotation, double size)
	: position(position), rotation(rotation), size(size), id(id)
{
}
