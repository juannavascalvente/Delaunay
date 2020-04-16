#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"

#include <iostream>
#include <string>
using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define ZERO 			0
#define TWO 			2
#define MAX_CHILDREN	3
#define NODE_POINTS		3


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Node
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	int nChildren;					// # children.
	int children[MAX_CHILDREN];		// Children identifiers.
	int	points[NODE_POINTS];		// Points indexes.
	int	face;						// Face id.

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
	Node();
	Node(int p1, int p2, int p3, int face);
    Node(int p1, int p2, int p3, int iChild1, int iChild2, int iChild3, int faceIn) : nChildren(MAX_CHILDREN), face(faceIn)
    {
        // Set children
        children[0] = iChild1;
        children[1] = iChild2;
        children[2] = iChild3;

        // Set points
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
    };
    Node(int p1, int p2, int p3, int iChild1, int iChild2, int faceIn) : nChildren(MAX_CHILDREN-1), face(faceIn)
    {
        // Set children
        children[0] = iChild1;
        children[1] = iChild2;
        children[2] = INVALID;

        // Set points
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
    };

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
	inline int getNChildren() { return(this->nChildren); };
	inline int getiChild(int index) { return(this->children[index]); };
	inline int getiPoint(int index) { return(this->points[index]); };
	inline int getFace() { return(this->face); };
	inline void setChildren(int id1, int id2) { this->children[0] = id1;
												 this->children[1] = id2;
												 this->nChildren = 2; };
	inline void setChildren(int id1, int id2, int id3) { this->children[0] = id1;
														  this->children[1] = id2;
														  this->children[2] = id3;
														  this->nChildren = 3; };
	inline bool isLeaf() { return(getNChildren() == ZERO); };
	inline bool splitted() { return(getNChildren() == TWO); };

    /*******************************************************************************************************************
    * I/O functions
    *******************************************************************************************************************/
	void print(std::ostream& out);
	void read(std::istream& in);
	void write(std::ostream& out);
	string toStr();

    /*******************************************************************************************************************
    * Operators
    *******************************************************************************************************************/
	Node& operator=( const Node& other );
};

#endif /* INCLUDE_NODE_H_ */
