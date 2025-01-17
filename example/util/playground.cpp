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
{
    srand( (unsigned)time(NULL) );
    Translation startpos = {0, 0, 0};
    Transform start = Transform(startpos, EulerAngles(0, 0, 0).toRotationMatrix());
    Vector3D translation = {rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX)};
    Transform transform = Transform(translation, EulerAngles(0, 0, 0).toRotationMatrix());
    Transform end = start * transform;
    Translation endpos = transform.getPosition();
    if (endpos == translation)
    {
        printf("Yay, transform works! (dx = %f, dy = %f, dz = %f)\n", translation.getX(), translation.getY(), translation.getZ());
    } else {
        printf("Transform failed! (dx = %f, dy = %f, dz = %f)\n", translation.getX(), translation.getY(), translation.getZ());
    }
}
    #endif

    // Rotation matrix tests
    #if 1
{
	srand( (unsigned)time(NULL) );
    EulerAngles vec = {rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX), rand() * ((2 * M_PI) / RAND_MAX)};
    RotationMatrix mat = vec.toRotationMatrix();
    EulerAngles vecprime = mat.toEulerAngles();
    if (vec.toRotationQuaternion().isEquivalent(vecprime.toRotationQuaternion()))
    {
        printf("Yay, rotation matrix conversion works! (pitch = %f, yaw = %f, roll = %f)\n", vec.x, vec.y, vec.z);
    } else {
        printf("Rotation matrix conversion failed! (pitch = %f, yaw = %f, roll = %f)\n", vec.x, vec.y, vec.z);
    }
}
    #endif

    printf("=== Rotation matrix pitch/yaw/roll tests === \n");

    // Pitch by 90 degrees, using rotation matrix
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationMatrix startMat = start.toRotationMatrix();
    RotationMatrix transformMat = EulerAngles(M_PI / 2, 0, 0).toRotationMatrix();
    RotationMatrix endMat = startMat * transformMat;
    EulerAngles end = endMat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Pitching Euler angles (0, 0, 0) by 90 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Yaw by 90 degrees, using rotation matrix
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationMatrix startMat = start.toRotationMatrix();
    RotationMatrix transformMat = EulerAngles(0, M_PI / 2, 0).toRotationMatrix();
    RotationMatrix endMat = startMat * transformMat;
    EulerAngles end = endMat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Yawing Euler angles (0, 0, 0) by 90 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Roll by 90 degrees, using rotation matrix
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationMatrix startMat = start.toRotationMatrix();
    RotationMatrix transformMat = EulerAngles(0, 0, M_PI / 2).toRotationMatrix();
    RotationMatrix endMat = startMat * transformMat;
    EulerAngles end = endMat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Rolling Euler angles (0, 0, 0) by 90 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Pitch by 180 degrees, using rotation matrix
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationMatrix startMat = start.toRotationMatrix();
    RotationMatrix transformMat = EulerAngles(M_PI, 0, 0).toRotationMatrix();
    RotationMatrix endMat = startMat * transformMat;
    EulerAngles end = endMat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Pitching Euler angles (0, 0, 0) by 180 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Yaw by 180 degrees, using rotation matrix
    // Note that the answer is (180.000000, 0.000000, 180.000000) due to gimbal lock
    // Gimbal lock also occurs in while pitching and rolling, but our implementation details
    // cause the answer to be as expected.
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationMatrix startMat = start.toRotationMatrix();
    RotationMatrix transformMat = EulerAngles(0, M_PI, 0).toRotationMatrix();
    RotationMatrix endMat = startMat * transformMat;
    EulerAngles end = endMat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Yawing Euler angles (0, 0, 0) by 180 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Roll by 180 degrees, using rotation matrix
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationMatrix startMat = start.toRotationMatrix();
    RotationMatrix transformMat = EulerAngles(0, 0, M_PI).toRotationMatrix();
    RotationMatrix endMat = startMat * transformMat;
    EulerAngles end = endMat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Rolling Euler angles (0, 0, 0) by 180 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    printf("=== Quaternion pitch/yaw/roll tests ===\n");

    // Pitch by 90 degrees, using quaternions
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationQuaternion startQuat = start.toRotationQuaternion();
    RotationQuaternion transformQuat = RotationQuaternion::fromAxisAngle(Vector3D(1, 0, 0), M_PI / 2);
    RotationQuaternion endQuat = startQuat * transformQuat;
    EulerAngles end = endQuat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Pitching Euler angles (0, 0, 0) by 90 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Yaw by 90 degrees, using quaternions
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationQuaternion startQuat = start.toRotationQuaternion();
    RotationQuaternion transformQuat = RotationQuaternion::fromAxisAngle(Vector3D(0, 1, 0), M_PI / 2);
    RotationQuaternion endQuat = startQuat * transformQuat;
    EulerAngles end = endQuat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Yawing Euler angles (0, 0, 0) by 90 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    // Roll by 90 degrees, using quaternions
    #if 1
{
    EulerAngles start = {0, 0, 0};
    RotationQuaternion startQuat = start.toRotationQuaternion();
    RotationQuaternion transformQuat = RotationQuaternion::fromAxisAngle(Vector3D(0, 0, 1), M_PI / 2);
    RotationQuaternion endQuat = startQuat * transformQuat;
    EulerAngles end = endQuat.toEulerAngles();

    constexpr double RADIANS_TO_DEGREES = 180 / M_PI;

    printf("Rolling Euler angles (0, 0, 0) by 90 degrees results in (%f, %f, %f) degrees\n",
        end.x * RADIANS_TO_DEGREES, end.y * RADIANS_TO_DEGREES, end.z * RADIANS_TO_DEGREES);
}
    #endif

    return 0;
}