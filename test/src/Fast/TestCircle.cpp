#include "Circle.h"
#include "defines.h"
#include "Point.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates a circle given its center and its radius and checks
 * all Circle class functions.
 *
 */
TEST (CircleUnitTest, Test_Generic_Constructor)
{
    TYPE fpX=1.0;
    TYPE fpY=1.0;
	Point<TYPE> centre(fpX, fpY);
    TYPE fpRadius=1.0;
	Circle circle(&centre, fpRadius);

    // Check circle center coordinates.
	Point<TYPE> *ptr_centre = circle.getRefCentre();
    EXPECT_EQ(fpX, ptr_centre->getX());
    EXPECT_EQ(fpY, ptr_centre->getY());

    // Check radius, area and perimeter.
    EXPECT_EQ(fpRadius, circle.getRadius());
    EXPECT_EQ(pow(fpRadius, 2.0)*PI, circle.area());
    EXPECT_EQ(2*fpRadius*PI, circle.perimeter());

    // Check circle does not contain external point (2, 3) nor points in
    // circle border, but contains (1.0,1.0), (0.0, 1.0) and (1.0,0.0).
	Point<TYPE> p1(1.0, 1.0);
	Point<TYPE> p2(0.1, 1.0);
	Point<TYPE> p3(1.0, 0.1);
	Point<TYPE> q1(2.0,3.0);
	Point<TYPE> q2(0.0, 1.0);
    EXPECT_EQ(true,  circle.inCircle(p1));
    EXPECT_EQ(true,  circle.inCircle(p2));
    EXPECT_EQ(true,  circle.inCircle(p3));
    EXPECT_NE(true,  circle.inCircle(q1));
    EXPECT_NE(true,  circle.inCircle(q2));
}

/**
 * DESCRIPTION: creates a circle given three points on the circle border
 * and checks all Circle class functions.
 *
 */
TEST (CircleUnitTest, Test_3_Points_Constructor)
{
    TYPE fpX=2.0;
    TYPE fpY=2.0;
	Point<TYPE> p1(0.0, 2.0);
	Point<TYPE> p2(4.0, 2.0);
	Point<TYPE> p3(2.0, 4.0);
	vector<Point<TYPE>> vPoints;
    vPoints.push_back(p1);
    vPoints.push_back(p2);
    vPoints.push_back(p3);
	Circle circle(vPoints);

    // Check circle center coordinates.
	Point<TYPE> *ptr_centre = circle.getRefCentre();
    EXPECT_EQ(fpX, ptr_centre->getX());
    EXPECT_EQ(fpY, ptr_centre->getY());

    // Check radius, area and perimeter.
    TYPE fpRadius=2.0;
    EXPECT_EQ(fpRadius, circle.getRadius());
    EXPECT_EQ(pow(fpRadius, 2.0)*PI, circle.area());
    EXPECT_EQ(2*fpRadius*PI, circle.perimeter());

    // Check circle does not contain external point (2, 3) nor points in
    // circle border, but contains (1.0,1.0), (0.0, 1.0) and (1.0,0.0).
    Point<TYPE> q1(0.0, 2.0);
	Point<TYPE> q2(0.0, 0.0);
	Point<TYPE> q3(ptr_centre->getX(), ptr_centre->getY());
	Point<TYPE> q4(4.0,4.0);
    EXPECT_EQ(false, circle.inCircle(q1));
    EXPECT_EQ(false, circle.inCircle(q2));
    EXPECT_EQ(true,  circle.inCircle(q3));
    EXPECT_EQ(false, circle.inCircle(q4));
}
