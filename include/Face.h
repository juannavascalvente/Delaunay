/*
 * Face.h
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_FACE_H_
#define INCLUDE_FACE_H_

#include <iostream>
using namespace std;

/****************************************************************************
//	 						FACE CLASS DEFITNION
****************************************************************************/
class Face
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int		edge;				// Edge in face.

public:
	/*------------------------------------------------------------------------
	  Constructor/Destructor.
	------------------------------------------------------------------------*/
	Face();
	Face( int edge, bool imaginary);
	Face( int edge);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool hasNegativeVertex();

	// Get / Set.
	inline void setEdge( int value) { this->edge = value; };
	inline int 	getEdge( ) { return(this->edge); };

	// Operators.
	friend istream &operator>>(istream &in, Face &face)
				{ in >> face.edge; return(in);};
	friend ostream &operator<<(ostream &out, Face &face)
				{ out << face.edge; return(out);};

	// I/O functions.
	void 	print( std::ostream& out);
	string 	toStr();
};

#endif /* INCLUDE_FACE_H_ */
