/*
 * Polygon.h
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_POLYGON_H_
#define INCLUDE_POLYGON_H_

#include "Line.h"
#include "Point.h"
#include "Set.h"

typedef Point<TYPE> PointSet;

//****************************************************************************
//                           	POLYGON CLASS
//****************************************************************************
class Polygon
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	Set<PointSet>	set;		// Set of points.

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Polygon(){};
	Polygon(int n) : set(n) {};
	Polygon(int n, Point<TYPE> *points);
	virtual ~Polygon() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	inline int getNElements() { return(this->set.getNElements()); };
	inline int getSize() { return(this->set.getSize()); };
	inline Point<TYPE>* at(int index) { return(this->set.at(index)); };

	// Modification set functions.
	inline void	add(PointSet *p) { this->set.add(*p);};
	inline void reset() { this->set.reset();};
	inline void resize(int size, bool copy);

	// Figures functions.
	virtual double	area();
	double 	perimeter();
	void 	centroid(Point<TYPE> &center);
	bool	getIntersections(Line &line, Set<int> &set);
	bool	isInternal(Point<TYPE> &p);

	// I/O functions.
	void 	print(std::ostream& out);
	string 	toStr();
};

#endif /* INCLUDE_POLYLINE_H_ */
