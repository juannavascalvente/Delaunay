#include "Point.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates a point using default constructor and checks
 * point coordinates.
 *
 */
TEST (PointUnitTest, Test_Default_Constructor)
{
    // Check point coordinates.
    Point<float> p;
    EXPECT_EQ(0.0, p.getX());
    EXPECT_EQ(0.0, p.getY());
    EXPECT_NE(1.0, p.getX());
    EXPECT_NE(1.0, p.getY());
}

/**
 * DESCRIPTION: creates a point using coordinates constructor and checks
 * point coordinates.
 *
 */
TEST (PointUnitTest, Test_Coordinate_Constructor)
{
    // Check point coordinates.
	float fpX=1.0;
	float fpY=1.0;
    Point<float> p(fpX, fpY);
    EXPECT_EQ(fpX, p.getX());
    EXPECT_EQ(fpY, p.getY());
    EXPECT_NE(0.0, p.getX());
    EXPECT_NE(0.0, p.getY());
}

/**
 * DESCRIPTION: check setX, setY and setOrigin functions.
 *
 */
TEST (PointUnitTest, Test_SetX_SetY)
{
    // Check point coordinates.
    Point<float> p;
    EXPECT_EQ(0.0, p.getX());
    EXPECT_EQ(0.0, p.getY());
    EXPECT_NE(1.0, p.getX());
    EXPECT_NE(1.0, p.getY());

    // Change to (1.0, 1.0) coordinates.
	float fpX=1.0;
	float fpY=1.0;
	p.setX(fpX);
	p.setY(fpY);
	EXPECT_NE(0.0, p.getX());
	EXPECT_NE(0.0, p.getY());
    EXPECT_EQ(1.0, p.getX());
    EXPECT_EQ(1.0, p.getY());

    // Reset point to origin.
    p.setOrigin();
    EXPECT_EQ(0.0, p.getX());
    EXPECT_EQ(0.0, p.getY());
    EXPECT_NE(1.0, p.getX());
    EXPECT_NE(1.0, p.getY());
}

/**
 * DESCRIPTION: check Distance function.
 *
 */
TEST (PointUnitTest, Test_Distance)
{
    // Check point coordinates.
    Point<float> p(0.0, 0.0);
    Point<float> q(1.0, 0.0);
    Point<float> r(1.0, 1.0);
    EXPECT_EQ(0.0, p.distance(p));
    EXPECT_EQ(1.0, p.distance(q));
    float fpDistance=sqrt(2.0);
    EXPECT_EQ(fpDistance, p.distance(r));
}

/**
 * DESCRIPTION: check function that computes middle point between two points.
 *
 */
TEST (PointUnitTest, Test_MiddlePoint)
{
    // Check point coordinates.
    Point<float> p(0.0, 0.0);
    Point<float> q(1.0, 0.0);
    Point<float> middlePq(0.5, 0.0);
    Point<float> middlePr(0.5, 0.5);

    // Check middle point between p and q.
    Point<float> computedMiddle;
    Point<float>::middlePoint(&p, &q, &computedMiddle);
    EXPECT_EQ(computedMiddle, middlePq);
    EXPECT_NE(computedMiddle, middlePr);

    // Check middle point between p and r.
    Point<float> r(1.0, 1.0);
    Point<float>::middlePoint(&p, &r, &computedMiddle);
    EXPECT_NE(computedMiddle, middlePq);
    EXPECT_EQ(computedMiddle, middlePr);
}

/**
 * DESCRIPTION: check point generated randomly is not the same after shaking and
 * shifting.
 */
TEST (PointUnitTest, Test_Random_Shake_Shift)
{
    // Check point coordinates.
    Point<float> p;
    Point<float> q;
    p.random();
    q = p;
    EXPECT_EQ(p, q);

    // Check middle point between p and r.
    float fpShift=0.2;
    q = p;
    p.shift(fpShift, fpShift);
    float fpComputedDistance=p.distance(q);
    float fpExpectedDistance=p.distance(q);
    EXPECT_EQ(fpExpectedDistance, fpComputedDistance);
}

/**
 * DESCRIPTION: test function that compares Y coordinates between two points
 * and also compare two points using lexicographical comparison.
 */
TEST (PointUnitTest, Test_Comparison)
{
    // Check point coordinates.
    Point<float> p(1.0, 1.0);
    Point<float> q(2.0, 2.0);
    EXPECT_TRUE(Point<float>::lowerY(&p, &q));
    EXPECT_FALSE(Point<float>::lowerY(&q, &p));
    EXPECT_FALSE(Point<float>::lowerY(&p, &p));

    // Check point is not equal after shaking it.
    EXPECT_FALSE(Point<TYPE>::higher_Point(&p, &q, &Point<TYPE>::lexicographicHigher));
    EXPECT_TRUE(Point<TYPE>::higher_Point(&q, &p, &Point<TYPE>::lexicographicHigher));
    EXPECT_FALSE(Point<TYPE>::higher_Point(&p, &p, &Point<TYPE>::lexicographicHigher));

    Point<float> r(1.0, 2.0);
    EXPECT_FALSE(Point<TYPE>::higher_Point(&r, &q, &Point<TYPE>::lexicographicHigher));
    EXPECT_TRUE(Point<TYPE>::higher_Point(&q, &r, &Point<TYPE>::lexicographicHigher));

    EXPECT_FALSE(Point<TYPE>::higher_Point(&p, &r, &Point<TYPE>::lexicographicHigher));
    EXPECT_TRUE(Point<TYPE>::higher_Point(&r, &p, &Point<TYPE>::lexicographicHigher));
}

/**
 * DESCRIPTION: test function that checks turn between three points.
 */
TEST (PointUnitTest, Test_Check_Turn)
{
    // Check point coordinates.
    Point<float> p(1.0, 1.0);
    Point<float> q(2.0, 2.0);
    Point<float> r1(3.0, 3.0);
    Point<float> r2(3.0, 0.0);
    Point<float> r3(0.0, 3.0);

    EXPECT_EQ(COLLINEAR, p.check_Turn(q, r1));
    EXPECT_NE(LEFT_TURN, p.check_Turn(q, r1));
    EXPECT_NE(RIGHT_TURN, p.check_Turn(q, r1));

    EXPECT_NE(COLLINEAR, p.check_Turn(q, r2));
    EXPECT_NE(LEFT_TURN, p.check_Turn(q, r2));
    EXPECT_EQ(RIGHT_TURN, p.check_Turn(q, r2));

    EXPECT_NE(COLLINEAR, p.check_Turn(q, r3));
    EXPECT_EQ(LEFT_TURN, p.check_Turn(q, r3));
    EXPECT_NE(RIGHT_TURN, p.check_Turn(q, r3));
}

/**
 * DESCRIPTION: test operator function.
 */
TEST (PointUnitTest, Test_Operators)
{
    // Check point coordinates.
    Point<float> p(1.0, 1.0);
    Point<float> q(2.0, 2.0);
    Point<float> r(3.0, 3.0);
    Point<float> s;

    s = p;
    EXPECT_EQ(p + q, r);
    p = s;
    s = r;
    EXPECT_EQ(r - q, p);
    r = s;

    s = p;
    EXPECT_EQ(p*1, p);
    EXPECT_EQ(p*2, q);

    p = s;
    s = r / 3.0;
    EXPECT_EQ(s, p);
}

