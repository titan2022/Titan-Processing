#ifndef APRILTAG
#define APRILTAG

#include "../util/Vector3D.hpp"

namespace titan
{
class Apriltag
{
  public:
	Apriltag(int id, const Vector3D &position, const Vector3D &rotation, double size);
	Vector3D position;
	Vector3D rotation;
	double size;
	int id;
};
} // namespace titan

#endif