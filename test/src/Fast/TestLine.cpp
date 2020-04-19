#include "Line.h"
#include "Point.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates a line using default constructor and checks
 * fields
 *
 */
TEST (LineUnitTest, Test_Default_Constructor)
{
    // Create line using default constructor.
	Line line;

	// Check fields.
	Point<TYPE> p;
    EXPECT_EQ(0.0, line.getSlope());
    EXPECT_EQ(0.0, line.getN());
    EXPECT_EQ(p, line.getOrigin());
    EXPECT_EQ(p, line.getDest());
}

/**
 * DESCRIPTION: creates a line using parameterized constructor and checks
 * fields
 *
 */
TEST (LineUnitTest, Test_Parameterized_Constructor)
{
    // Create line using parameterized constructor.
	Point<TYPE> p(1.0, 1.0);
	Point<TYPE> q(3.0, 9.0);
	Line line(p, q);

	// Check fields.
    EXPECT_EQ(4.0, line.getSlope());
    EXPECT_EQ(-3.0, line.getN());
    EXPECT_EQ(p, line.getOrigin());
    EXPECT_EQ(q, line.getDest());
}

/**
 * DESCRIPTION: checks set origin and destination using default and
 * parameterized constructor.
 *
 */
TEST (LineUnitTest, Test_Set_Origin_Destination)
{
    // Create line using default constructor.
	Line lineDefault;

	// Check default constructor fields.
	Point<TYPE> p;
    EXPECT_EQ(0.0, lineDefault.getSlope());
    EXPECT_EQ(0.0, lineDefault.getN());
    EXPECT_EQ(p, lineDefault.getOrigin());
    EXPECT_EQ(p, lineDefault.getDest());

    // Create line using parameterized constructor.
	Point<TYPE> q(1.0, 1.0);
	Point<TYPE> r(3.0, 9.0);
	Line lineParam(q, r);

	// Check default constructor fields.
    EXPECT_EQ(4.0, lineParam.getSlope());
    EXPECT_EQ(-3.0, lineParam.getN());
    EXPECT_EQ(q, lineParam.getOrigin());
    EXPECT_EQ(r, lineParam.getDest());

    // Check new origin point for both lines.
    Point<TYPE> s(10.0, 70.0);
    lineDefault.setOrigin(&s);
    lineParam.setOrigin(&s);
    EXPECT_EQ(s, lineDefault.getOrigin());
    EXPECT_EQ(s, lineParam.getOrigin());

    // Check new destination point for both lines.
    lineDefault.setDestination(&p);
    lineParam.setDestination(&p);
    EXPECT_EQ(p, lineDefault.getDest());
    EXPECT_EQ(p, lineParam.getDest());
}


/**
 * DESCRIPTION: checks functions that computes intersection.
 *
 */
TEST (LineUnitTest, Test_Intersection)
{
	// Create line.
	Point<TYPE> p1(1.0, 1.0);
	Point<TYPE> p2(1.0, 10.0);
	Line line1(p1, p2);

	// Check line do not intersect itself.
    EXPECT_FALSE(line1.intersect(line1));
    EXPECT_FALSE(line1.intersect(line1));

	// Create line parallel to original
	Point<TYPE> q1(5.0, 1.0);
	Point<TYPE> q2(5.0, 10.0);
	Line lineParallel(q1, q2);

	// Check parallel lines do not intersect.
    EXPECT_FALSE(line1.intersect(lineParallel));
    EXPECT_FALSE(lineParallel.intersect(line1));

    // Create perpendicular line.
	Point<TYPE> r1(0.0, 5.0);
	Point<TYPE> r2(2.0, 5.0);
	Line linePerpendicular(r1, r2);

	// Check perpendicular line intersects original line.
	EXPECT_TRUE(line1.intersect(linePerpendicular));
	EXPECT_TRUE(linePerpendicular.intersect(line1));
	Point<TYPE> intersection;
    line1.getIntersection(linePerpendicular, intersection);
	EXPECT_EQ(intersection.getX(), p1.getX());
	EXPECT_EQ(intersection.getY(), r2.getY());
    EXPECT_FALSE(linePerpendicular.intersect(lineParallel));

    // Create line that has same X coordinates but has lower Y coordinates.
	Point<TYPE> s1(1.0, 20.0);
	Point<TYPE> s2(1.0, 30.0);
	Line lineHigherY(s1, s2);
	EXPECT_FALSE(line1.intersect(lineHigherY));
	EXPECT_FALSE(lineHigherY.intersect(line1));
	EXPECT_FALSE(lineHigherY.intersect(lineParallel));
	EXPECT_FALSE(lineParallel.intersect(lineHigherY));
	EXPECT_FALSE(lineHigherY.intersect(linePerpendicular));
	EXPECT_FALSE(linePerpendicular.intersect(lineHigherY));

    // Create line that share origin point with original line do not intersect.
	Point<TYPE> s3(1.0, 10.0);
	Point<TYPE> s4(1.0, 30.0);
	Line lineShareOrigin(s3, s4);
	EXPECT_FALSE(line1.intersect(lineShareOrigin));
	EXPECT_FALSE(lineShareOrigin.intersect(line1));

    // Create line that share destination point with original line do not intersect.
	Point<TYPE> s5(1.0, 0.0);
	Point<TYPE> s6(1.0, 1.0);
	Line lineShareDestination(s3, s4);
	EXPECT_FALSE(line1.intersect(lineShareDestination));
	EXPECT_FALSE(lineShareDestination.intersect(line1));
}

