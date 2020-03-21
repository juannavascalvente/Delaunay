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
enum Direction_E {HORIZONTAL_0,
				  VERTICAL_90,
                  HORIZONTAL_180,
				  VERTICAL_270,
				  FROM_0_TO_45,
				  FROM_45_TO_90,
				  FROM_90_TO_135,
                  FROM_135_TO_180,
                  FROM_180_TO_225,
				  FROM_225_TO_270,
				  FROM_270_TO_315,
                  FROM_315_TO_360};

// Possible slope values.
typedef enum Line_Slope_Enum
{
	INFINITE_POS_SLOPE,
	INFINITE_NEG_SLOPE,
	ZERO_SLOPE,
	REAL_SLOPE
} Line_Slope_Enum;

//****************************************************************************
//                           	LINE CLASS
//****************************************************************************
class Line
{
	Point<TYPE> origin;				// Line origin point.
	Point<TYPE> destination;		// Line destination point.
	TYPE	m;						// Line slope (m value in y=mx + n).
	TYPE 	n;						// n value in y=mx + n.
	Line_Slope_Enum enSlopeType;	// Type of slope (real, positive inf or

	//------------------------------------------------------------------------
	// 	Private functions.
	//------------------------------------------------------------------------
	void 		setSlopeAndN(void);
	Direction_E getDirection(void);

	inline void	setSlope(TYPE m)	{ this->m = m; };
	inline void setN(TYPE n) 		{ this->n = n; };
	inline void setSlopeType(Line_Slope_Enum enSlopeType)
									{ this->enSlopeType = enSlopeType; };

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Line(void);
	Line(Point<TYPE> p, Point<TYPE> q);

	//------------------------------------------------------------------------
	// Public API.
	//------------------------------------------------------------------------
	bool intersect(Line other);
	void getIntersection(Line other, Point<TYPE> &intersection);
	void getMiddle(Point<TYPE> &middle);
	void extendToBoundary(Point<TYPE> &extreme);
	enum Turn_T	checkTurn(Point<TYPE> *p);

	// GET/SET functions.
	inline Point<TYPE> 	getOrigin(void) 		{ return(this->origin); };
	inline Point<TYPE> 	getDest(void) 			{ return(this->destination); };
	inline TYPE getSlope(void)					{ return(this->m); };
	inline TYPE getN(void) 						{ return(this->n); };
	inline Line_Slope_Enum getSlopeType(void) 	{ return(this->enSlopeType); };

	void   setOrigin(Point<TYPE> *p);
	void   setDestination(Point<TYPE> *p);;

	void   print(std::ostream& out);
};
#endif /* SRC_LINE_H_ */
