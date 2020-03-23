/*
 * Graph.h
 *
 *  Created on: Jul 11, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_GRAPH_H_
#define INCLUDE_GRAPH_H_

#include "Node.h"


//****************************************************************************
//                           	GRAPH CLASS
//****************************************************************************
class Graph
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int 	size;
	int 	nElements;
	Node 	*nodes;
	int  	*face_Node_Assign;
	bool	allocated;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Graph();
	Graph(int size);
	~Graph();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	inline bool isFull( ) {return(this->size == this->nElements);};
	inline int getNElements( ) {return(this->nElements);};
	inline int getSize( ) {return(this->size);};
	inline int isLeaf( int nodeIndex) {return(this->nodes[nodeIndex].isLeaf());};

	// Graph manipulation
	void insert( Node &node);
	void update( int index, int nChildren, Node *node);

	// GET/SET functions.
	inline Node* getRefNode(int nodeId) { return(&this->nodes[nodeId]); };
	inline int getNodeAssigned(int face) { return(this->face_Node_Assign[face]); };
	inline int getNChildren(int nodeId) { return(this->nodes[nodeId].getNChildren()); };
	inline int getiChild( int nodeId, int iChild) { return(this->nodes[nodeId].getiChild(iChild)); };
	void getVertices( int nodeIndex, int &index1, int &index2, int &index3);

	// Graph size functions.
	bool resize( int size, bool copy);
	void reset();

	// I/O functions.
	void print(std::ostream& out);
	bool read(string fileName);
	bool write(string fileName);
};

#endif /* INCLUDE_GRAPH_H_ */
