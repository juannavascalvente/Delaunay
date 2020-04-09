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
//	 						DEFINES SECTION
****************************************************************************/
#define INVALID_INDEX		(-1)

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
	Vertex(){this->originEdge = INVALID_INDEX; this->point = Point<TYPE>(0.0, 0.0);};
	Vertex(int originEdge, Point<TYPE> p) {this->originEdge = originEdge; this->point = p;};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	// Get / Set.
	inline void setOrigin(int value) {this->originEdge = value;};
	inline void setPoint(Point<TYPE> *value) {this->point = *value;};
	inline int getOrigin() const {return(this->originEdge);};
	inline Point<TYPE> getPoint() const {return(this->point);};
	inline Point<TYPE> *getRefPoint() {return(&this->point);};

	void random();

	// I/O functions.
	int		read(ifstream ifs);
	int		write(ofstream ofs);
	void 	print(std::ostream& out) const;
	void 	printPoint(std::ostream& out) const;

	// Operators.
	Vertex& operator=(Vertex other)
	{
		this->originEdge = other.originEdge;
		this->point = other.point;
		return(*this);
	};
	inline friend istream& operator>>(istream &in, Vertex &v)
	{
		in >> v.point;
		in >> v.originEdge;
		return(in);
	};
	inline friend ostream& operator<<(ostream &out, Vertex &v)
	{
		out << v.point << " ";
		out << v.originEdge;
		out << endl;
		return(out);
	};
	bool  operator==(const Vertex &v) const;
	bool  operator!=(const Vertex &v) const;
};

#endif /* VERTEX_H_ */


