/*
 * Node.cpp
 *
 *  Created on: Jul 11, 2016
 *      Author: jnavas
 */

#include "defines.h"
#include "Node.h"

#include <sstream>
#include <iostream>
using namespace std;


//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Node::Node(void)
{
	// Initialize fields.
	this->nChildren = 0;
	this->children[0] = INVALID;
	this->children[1] = INVALID;
	this->children[2] = INVALID;
	this->points[0] = INVALID;
	this->points[1] = INVALID;
	this->points[2] = INVALID;
	this->face = INVALID;
}

Node::Node(int p1, int p2, int p3, int face)
{
	// Initialize fields.
	this->nChildren = 0;
	this->children[0] = INVALID;
	this->children[1] = INVALID;
	this->children[2] = INVALID;
	this->points[0] = p1;
	this->points[1] = p2;
	this->points[2] = p3;
	this->face = face;
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	print
* IN:		out			output stream
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints node information
***************************************************************************/
void Node::print(std::ostream& out)
{
	// Check if node is a leaf.
	if (this->isLeaf())
	{
		out << "Node is a leaf. Node points: " << this->points[0] <<
				"," << this->points[1] << "," << this->points[2] <<
				". Assigned face " << this->face;
	}
	// Print node data.
	else if (this->nChildren == TWO)
	{
		out << "Node has " << this->nChildren << " children: " <<
				this->children[0] << "," << this->children[1] <<
				". Points: " << this->points[0] << "," << this->points[1] <<
				"," << this->points[2] << ". Assigned face " <<
				this->face;
	}
	else
	{
		out << "Node has " << this->nChildren << " children: " <<
				this->children[0] << "," << this->children[1] << "," <<
				this->children[2] << ". Points: " << this->points[0] <<
				"," << this->points[1] << "," << this->points[2] <<
				". Assigned face " << this->face;
	}
}

/***************************************************************************
* Name: 	read
* IN:		in			in stream to read form.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	reads the node information from "in" stream.
***************************************************************************/
void Node::read(std::istream& in)
{
	int	i=0;		// Loop counter.

	// Write node points.
	for (i=0; i<NODE_POINTS; i++)
	{
		in >> this->points[i];
	}

	// Write node children.
	in >> this->nChildren;
	for (i=0; i<MAX_CHILDREN; i++)
	{
		in >> this->children[i];
	}

	// Write node face.
	in >> this->face;
}

/***************************************************************************
* Name: 	write
* IN:		out			output stream to write to.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	writes the node information to "out" stream.
***************************************************************************/
void Node::write(std::ostream& out)
{
	int	i=0;		// Loop counter.

	// Write node points.
	for (i=0; i<NODE_POINTS; i++)
	{
		out << this->points[i] << " ";
	}

	// Write node children.
	out << this->nChildren << " ";
	for (i=0; i<MAX_CHILDREN; i++)
	{
		out << this->children[i] << " ";
	}

	// Write node face.
	out << this->face;
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	node information as a string
* GLOBAL:	NONE
* Description: 	concat the node information as a string
***************************************************************************/
string Node::toStr(void)
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print( oss);
	text = oss.str();

	return(text);
}

/***************************************************************************
* Name: 	opeartor=
* IN:		other		node to copy
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	copy input node to "this"
***************************************************************************/
Node& Node::operator=(const Node& other)
{
	// Copy all fields.
	this->nChildren = other.nChildren;
	this->children[0] = other.children[0];
	this->children[1] = other.children[1];
	this->children[2] = other.children[2];
	this->points[0] = other.points[0];
	this->points[1] = other.points[1];
	this->points[2] = other.points[2];
	this->face = other.face;
	return(*this);
}
