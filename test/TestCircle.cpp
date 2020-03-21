#include "Circle.h"
#include "defines.h"
#include "Point.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates default circle and checks all Circle class functions
 *
 */
TEST (CircleUnitTest, Test_Default_Constructor)
{
	Circle circle;

    // Check circle center coordinates.
    Point<float> *centre;
    centre = circle.getRefCentre();
    EXPECT_EQ(0.0, centre->getX());
    EXPECT_NE(1.0, centre->getX());
    EXPECT_EQ(0.0, centre->getY());
    EXPECT_NE(1.0, centre->getY());

    // Check radius, area and perimeter.
    EXPECT_EQ(0.0, circle.getRadius());
    EXPECT_EQ(0.0, circle.area());
    EXPECT_EQ(0.0, circle.perimeter());

    // Check circle contains no point.
	Point<float> p;
	Point<float> q(2.0,3.0);
    EXPECT_EQ(false, circle.inCircle(&p));
    EXPECT_EQ(false, circle.inCircle(&q));
}

/**
 * DESCRIPTION: creates a circle given its center and its radius and checks
 * all Circle class functions.
 *
 */
TEST (CircleUnitTest, Test_Generic_Constructor)
{
	float fpX=1.0;
	float fpY=1.0;
	Point<float> centre(fpX, fpY);
	float fpRadius=1.0;
	Circle circle(&centre, fpRadius);

    // Check circle center coordinates.
	Point<float> *ptr_centre = circle.getRefCentre();
    EXPECT_EQ(fpX, ptr_centre->getX());
    EXPECT_EQ(fpY, ptr_centre->getY());

    // Check radius, area and perimeter.
    EXPECT_EQ(fpRadius, circle.getRadius());
    EXPECT_EQ(pow(fpRadius, 2.0)*PI, circle.area());
    EXPECT_EQ(2*fpRadius*PI, circle.perimeter());

    // Check circle does not contain external point (2, 3) nor points in
    // circle border, but contains (1.0,1.0), (0.0, 1.0) and (1.0,0.0).
	Point<float> p1(1.0, 1.0);
	Point<float> p2(0.1, 1.0);
	Point<float> p3(1.0, 0.1);
	Point<float> q1(2.0,3.0);
	Point<float> q2(0.0, 1.0);
    EXPECT_EQ(true,  circle.inCircle(&p1));
    EXPECT_EQ(true,  circle.inCircle(&p2));
    EXPECT_EQ(true,  circle.inCircle(&p3));
    EXPECT_NE(true,  circle.inCircle(&q1));
    EXPECT_NE(true,  circle.inCircle(&q2));
}

/**
 * DESCRIPTION: creates a circle given three points on the circle border
 * and checks all Circle class functions.
 *
 */
TEST (CircleUnitTest, Test_3_Points_Constructor)
{
	float fpX=2.0;
	float fpY=2.0;
	Point<float> p1(0.0, 2.0);
	Point<float> p2(4.0, 2.0);
	Point<float> p3(2.0, 4.0);
	Circle circle(&p1, &p2, &p3);

    // Check circle center coordinates.
	Point<float> *ptr_centre = circle.getRefCentre();
    EXPECT_EQ(fpX, ptr_centre->getX());
    EXPECT_EQ(fpY, ptr_centre->getY());

    // Check radius, area and perimeter.
    float fpRadius=2.0;
    EXPECT_EQ(fpRadius, circle.getRadius());
    EXPECT_EQ(pow(fpRadius, 2.0)*PI, circle.area());
    EXPECT_EQ(2*fpRadius*PI, circle.perimeter());

    // Check circle does not contain external point (2, 3) nor points in
    // circle border, but contains (1.0,1.0), (0.0, 1.0) and (1.0,0.0).
    Point<float> q1(0.0, 2.0);
	Point<float> q2(0.0, 0.0);
	Point<float> q3(ptr_centre->getX(), ptr_centre->getY());
	Point<float> q4(4.0,4.0);
    EXPECT_EQ(false, circle.inCircle(&q1));
    EXPECT_EQ(false, circle.inCircle(&q2));
    EXPECT_EQ(true,  circle.inCircle(&q3));
    EXPECT_EQ(false, circle.inCircle(&q4));
}
