#include "defines.h"
#include "Node.h"

#include <gtest/gtest.h>

/**
 * DESCRIPTION: creates Node using default constructor and checks fields.
 *
 */
TEST (NodeUnitTest, Test_Default_Constructor)
{
    // Create line using default constructor.
	Node node;

	// Check fields.
    EXPECT_EQ(0, node.getNChildren());
    EXPECT_THROW( node.getiChild(0), std::runtime_error);
    EXPECT_THROW( node.getiChild(1), std::runtime_error);
    EXPECT_THROW( node.getiChild(2), std::runtime_error);
    EXPECT_THROW( node.getiChild(3), std::runtime_error);
    EXPECT_THROW( node.getiPoint(0), std::runtime_error);
    EXPECT_THROW( node.getiPoint(1), std::runtime_error);
    EXPECT_THROW( node.getiPoint(2), std::runtime_error);
    EXPECT_THROW( node.getiPoint(3), std::runtime_error);
    EXPECT_EQ(INVALID, node.getFace());

    // Check various functions.
    EXPECT_TRUE(node.isLeaf());
    EXPECT_FALSE(node.isSplitted());
}


/**
 * DESCRIPTION: creates Node using parameterized constructor and checks fields.
 *
 */
TEST (NodeUnitTest, Test_Parameterized_Constructor)
{
	int iP=1;
	int iQ=10;
	int iR=100;
	int iFace=100;

    // Create line using default constructor.
    vector<int> vPoints = {iP, iQ, iR};
	Node node(vPoints, iFace);

	// Check fields.
    EXPECT_EQ(0, node.getNChildren());
    EXPECT_THROW( node.getiChild(0), std::runtime_error);
    EXPECT_THROW( node.getiChild(1), std::runtime_error);
    EXPECT_THROW( node.getiChild(2), std::runtime_error);
    EXPECT_EQ(iP, node.getiPoint(0));
    EXPECT_EQ(iQ, node.getiPoint(1));
    EXPECT_EQ(iR, node.getiPoint(2));
    EXPECT_EQ(iFace, node.getFace());

    // Check various functions.
    EXPECT_TRUE(node.isLeaf());
    EXPECT_FALSE(node.isSplitted());
}


/**
 * DESCRIPTION: creates Node using and checks set children.
 *
 */
TEST (NodeUnitTest, Test_Set_Children)
{
	int iP=1;
	int iQ=10;
	int iR=100;
	int iFace=100;

    // Create line using default constructor.
	Node nodeDefault;

    // Create line using default constructor.
    vector<int> vPoints = {iP, iQ, iR};
    Node nodeParams(vPoints, iFace);

    // Check both have no children.
    EXPECT_TRUE(nodeDefault.isLeaf());
    EXPECT_FALSE(nodeDefault.isSplitted());
    EXPECT_TRUE(nodeParams.isLeaf());
    EXPECT_FALSE(nodeParams.isSplitted());

    int id1=20;
    int id2=30;
    int id3=40;

    // Set 2 children into node.
    nodeDefault.setChildren(id1, id2);
    nodeParams.setChildren(id1, id2);

    // Check both have children and the node has been splitted.
    EXPECT_FALSE(nodeDefault.isLeaf());
    EXPECT_TRUE(nodeDefault.isSplitted());
    EXPECT_FALSE(nodeParams.isLeaf());
    EXPECT_TRUE(nodeParams.isSplitted());
    EXPECT_EQ(id1, nodeDefault.getiChild(0));
    EXPECT_EQ(id2, nodeDefault.getiChild(1));
    EXPECT_THROW( nodeDefault.getiChild(2), std::runtime_error);
    EXPECT_EQ(id1, nodeParams.getiChild(0));
    EXPECT_EQ(id2, nodeParams.getiChild(1));
    EXPECT_THROW( nodeParams.getiChild(2), std::runtime_error);

    // Set 3 children into node.
    nodeDefault.setChildren(id1, id2, id3);
    nodeParams.setChildren(id1, id2, id3);

    // Check both have children and the node has NOT been splitted.
    EXPECT_FALSE(nodeDefault.isLeaf());
    EXPECT_FALSE(nodeDefault.isSplitted());
    EXPECT_FALSE(nodeParams.isLeaf());
    EXPECT_FALSE(nodeParams.isSplitted());
    EXPECT_EQ(id1, nodeDefault.getiChild(0));
    EXPECT_EQ(id2, nodeDefault.getiChild(1));
    EXPECT_EQ(id3, nodeDefault.getiChild(2));
    EXPECT_EQ(id1, nodeParams.getiChild(0));
    EXPECT_EQ(id2, nodeParams.getiChild(1));
    EXPECT_EQ(id3, nodeParams.getiChild(2));
}


/**
 * DESCRIPTION: creates Node and checks assignment.
 *
 */
TEST (NodeUnitTest, Test_Assignment_Operator)
{
	int iP=1;
	int iQ=10;
	int iR=100;
	int iFace=100;

    // Create line using default constructor.
    vector<int> vPoints = {iP+1, iQ+1, iR+1};
    Node node1(vPoints, iFace + 1);
    vPoints = {iP, iQ, iR};
    Node node2(vPoints, iFace);

	// Check fields.
    EXPECT_EQ(node1.getNChildren(), node2.getNChildren());
    EXPECT_THROW( node1.getiChild(0), std::runtime_error);
    EXPECT_THROW( node2.getiChild(0), std::runtime_error);
    EXPECT_THROW( node1.getiChild(1), std::runtime_error);
    EXPECT_THROW( node2.getiChild(1), std::runtime_error);
    EXPECT_THROW( node1.getiChild(2), std::runtime_error);
    EXPECT_THROW( node2.getiChild(2), std::runtime_error);
    EXPECT_NE(node1.getiPoint(0), node2.getiPoint(0));
    EXPECT_NE(node1.getiPoint(1), node2.getiPoint(1));
    EXPECT_NE(node1.getiPoint(2), node2.getiPoint(2));
    EXPECT_NE(node1.getFace(), node2.getFace());

    // Execute assignment.
    node2 = node1;

	// Check fields.
    EXPECT_EQ(node1.getNChildren(), node2.getNChildren());
    EXPECT_THROW( node1.getiChild(0), std::runtime_error);
    EXPECT_THROW( node2.getiChild(0), std::runtime_error);
    EXPECT_THROW( node1.getiChild(1), std::runtime_error);
    EXPECT_THROW( node2.getiChild(1), std::runtime_error);
    EXPECT_THROW( node1.getiChild(2), std::runtime_error);
    EXPECT_THROW( node2.getiChild(2), std::runtime_error);
    EXPECT_EQ(node1.getiPoint(0), node2.getiPoint(0));
    EXPECT_EQ(node1.getiPoint(1), node2.getiPoint(1));
    EXPECT_EQ(node1.getiPoint(2), node2.getiPoint(2));
    EXPECT_EQ(node1.getFace(), node2.getFace());
}
