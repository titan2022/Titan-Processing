#include <cmath>

#include "../../include/util/Vector3D.hpp"

using namespace titan;

/**
 * Stars streams with every camera available
 */
int main(int argc, char const *argv[])
{
	srand( (unsigned)time(NULL) );
    Vector3D vec = {rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX)};
    cv::Mat mat = vec.toRotationMatrix();
    Vector3D vecprime = Vector3D::fromRotationMatrix(mat);
    if(vec.toQuaternion().isEquivalent(vecprime.toQuaternion()))
    {
        printf("Yay, rotation matrix conversion works! (pitch = %f, yaw = %f, roll = %f)\n", vec.getX(), vec.getY(), vec.getZ());
    } else {
        printf("Rotation matrix conversion failed! (pitch = %f, yaw = %f, roll = %f)\n", vec.getX(), vec.getY(), vec.getZ());
    }
    return 0;
}