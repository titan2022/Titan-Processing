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
    Vector3D startpos = {0, 0, 0};
    cv::Mat start = Vector3D::makeTransform(startpos, {0, 0, 0});
    Vector3D translation = {rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX)};
    cv::Mat transform = Vector3D::makeTransform(translation, {0, 0, 0});
    cv::Mat end = start * transform;
    Vector3D endpos = Vector3D::positionFromTransform(transform);
    if(endpos == translation)
    {
        printf("Yay, transform works! (dx = %f, dy = %f, dz = %f)\n", translation.getX(), translation.getY(), translation.getZ());
    } else {
        printf("Transform failed! (dx = %f, dy = %f, dz = %f)\n", translation.getX(), translation.getY(), translation.getZ());
    }
    #endif

    // Rotation matrix tests
    #if 0
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
    #endif
    return 0;
}