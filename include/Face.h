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
	Face(void);
	Face(int edge);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool isInvalid(void);

	// Get / Set.
	inline void setEdge(int value) { this->edge = value; };
	inline int 	getEdge(void) { return(this->edge); };

	// Operators.
	Face& operator=(Face other)
	{
		this->edge = other.edge;
		return(*this);
	};
	friend istream &operator>>(istream &in, Face &face)
				{ in >> face.edge; return(in);};
	friend ostream &operator<<(ostream &out, Face &face)
				{ out << face.edge; return(out);};
	inline bool operator==(const Face &other) const {return(this->edge == other.edge);};
	inline bool operator!=(const Face &other) const {return(this->edge != other.edge);};

	// I/O functions.
	void 	print(std::ostream& out) const;
	string 	toStr(void);
};

#endif /* INCLUDE_FACE_H_ */
