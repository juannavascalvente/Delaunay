/*
 * Vertex.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <iostream>
using namespace std;

#include "defines.h"
#include "Point.h"

/****************************************************************************
//	 						VERTEX CLASS DEFITNION
****************************************************************************/
class Vertex
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int			originEdge;
	Point<TYPE>	point;

public:

	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Vertex(){ this->originEdge = INVALID; this->point = Point<TYPE>(0.0, 0.0); };
	Vertex(int originEdge, Point<TYPE> p) { this->originEdge = originEdge; this->point = p; };

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	// Get / Set.
	inline void setOrigin(int value) { this->originEdge = value; };
	inline void setPoint(Point<TYPE> *value) { this->point = *value; };

	void random();
	void shake();

	inline int getOrigin() { return(this->originEdge); };
	inline Point<TYPE> getPoint() { return(this->point); };
	inline Point<TYPE> *getRefPoint() { return(&this->point); };

	// I/O functions.
	int		read(ifstream ifs);
	int		write(ofstream ofs);
	void 	print(std::ostream& out);
	void 	printPoint(std::ostream& out);

	// Operators.
	Vertex& operator=(Vertex other)
	{
		this->originEdge = other.getOrigin();
		this->point = other.getPoint();
		return(*this);
	};
	friend istream &operator>>(istream &in, Vertex &v) { in >> v.point; in >> v.originEdge; return(in);};
	friend ostream &operator<<(ostream &out, Vertex &v) { out << v.point << " " << v.originEdge; return(out);};
};

#endif /* VERTEX_H_ */