/**
 * DESCRIPTION: check middle point of a line
 *
 */
TEST (LineUnitTest, Test_Middle)
{
    // Create line using parameterized constructor.
	Point<TYPE> p(1.0, 1.0);
	Point<TYPE> q(3.0, 3.0);
	Line line(p, q);

	// Check fields.
	Point<TYPE> middleComputed;
	Point<TYPE> middleExpected(2.0, 2.0);
	line.getMiddle(middleComputed);
    EXPECT_EQ(middleExpected, middleComputed);

    Point<TYPE> r(10.0, 10.0);
    Line line2(p, r);
    middleExpected.setX(5.5);
    middleExpected.setY(5.5);
    line2.getMiddle(middleComputed);
    EXPECT_EQ(middleExpected, middleComputed);
}

/**
 * DESCRIPTION: check function that checks the turn between two points in a
 * line and a given point.
 *
 */
TEST (LineUnitTest, Test_Check_Turn)
{
    // Create line using parameterized constructor.
	Point<TYPE> p(1.0, 1.0);
	Point<TYPE> q(3.0, 3.0);
	Line line(p, q);

	// Check collinear point where point is an extension of the line.
	Point<TYPE> r(10.0, 10.0);
    EXPECT_EQ(COLLINEAR, line.checkTurn(&r));
    EXPECT_NE(LEFT_TURN, line.checkTurn(&r));
    EXPECT_NE(RIGHT_TURN, line.checkTurn(&r));

	// Check collinear point where new point is in reverse direction of the line.
    r.setX(0.0);
    r.setY(0.0);
    EXPECT_EQ(COLLINEAR, line.checkTurn(&r));
    EXPECT_NE(LEFT_TURN, line.checkTurn(&r));
    EXPECT_NE(RIGHT_TURN, line.checkTurn(&r));

	// Check collinear point between line extreme points;
    r.setX(2.0);
    r.setY(2.0);
    EXPECT_EQ(COLLINEAR, line.checkTurn(&r));
    EXPECT_NE(LEFT_TURN, line.checkTurn(&r));
    EXPECT_NE(RIGHT_TURN, line.checkTurn(&r));

	// Check left turn.
    r.setX(3.0);
    r.setY(10.0);
	EXPECT_NE(COLLINEAR, line.checkTurn(&r));
    EXPECT_EQ(LEFT_TURN, line.checkTurn(&r));
    EXPECT_NE(RIGHT_TURN, line.checkTurn(&r));

	// Check right turn.
    r.setX(10.0);
    r.setY(3.0);
	EXPECT_NE(COLLINEAR, line.checkTurn(&r));
	EXPECT_NE(LEFT_TURN, line.checkTurn(&r));
    EXPECT_EQ(RIGHT_TURN, line.checkTurn(&r));
}

/**
 * DESCRIPTION: check function that computes a point that is the extension of
 * the line and it is located in the extreme of the display.
 *
 */
TEST (LineUnitTest, Test_Extend_To_Boundary)
{
    // Create line using parameterized constructor.
	Point<TYPE> p(1.0, 1.0);
	Point<TYPE> q(3.0, 3.0);
	Line line(p, q);

	// Check extreme point is in right side.
	Point<TYPE> computedExtreme;
	Point<TYPE> expectedExtreme(2*MAX_X_COORD, 2*MAX_Y_COORD);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point is in low side.
	expectedExtreme.setX((TYPE) 3637.7272727272725);
	expectedExtreme.setY(-MAX_X_COORD);
	q.setX(5.0);
	q.setY(-10.0);
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point is in left side.
	expectedExtreme.setX(-2*MAX_X_COORD);
	expectedExtreme.setY(30002.500000);
	q.setX(-5.0);
	q.setY(10.0);
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point is in up side.
	expectedExtreme.setX(10000.500000);
	expectedExtreme.setY(2*MAX_Y_COORD);
	q.setX(2.0);
	q.setY(3.0);
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point parallel to the right.
	q.setX(3.0);
	q.setY(1.0);
	expectedExtreme.setX(MAX_X_COORD);
	expectedExtreme.setY(q.getY());
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point parallel to the left.
	q.setX(-3.0);
	q.setY(1.0);
	expectedExtreme.setX(-MAX_X_COORD);
	expectedExtreme.setY(q.getY());
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point positive infinite.
	q.setX(1.0);
	q.setY(10.0);
	expectedExtreme.setX(q.getX());
	expectedExtreme.setY(MAX_X_COORD);
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);

	// Check extreme point negative infinite.
	q.setX(1.0);
	q.setY(-10.0);
	expectedExtreme.setX(q.getX());
	expectedExtreme.setY(-MAX_X_COORD);
	line.setDestination(&q);
	line.extendToBoundary(computedExtreme);
	EXPECT_EQ(expectedExtreme, computedExtreme);
}
