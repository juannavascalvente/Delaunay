#include "Vertex.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: check default vertex constructor.
 *
 */
TEST (VertexUnitTest, Test_Default_Constructor)
{
    // Check vertex edge and origin point.
	Vertex v;
	Point<TYPE> p;
    EXPECT_EQ(INVALID_INDEX, v.getOrigin());
    EXPECT_EQ(p, v.getPoint());

    // Modify origin coordinates.
    p.setX(2.0);
    p.setY(3.0);
    v.setPoint(&p);
    EXPECT_EQ(INVALID_INDEX, v.getOrigin());
    EXPECT_EQ(p, v.getPoint());
}

/**
 * DESCRIPTION: check parameterized vertex constructor.
 *
 */
TEST (VertexUnitTest, Test_Parameter_Constructor)
{
    // Check vertex edge and origin point.
	int originEdge=7;
	TYPE fpX=5.0;
	TYPE fpY=3.0;
	Point<TYPE> p(fpX, fpY);
	Vertex v(originEdge, p);

	// Check vertex build successfully.
    EXPECT_EQ(originEdge, v.getOrigin());
    EXPECT_EQ(p, v.getPoint());

    // Modify origin coordinates.
    p.setX(2.0);
    p.setY(3.0);
    v.setPoint(&p);
    EXPECT_EQ(originEdge, v.getOrigin());
    EXPECT_EQ(p, v.getPoint());
}

/**
 * DESCRIPTION: check shake
 *
 */
TEST (VertexUnitTest, Test_Shake)
{
    // Check vertex edge and origin point.
	int originEdge=7;
	TYPE fpX=5.0;
	TYPE fpY=3.0;
	Point<TYPE> p(fpX, fpY);
	Vertex v(originEdge, p);

	// Check vertex build successfully.
    EXPECT_EQ(originEdge, v.getOrigin());
    EXPECT_EQ(p, v.getPoint());

    // Shake vertex.
    v.shake();
    EXPECT_EQ(originEdge, v.getOrigin());
    EXPECT_NE(p, v.getPoint());
}

/**
 * DESCRIPTION: check operators
 *
 */
TEST (VertexUnitTest, Test_Operators)
{
    // Compare default vertex.
	Vertex v1;
	Vertex v2;
	EXPECT_EQ(v1, v2);

	// Check two vertex are not equal.
	int originEdge=7;
	TYPE fpX=5.0;
	TYPE fpY=3.0;
	Point<TYPE> p(fpX, fpY);
	Vertex v3(originEdge, p);
	EXPECT_NE(v1, v3);

	// Check assignment operator.
	v1 = v3;
	EXPECT_EQ(v1, v3);
	EXPECT_NE(v1, v2);
	EXPECT_NE(v3, v2);
}
