#define _USE_MATH_DEFINES
#include <math.h>
#include <gtest/gtest.h>

#include "../../include/util/Vector3D.hpp"


TEST(ConfigTest, IOTest) {
    Vector3D v1(-1.5, 0.0, 1.0);
    ASSERT_DOUBLE_EQ(v1.getX(), -1.5);
    ASSERT_DOUBLE_EQ(v1.getY(), 0.0);
    ASSERT_DOUBLE_EQ(v1.getZ(), 1.0);

    cv::Vec3d cvVec(2, 3, 4);
    Vector3D v2(cvVec);
    ASSERT_DOUBLE_EQ(v2.getX(), 2);
    ASSERT_DOUBLE_EQ(v2.getY(), 3);
    ASSERT_DOUBLE_EQ(v2.getZ(), 4);

    std::vector<double> vecVec = {5, 6, 7};
    Vector3D v3(vecVec);
    ASSERT_DOUBLE_EQ(v3.getX(), 5);
    ASSERT_DOUBLE_EQ(v3.getY(), 6);
    ASSERT_DOUBLE_EQ(v3.getZ(), 7);

    double arrVec[3] = {8, 9, 10};
    Vector3D v4(arrVec);
    ASSERT_DOUBLE_EQ(v4.getX(), 8);
    ASSERT_DOUBLE_EQ(v4.getY(), 9);
    ASSERT_DOUBLE_EQ(v4.getZ(), 10);

    // TODO: find a way to express in bytes
    // const char* charVec = ""; // 11, 12, 13
    // Vector3D v5(charVec);
    // ASSERT_DOUBLE_EQ(v5.getX(), 11);
    // ASSERT_DOUBLE_EQ(v5.getY(), 12);
    // ASSERT_DOUBLE_EQ(v5.getZ(), 13);

    // TODO: rotation tests
    Vector3D rotatedV1 = v1.getRotatedX(M_PI * 0.5); // 90 deg
    ASSERT_DOUBLE_EQ(v1.getX(), rotatedV1.getX());
    // ASSERT_DOUBLE_EQ(v1.getX(), rotatedV1.getX());
}