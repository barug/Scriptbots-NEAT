#include <gtest/gtest.h>
#include "vmath.h"
#include <cmath>

class VmathTest : public ::testing::Test {
protected:
    const float EPSILON = 1e-5f;
};

// Vector2f construction and basic operations
TEST_F(VmathTest, DefaultConstructor) {
    Vector2f v;
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST_F(VmathTest, ParameterizedConstructor) {
    Vector2f v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.x, 3.0f);
    EXPECT_FLOAT_EQ(v.y, 4.0f);
}

TEST_F(VmathTest, CopyConstructor) {
    Vector2f v1(3.0f, 4.0f);
    Vector2f v2(v1);
    EXPECT_FLOAT_EQ(v2.x, 3.0f);
    EXPECT_FLOAT_EQ(v2.y, 4.0f);
}

// Length calculations
TEST_F(VmathTest, LengthZeroVector) {
    Vector2f v(0.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.length(), 0.0f);
}

TEST_F(VmathTest, LengthUnitVectors) {
    Vector2f v1(1.0f, 0.0f);
    Vector2f v2(0.0f, 1.0f);
    EXPECT_FLOAT_EQ(v1.length(), 1.0f);
    EXPECT_FLOAT_EQ(v2.length(), 1.0f);
}

TEST_F(VmathTest, Length345Triangle) {
    Vector2f v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.length(), 5.0f);
}

TEST_F(VmathTest, LengthNegativeComponents) {
    Vector2f v(-3.0f, -4.0f);
    EXPECT_FLOAT_EQ(v.length(), 5.0f);
}

// Vector addition
TEST_F(VmathTest, AdditionOperator) {
    Vector2f v1(1.0f, 2.0f);
    Vector2f v2(3.0f, 4.0f);
    Vector2f result = v1 + v2;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
}

TEST_F(VmathTest, AdditionWithZero) {
    Vector2f v1(5.0f, 7.0f);
    Vector2f v2(0.0f, 0.0f);
    Vector2f result = v1 + v2;
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 7.0f);
}

// Vector subtraction
TEST_F(VmathTest, SubtractionOperator) {
    Vector2f v1(5.0f, 7.0f);
    Vector2f v2(2.0f, 3.0f);
    Vector2f result = v1 - v2;
    EXPECT_FLOAT_EQ(result.x, 3.0f);
    EXPECT_FLOAT_EQ(result.y, 4.0f);
}

