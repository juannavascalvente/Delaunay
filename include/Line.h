/*
 * Line.h
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#ifndef SRC_LINE_H_
#define SRC_LINE_H_

#include "defines.h"
#include "Point.h"

//****************************************************************************
//                           ENUM DEFINITION
//****************************************************************************
// Directions of a line or segment (in degrees).
enum Direction_E { HORIZONTAL_0,
     	 	 	 VERTICAL_90,
                 HORIZONTAL_180,
				 VERTICAL_270,
				 FROM_0_TO_90,
                 FROM_90_TO_180,
                 FROM_180_TO_270,
                 FROM_270_TO_360};

//****************************************************************************
//                           	LINE CLASS
//****************************************************************************
class Line
{
	Point<TYPE> origin;			// Line origin point.
	Point<TYPE> destination;	// Line destination point.
	TYPE	m;					// Line slope (m value in y=mx + n).
	TYPE 	n;					// n value in y=mx + n.

	//------------------------------------------------------------------------
	// 	Private functions.
	//------------------------------------------------------------------------
	int 		getSlopeAndN();
	Direction_E getDirection();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Line();
	Line(Point<TYPE> p, Point<TYPE> q) : origin(p), destination(q)
	{
		// Initialize origin and destination.
		this->m = 0.0;
		this->n = 0.0;
	}

	//------------------------------------------------------------------------
	// Public API.
	//------------------------------------------------------------------------
	bool intersect(Line other);
	void getIntersection(Line other, Point<TYPE> &intersection);
	void getMiddle(Point<TYPE> &middle);
	void extendToBoundary(Point<TYPE> &extrme);
	enum Turn_T	checkTurn(Point<TYPE> *p);

	// GET/SET functions.
	inline Point<TYPE> 	getOrigin() 	{ return(this->origin); };
	inline Point<TYPE> 	getDest() 		{ return(this->destination); };
	inline Point<TYPE>* getOriginRef() 	{ return(&this->origin); };
	inline Point<TYPE>* getDestRef() 	{ return(&this->destination); };
	void setOrigin(Point<TYPE> *p) 	{ this->origin = *p; };
	void setDestination(Point<TYPE> *p) 	{ this->destination = *p; };

	inline void		setSlope(TYPE m){ this->m = m; };
	inline void 	setN(TYPE n) 	{ this->n = n; };
	inline TYPE 	getSlope()		{ return(this->m); };
	inline TYPE 	getN() 			{ return(this->n); };

	void 	print(std::ostream& out);
};
#endif /* SRC_LINE_H_ */
