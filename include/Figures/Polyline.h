/*
 * Polyline.h
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_POLYLINE_H_
#define INCLUDE_POLYLINE_H_

#include "Line.h"
#include "Set.h"

//****************************************************************************
//                           	POLYLINE CLASS
//****************************************************************************
class Polyline
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	Set<Line>	set;		// Set of points.

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Polyline() {};
	Polyline( int n);
	Polyline( int n, Point<TYPE> *points);
	virtual ~Polyline() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	inline int getLength() { return(this->set.getNElements()); };

	// Modification set functions.
	inline void	 add( Line *line) { this->set.add( *line);};
	inline Line* at( int index) { return(this->set.at( index));};
	inline void  reset() { this->set.reset();};

	// Figures functions.
	double 	length();

	// I/O functions.
	void print(std::ostream& out);
	string toStr();
};

#endif /* INCLUDE_POLYLINE_H_ */
