#include "Face.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates a face using default constructor and checks
 * fields
 *
 */
TEST (FaceUnitTest, Test_Default_Constructor)
{
    // Check point coordinates.
	Face face;
    EXPECT_TRUE(face.isInvalid());

    int iEdge=7;
    face.setEdge(iEdge);
    EXPECT_FALSE(face.isInvalid());
    EXPECT_EQ(face.getEdge(), iEdge);
}


/**
 * DESCRIPTION: creates a face using parameterized constructor and checks
 * fields
 *
 */
TEST (FaceUnitTest, Test_Parameterized_Constructor)
{
    // Check parameterized face constructor.
	int iEdge=7;
	Face face(iEdge);
    EXPECT_FALSE(face.isInvalid());
    EXPECT_EQ(face.getEdge(), iEdge);
}


/**
 * DESCRIPTION: checks face operators
 *
 */
TEST (FaceUnitTest, Test_Operators)
{
    // Check equality.
	int iEdge=7;
	Face face1(iEdge);
	Face face2(iEdge);
    EXPECT_EQ(face1, face2);
    EXPECT_TRUE(face1 == face2);

    // Check inequality.
    Face face3(iEdge+2);
    EXPECT_NE(face3, face2);
    EXPECT_TRUE(face3 != face2);

    // Check assignment.
    face3 = face2;
    EXPECT_EQ(face2, face3);
    EXPECT_TRUE(face2 == face3);
}
