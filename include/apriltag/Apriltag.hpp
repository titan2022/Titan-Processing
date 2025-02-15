#ifndef APRILTAG
#define APRILTAG

#include "../util/Vector3D.hpp"

namespace titan
{
class Apriltag
{
  public:
	// Apriltag(int id, Vector3D &position, Vector3D &rotation, double size);
	Apriltag(int id, Translation3d position, Rotation3d rotation, double size);
	// Apriltag &operator=(const Apriltag &other); // buggy and unnecessary
	Translation3d position;
	Rotation3d rotation;
	double size;
	int id;
};
} // namespace titan

#endif