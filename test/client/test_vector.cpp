#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include <math.h>

#include "../../include/util/Vector3D.hpp"

using namespace titan;

TEST(VectorTest, DoubleConstructor)
{
	Vector3D v(-1.5, 0.0, 1.0);
	ASSERT_DOUBLE_EQ(v.getX(), -1.5);
	ASSERT_DOUBLE_EQ(v.getY(), 0.0);
	ASSERT_DOUBLE_EQ(v.getZ(), 1.0);
}

TEST(VectorTest, Vec3dConstructor)
{
	cv::Vec3d cvVec(2, 3, 4);
	Vector3D v(cvVec);
	ASSERT_DOUBLE_EQ(v.getX(), 2);
	ASSERT_DOUBLE_EQ(v.getY(), 3);
	ASSERT_DOUBLE_EQ(v.getZ(), 4);
}

TEST(VectorTest, VectorConstructor)
{
	std::vector<double> vecVec = {5, 6, 7};
	Vector3D v(vecVec);
	ASSERT_DOUBLE_EQ(v.getX(), 5);
	ASSERT_DOUBLE_EQ(v.getY(), 6);
	ASSERT_DOUBLE_EQ(v.getZ(), 7);
}

TEST(VectorTest, ArrayConstructor)
{
	double arrVec[3] = {8, 9, 10};
	Vector3D v(arrVec);
	ASSERT_DOUBLE_EQ(v.getX(), 8);
	ASSERT_DOUBLE_EQ(v.getY(), 9);
	ASSERT_DOUBLE_EQ(v.getZ(), 10);
}

TEST(VectorTest, CharConstructor)
{
	// TODO: find a way to express in bytes
	// const char* charVec = ""; // 11, 12, 13
	// Vector3D v5(charVec);
	// ASSERT_DOUBLE_EQ(v5.getX(), 11);
	// ASSERT_DOUBLE_EQ(v5.getY(), 12);
	// ASSERT_DOUBLE_EQ(v5.getZ(), 13);
}

// TODO: rotation tests
TEST(VectorTest, Rotation)
{
	Vector3D v(-1.5, 0.0, 1.0);
	Vector3D rotatedV = v.getRotatedX(M_PI * 0.5); // 90 deg
	ASSERT_DOUBLE_EQ(v.getX(), rotatedV.getX());
	// ASSERT_DOUBLE_EQ(v1.getX(), rotatedV1.getX());
}

// TODO: test quaternion to euler angles conversion 
TEST(VectorTest, QuaternionTest)
{

}

TEST(VectorTest, RotationMatrixTest)
{
	
}