TEST_F(VmathTest, SubtractionFromSelf) {
    Vector2f v(5.0f, 7.0f);
    Vector2f result = v - v;
    EXPECT_FLOAT_EQ(result.x, 0.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
}

// Scalar multiplication
TEST_F(VmathTest, ScalarMultiplication) {
    Vector2f v(2.0f, 3.0f);
    Vector2f result = v * 2.0f;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
}

TEST_F(VmathTest, ScalarMultiplicationByZero) {
    Vector2f v(5.0f, 7.0f);
    Vector2f result = v * 0.0f;
    EXPECT_FLOAT_EQ(result.x, 0.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
}

TEST_F(VmathTest, ScalarMultiplicationNegative) {
    Vector2f v(2.0f, 3.0f);
    Vector2f result = v * -1.0f;
    EXPECT_FLOAT_EQ(result.x, -2.0f);
    EXPECT_FLOAT_EQ(result.y, -3.0f);
}

// Rotation
TEST_F(VmathTest, Rotate90Degrees) {
    Vector2f v(1.0f, 0.0f);
    v.rotate(M_PI / 2);
    EXPECT_NEAR(v.x, 0.0f, EPSILON);
    EXPECT_NEAR(v.y, 1.0f, EPSILON);
}

TEST_F(VmathTest, Rotate180Degrees) {
    Vector2f v(1.0f, 0.0f);
    v.rotate(M_PI);
    EXPECT_NEAR(v.x, -1.0f, EPSILON);
    EXPECT_NEAR(v.y, 0.0f, EPSILON);
}

TEST_F(VmathTest, Rotate360Degrees) {
    Vector2f v(3.0f, 4.0f);
    float originalX = v.x;
    float originalY = v.y;
    v.rotate(2 * M_PI);
    EXPECT_NEAR(v.x, originalX, EPSILON);
    EXPECT_NEAR(v.y, originalY, EPSILON);
}

TEST_F(VmathTest, RotateNegative90Degrees) {
    Vector2f v(1.0f, 0.0f);
    v.rotate(-M_PI / 2);
    EXPECT_NEAR(v.x, 0.0f, EPSILON);
    EXPECT_NEAR(v.y, -1.0f, EPSILON);
}

TEST_F(VmathTest, RotatePreservesLength) {
    Vector2f v(3.0f, 4.0f);
    float originalLength = v.length();
    v.rotate(M_PI / 3);  // 60 degrees
    EXPECT_NEAR(v.length(), originalLength, EPSILON);
}

// Get angle
TEST_F(VmathTest, GetAnglePositiveX) {
    Vector2f v(1.0f, 0.0f);
    EXPECT_NEAR(v.get_angle(), 0.0f, EPSILON);
}

TEST_F(VmathTest, GetAnglePositiveY) {
    Vector2f v(0.0f, 1.0f);
    EXPECT_NEAR(v.get_angle(), M_PI / 2, EPSILON);
}

TEST_F(VmathTest, GetAngleNegativeX) {
    Vector2f v(-1.0f, 0.0f);
    EXPECT_NEAR(fabs(v.get_angle()), M_PI, EPSILON);
}

TEST_F(VmathTest, GetAngleNegativeY) {
    Vector2f v(0.0f, -1.0f);
    EXPECT_NEAR(v.get_angle(), -M_PI / 2, EPSILON);
}

// Angle between vectors
TEST_F(VmathTest, AngleBetweenSameDirection) {
    Vector2f v1(1.0f, 0.0f);
    Vector2f v2(2.0f, 0.0f);
    EXPECT_NEAR(v1.angle_between(v2), 0.0f, EPSILON);
}

TEST_F(VmathTest, AngleBetweenPerpendicular) {
    Vector2f v1(1.0f, 0.0f);
    Vector2f v2(0.0f, 1.0f);
    EXPECT_NEAR(fabs(v1.angle_between(v2)), M_PI / 2, EPSILON);
}

TEST_F(VmathTest, AngleBetweenOpposite) {
    Vector2f v1(1.0f, 0.0f);
    Vector2f v2(-1.0f, 0.0f);
    EXPECT_NEAR(fabs(v1.angle_between(v2)), M_PI, EPSILON);
}

// Normalize
TEST_F(VmathTest, NormalizeUnitVector) {
    Vector2f v(1.0f, 0.0f);
    v.normalize();
    EXPECT_NEAR(v.length(), 1.0f, EPSILON);
}

TEST_F(VmathTest, NormalizeNonUnitVector) {
    Vector2f v(3.0f, 4.0f);
    v.normalize();
    EXPECT_NEAR(v.length(), 1.0f, EPSILON);
    EXPECT_NEAR(v.x, 0.6f, EPSILON);
    EXPECT_NEAR(v.y, 0.8f, EPSILON);
}

// Distance between points (using subtraction and length)
TEST_F(VmathTest, DistanceBetweenPoints) {
    Vector2f p1(0.0f, 0.0f);
    Vector2f p2(3.0f, 4.0f);
    float distance = (p2 - p1).length();
    EXPECT_FLOAT_EQ(distance, 5.0f);
}

TEST_F(VmathTest, DistanceSymmetric) {
    Vector2f p1(1.0f, 2.0f);
    Vector2f p2(4.0f, 6.0f);
    float d1 = (p2 - p1).length();
    float d2 = (p1 - p2).length();
    EXPECT_FLOAT_EQ(d1, d2);
}
