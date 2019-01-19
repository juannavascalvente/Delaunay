#include "Edge.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates an edge using default constructor and checks
 * fields
 *
 */
TEST (EdgeUnitTest, Test_Default_Constructor)
{
    // Check point coordinates.
	Edge edge;
    EXPECT_TRUE(edge.isInvalid());

    edge.setOrigin(2);
    EXPECT_TRUE(edge.isInvalid());

    edge.setTwin(2);
    EXPECT_TRUE(edge.isInvalid());

    edge.setPrevious(2);
    EXPECT_TRUE(edge.isInvalid());

    edge.setNext(2);
    EXPECT_TRUE(edge.isInvalid());

    edge.setFace(2);
    EXPECT_FALSE(edge.isInvalid());

    edge.reset();
    EXPECT_TRUE(edge.isInvalid());

    edge.set(1, 2, 3, 4, 5);
    EXPECT_FALSE(edge.isInvalid());
}

/**
 * DESCRIPTION: creates an edge using parameterized constructor and checks
 * fields
 *
 */
TEST (EdgeUnitTest, Test_Parameterized_Constructor)
{
    // Check point coordinates.
	Edge edge(1, 2, 3, 4, 5);
	EXPECT_FALSE(edge.isInvalid());

    edge.reset();
    EXPECT_TRUE(edge.isInvalid());
}

/**
 * DESCRIPTION: checks assignment, equality and inequality.
 *
 */
TEST (EdgeUnitTest, Test_Operators)
{
    // Check point coordinates.
	Edge edge1(1, 2, 3, 4, 5);
	Edge edge2(1, 2, 3, 4, 5);
	EXPECT_EQ(edge1, edge2);

	// Test equality.
	Edge edge3;
	Edge edge4;
	EXPECT_EQ(edge3, edge4);
	EXPECT_TRUE(edge3 == edge4);

	// Test inequality.
	EXPECT_NE(edge1, edge4);
	EXPECT_TRUE(edge1 != edge4);

	// Test assignment.
	edge1 = edge4;
	EXPECT_EQ(edge1, edge4);
}

/**
 * DESCRIPTION: Test getters and setters.
 *
 */
TEST (EdgeUnitTest, Test_Get_Set)
{
    // Check get functions.
	Edge edge1(1, 2, 3, 4, 5);
	Edge edge2(1, 2, 3, 4, 5);
	EXPECT_EQ(edge1.getOrigin(), edge2.getOrigin());
	EXPECT_EQ(edge1.getTwin(), edge2.getTwin());
	EXPECT_EQ(edge1.getPrevious(), edge2.getPrevious());
	EXPECT_EQ(edge1.getNext(), edge2.getNext());
	EXPECT_EQ(edge1.getFace(), edge2.getFace());

	edge2.setOrigin(edge1.getOrigin()+1);
	edge2.setTwin(edge1.getTwin()+1);
	edge2.setPrevious(edge1.getPrevious()+1);
	edge2.setNext(edge1.getNext()+1);
	edge2.setFace(edge1.getFace()+1);

	EXPECT_NE(edge1.getOrigin(), edge2.getOrigin());
	EXPECT_NE(edge1.getTwin(), edge2.getTwin());
	EXPECT_NE(edge1.getPrevious(), edge2.getPrevious());
	EXPECT_NE(edge1.getNext(), edge2.getNext());
	EXPECT_NE(edge1.getFace(), edge2.getFace());
}
