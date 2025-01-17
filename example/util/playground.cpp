#include <cmath>

#include "../../include/util/Vector3D.hpp"

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
    // Transform tests
    #if 1
    srand( (unsigned)time(NULL) );
    Translation startpos = {0, 0, 0};
    Transform start = Transform(startpos, EulerAngles(0, 0, 0).toRotationMatrix());
    Vector3D translation = {rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX)};
    Transform transform = Transform(translation, EulerAngles(0, 0, 0).toRotationMatrix());
    Transform end = start * transform;
    Translation endpos = transform.getPosition();
    if(endpos == translation)
    {
        printf("Yay, transform works! (dx = %f, dy = %f, dz = %f)\n", translation.getX(), translation.getY(), translation.getZ());
    } else {
        printf("Transform failed! (dx = %f, dy = %f, dz = %f)\n", translation.getX(), translation.getY(), translation.getZ());
    }
    #endif

    // Rotation matrix tests
    #if 1
	srand( (unsigned)time(NULL) );
    EulerAngles vec = {rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX)};
    RotationMatrix mat = vec.toRotationMatrix();
    EulerAngles vecprime = mat.toEulerAngles();
    if(vec.toRotationQuaternion().isEquivalent(vecprime.toRotationQuaternion()))
    {
        printf("Yay, rotation matrix conversion works! (pitch = %f, yaw = %f, roll = %f)\n", vec.x, vec.y, vec.z);
    } else {
        printf("Rotation matrix conversion failed! (pitch = %f, yaw = %f, roll = %f)\n", vec.x, vec.y, vec.z);
    }
    #endif
    return 0;
}