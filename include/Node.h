/*
 * Node.h
 *
 *  Created on: Jul 11, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include <iostream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------
// PUBLIC defines
//----------------------------------------------------------------------------
#define ZERO 			0
#define TWO 			2
#define THREE 			3
#define MAX_CHILDREN	3
#define NODE_POINTS		3

//****************************************************************************
//                           	NODE CLASS
//****************************************************************************
class Node
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int nChildren;					// # children.
	int children[MAX_CHILDREN];		// Children identifiers.
	int	points[NODE_POINTS];		// Points indexes.
	int	face;						// Face id.

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Node();
	Node( int p1, int p2, int p3, int face);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	// GET/SET functions.
	inline int getNChildren() { return(this->nChildren); };
	inline int getiChild(int index) { return(this->children[index]); };
	inline int getiPoint(int index) { return(this->points[index]); };
	inline int getFace() { return(this->face); };
	inline void setChildren( int id1, int id2) { this->children[0] = id1;
												 this->children[1] = id2;
												 this->nChildren = 2; };
	inline void setChildren( int id1, int id2, int id3) { this->children[0] = id1;
														  this->children[1] = id2;
														  this->children[2] = id3;
														  this->nChildren = 3; };
	inline bool isLeaf() { return(getNChildren() == ZERO); };
	inline bool splitted() { return(getNChildren() == TWO); };

	// I/O functions.
	void print(std::ostream& out);
	void read(std::istream& in);
	void write(std::ostream& out);
	string toStr();

	// Overload operators.
	Node& operator=( const Node& other );
};

#endif /* INCLUDE_NODE_H_ */
