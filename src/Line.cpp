/*
 * Line.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#include "Line.h"
#include "Logging.h"
#include <float.h>
#include <limits.h>

//#define DEBUG_EXTEND_SEGMENT
//#define DEBUG_COMPUTE_SLOPE_N
//#define DEBUG_LINE_INTERSECT

//#define LINE_GET_DIRECTION
/*****************************************************************************
* 							LINE class implementation
*****************************************************************************/
Line::Line(void) : m(0.0), n(0.0), enSlopeType(ZERO_SLOPE)
{
	// Initialize origin and destination points to (0,0).
	this->origin.setOrigin();
	this->destination.setOrigin();
}

Line::Line(Point<TYPE> p, Point<TYPE> q) : origin(p), destination(q), m(0.0),
		n(0.0), enSlopeType(REAL_SLOPE)
{
	this->setSlopeAndN();
}

//--------------------------------------------------------------------------
// Public functions.
//--------------------------------------------------------------------------
/*****************************************************************************
FUNCTION:       intersect
DESCRIPTION:    check if two segments intersects.
INPUT:          "other" second segment
OUTPUT:         TRUE    if this and other segments intersects.
                FALSE   i.o.c.
PRE:            N/A
POST:           *********************************************************
COMPLEXITY:     O(1)
*****************************************************************************/
// TODO - Issue #35
bool Line::intersect(Line other)
{
    bool doIntersect=false;
    enum Turn_T	turn1, turn2;

    // Compute turns between 1st segment and origin and destination point of segment 2.
    turn1 = this->origin.check_Turn(this->destination, other.origin);
    turn2 = this->origin.check_Turn(this->destination, other.destination);
#ifdef DEBUG_LINE_INTERSECT
	Logging::buildText(__FUNCTION__, __FILE__, "First turn 1 is ");
	Logging::buildText(__FUNCTION__, __FILE__, turn1);
	Logging::buildText(__FUNCTION__, __FILE__, " and turn 2 is ");
	Logging::buildText(__FUNCTION__, __FILE__, turn2);
	Logging::write(true, Info);
#endif

    // If it is collinear then return false (return value already set).
    if ((turn1 != COLLINEAR) && (turn2 != COLLINEAR))
    {
        if (((turn1 == LEFT_TURN) && (turn2 == RIGHT_TURN)) ||
            ((turn1 == RIGHT_TURN) && (turn2 == LEFT_TURN)))
        {
            // Compute turns between 2nd segment and origin and destination point of segment 1.
            turn1 = other.origin.check_Turn(other.destination, this->origin);
            turn2 = other.origin.check_Turn(other.destination, this->destination);
#ifdef DEBUG_LINE_INTERSECT
            Logging::buildText(__FUNCTION__, __FILE__, "Second turn 1 is ");
            Logging::buildText(__FUNCTION__, __FILE__, turn1);
            Logging::buildText(__FUNCTION__, __FILE__, " and turn 2 is ");
            Logging::buildText(__FUNCTION__, __FILE__, turn2);
            Logging::write(true, Info);
#endif
            // If it is collinear then return false (return value already set).
            if ((turn1 != COLLINEAR) && (turn2 != COLLINEAR))
            {
                if (((turn1 == LEFT_TURN) && (turn2 == RIGHT_TURN)) ||
                    ((turn1 == RIGHT_TURN) && (turn2 == LEFT_TURN)))
                {
                	doIntersect = true;
                }
            }
        }
    }

    return(doIntersect);
}


/*****************************************************************************
 * 	FUNCTION:      	getIntersection
 * 	DESCRIPTION:   	compute intersection between two segments.
 * 	INPUT:         	"other" second segment
 * 	OUTPUT:        	NONE
 * 	IN/OUTPUT:		"intersection" stores intersection point
 * 	PRE:            N/A
 * 	POST:           *********************************************************
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
// TODO - Issue #35
void Line::getIntersection(Line other, Point<TYPE> &intersection)
{
	// Compute intersection.
    TYPE   det=0.0;
    TYPE   A1=0.0, B1=0.0, C1=0.0;
    TYPE   A2=0.0, B2=0.0, C2=0.0;

    A1 = this->destination.getX() - this->origin.getX();
    B1 = this->destination.getY() - this->origin.getY();
    C1 = this->destination.getX()*this->origin.getY() - this->origin.getX()*this->destination.getY();

    A2 = other.destination.getX() - other.origin.getX();
    B2 = other.destination.getY() - other.origin.getY();
    C2 = other.destination.getX()*other.origin.getY() - other.origin.getX()*other.destination.getY();

    det = A1*B2 - A2*B1;

    // Set intersection values.
    intersection.setX((A2*C1 - A1*C2) / det);
    intersection.setY((B2*C1 - B1*C2) / det);
}

/*****************************************************************************
 * 	FUNCTION:      	getMiddle
 * 	DESCRIPTION:   	compute line middle point.
 * 	INPUT:         	NONE
 * 	OUTPUT:        	middle		line middle point
 * 	IN/OUTPUT:		NONE
 * 	PRE:            N/A
 * 	POST:           computes the middle point of the line.
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
void Line::getMiddle(Point<TYPE> &middle)
{
    // Compute middle point of edge.
    middle.setX((this->origin.getX() + this->destination.getX()) / 2.0);
    middle.setY((this->origin.getY() + this->destination.getY()) / 2.0);
}



/*****************************************************************************
 * 	FUNCTION:      	extendToBoundary
 * 	DESCRIPTION:   	Extend line formed by line and get point at window
 * 					boundaries.
 * 	INPUT:         	NONE
 * 	OUTPUT:        	extreme		line middle point
 * 	IN/OUTPUT:		NONE
 * 	PRE:            N/A
 * 	POST:
 * 	COMPLEXITY:     O(1)
*****************************************************************************/

//FROM_0_TO_45,
//				  FROM_45_TO_90,
//				  FROM_90_TO_135,
//                  FROM_135_TO_180,
//                  FROM_180_TO_225,
//				  FROM_225_TO_270,
//				  FROM_270_TO_315,
//                  FROM_315_TO_360

//MAX_X_COORD				10000.0
//#define MAX_Y_COORD
void Line::extendToBoundary(Point<TYPE> &extreme)
{
    Direction_E direction; 	// Line direction.

    // Compute line m and n.
#ifdef DEBUG_EXTEND_SEGMENT
	Logging::buildText(__FUNCTION__, __FILE__, "Segment points are ");
	Logging::buildText(__FUNCTION__, __FILE__, &this->origin);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, &this->destination);
	Logging::write(true, Info);
#endif
	// Check type of slope.
	switch(this->getSlopeType())
	{
		case REAL_SLOPE:
		{
			// Compute direction of p-q line.
			direction = this->getDirection();

			// Line towards positive x coordinates values.
			switch (direction)
			{
				// Max Y coordinate and compute X.
				case FROM_45_TO_90:
				case FROM_90_TO_135:
				{
//					cout << " FROM_45_TO_90 FROM_90_TO_135 " << endl;
					extreme.setY(MAX_Y_COORD);
					extreme.setX((extreme.getY() - this->getN()) / this->getSlope());
					break;
				}
				// Min X coordinate and compute Y.
				case FROM_135_TO_180:
				case FROM_180_TO_225:
				{
//					cout << " FROM_135_TO_180 FROM_180_TO_225 " << endl;
					extreme.setX(-MAX_X_COORD);
					extreme.setY((this->getSlope()*extreme.getX()) + this->getN());
					break;
				}
				// Min Y coordinate and compute X.
				case FROM_225_TO_270:
				case FROM_270_TO_315:
				{
//					cout << " FROM_225_TO_270 FROM_270_TO_315 " << endl;
					extreme.setY(-MAX_Y_COORD);
					extreme.setX((extreme.getY() - this->getN()) / this->getSlope());
					break;
				}
				// FROM_0_TO_45
				// FROM_315_TO_360
				// Max X coordinate and compute Y.
				default:
				{
//					cout << " FROM_0_TO_45 FROM_315_TO_360 " << endl;
					extreme.setX(MAX_X_COORD);
					extreme.setY((this->getSlope()*extreme.getX()) + this->getN());
					break;
				}
			}
//			printf("%lf %lf\n", extreme.getX(), extreme.getY());
			break;
		}
		// Set current x and maximum y-coordinate value.
		case INFINITE_POS_SLOPE:
		{
			extreme.setX(this->origin.getX());
			extreme.setY(MAX_Y_COORD);
			break;
		}
		// Set current x and minimum y-coordinate value.
		case INFINITE_NEG_SLOPE:
		{
			extreme.setX(this->origin.getX());
			extreme.setY(-MAX_Y_COORD);
			break;
		}
		// ZERO_SLOPE
		default:
		{
			// Compute direction of p-q line.
			direction = this->getDirection();

			// Line is parallel to X coordinates axis towards right.
			if (direction == HORIZONTAL_0)
			{
				extreme.setX(MAX_X_COORD);
				extreme.setY(this->origin.getY());
			}
			// Line is parallel to X coordinates axis towards left.
			else
			{
				extreme.setX(-MAX_X_COORD);
				extreme.setY(this->origin.getY());
			}
			break;
		}
	}
#ifdef DEBUG_EXTEND_SEGMENT
	Logging::buildText(__FUNCTION__, __FILE__, "Extreme point is ");
	Logging::buildText(__FUNCTION__, __FILE__, &extreme);
	Logging::write(true, Info);
#endif
}

/*****************************************************************************
 * 	FUNCTION:      	checkTurn
 * 	DESCRIPTION:   	return turn between line points and input "p" point.
 * 	INPUT:         	p			point to check turn
 * 	OUTPUT:        	NONE
 * 	IN/OUTPUT:		NONE
 * 	RETURN:			turn between line extreme points and input "p" point.
 * 	PRE:            N/A
 * 	POST:           computes the middle point of the line.
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
enum Turn_T	Line::checkTurn(Point<TYPE> *p)
{
	// Check turn.
	return(this->origin.check_Turn(this->destination, *p));
}


/*****************************************************************************
 * 	FUNCTION:      	setOrigin
 * 	DESCRIPTION:   	updates origin point and updates line slope and n value.
 * 	INPUT:         	p			new origin point
 * 	OUTPUT:        	NONE
 * 	IN/OUTPUT:		NONE
 * 	RETURN:			NONE
 * 	PRE:            N/A
 * 	POST:           origin point is p and slope and n are recomputed.
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
void Line::setOrigin(Point<TYPE> *p)
{
	// Update destination point.
	this->origin = *p;

	// Update slope and N.
	this->setSlopeAndN();
}


/*****************************************************************************
 * 	FUNCTION:      	setOrigin
 * 	DESCRIPTION:   	updates destination point and updates line slope and n
 * 					value.
 * 	INPUT:         	p			new destination point
 * 	OUTPUT:        	NONE
 * 	IN/OUTPUT:		NONE
 * 	RETURN:			NONE
 * 	PRE:            N/A
 * 	POST:           destination point is p and slope and n are recomputed.
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
void Line::setDestination(Point<TYPE> *p)
{
	// Update destination point.
	this->destination = *p;

	// Update slope and N.
	this->setSlopeAndN();
}


/***************************************************************************
* Name: 		print
* IN:			out			output stream
* OUT:			NONE
* RETURN:		NONE
* GLOBAL:		NONE
* Description: 	print line origin and destination points.
***************************************************************************/
void Line::print(std::ostream& out)
{
	this->origin.print(out);
	this->destination.print(out);
	out << "Slope: " << this->m << " N: " << this->n << endl;
	out << "Slope type: ";
	switch (this->enSlopeType)
	{
		case INFINITE_POS_SLOPE:
		{
			out << " INFINITE_POS_SLOPE" << endl;
			break;
		}
		case INFINITE_NEG_SLOPE:
		{
			out << " INFINITE_NEG_SLOPE" << endl;
			break;
		}
		case ZERO_SLOPE:
		{
			out << " ZERO_SLOPE" << endl;
			break;
		}
		default:
		{
			out << " REAL_SLOPE" << endl;
			break;
		}
	}
}


//--------------------------------------------------------------------------
// Private functions.
//--------------------------------------------------------------------------
/*****************************************************************************
 * 	FUNCTION:      	setSlopeAndN
 * 	DESCRIPTION:   	compute m and n values in y=mx + n.
 * 	INPUT:         	NONE
 * 	OUTPUT:			NONE
 * 	IN/OUTPUT:		NONE
 * 	PRE:            N/A
 * 	POST:           m = slope between origin and destination
 * 					n = n value in y=mx + n
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
void Line::setSlopeAndN(void)
{
	TYPE fpDeltaX = (this->destination.getX() - this->origin.getX());
	TYPE fpDeltaY = (this->destination.getY() - this->origin.getY());

	// Check if slope is zero.
	if (0 == fpDeltaY)
	{
		this->setSlopeType(ZERO_SLOPE);
	}
	else if (0 == fpDeltaX)
	{
		if (fpDeltaY > 0)
		{
			this->setSlopeType(INFINITE_POS_SLOPE);
		}
		else
		{
			this->setSlopeType(INFINITE_NEG_SLOPE);
		}
	}
	else
	{
		this->setSlopeType(REAL_SLOPE);

		// Compute slope (m) in y=m*x + n
		this->setSlope(fpDeltaY / fpDeltaX);

		// Compute (n) in y=m*x + n
		this->setN(this->destination.getY() - this->getSlope()*this->destination.getX());

	}
}

/*****************************************************************************
 * 	FUNCTION:      	getDirection
 * 	DESCRIPTION:   	PENDING
 * 	INPUT:         	NONE
 * 	OUTPUT:			NONE
 * 	IN/OUTPUT:		NONE
 * 	RETURN:			PENDING
 * 	PRE:            N/A
 * 	POST:			PENDING
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
Direction_E Line::getDirection(void)
{
	Direction_E direction;		// Return value.

#ifdef LINE_GET_DIRECTION
	Logging::buildText(__FUNCTION__, __FILE__, "Slope ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getSlope());
	Logging::buildText(__FUNCTION__, __FILE__, ". Y coordinate for origin ");
	Logging::buildText(__FUNCTION__, __FILE__, this->origin.getY());
	Logging::buildText(__FUNCTION__, __FILE__, " and destination ");
	Logging::buildText(__FUNCTION__, __FILE__, this->destination.getY());
	Logging::buildText(__FUNCTION__, __FILE__, ". Line direction");
#endif

    // Only first or third quadrant.
	if (this->getSlopeType() != REAL_SLOPE)
	{
		if (INFINITE_POS_SLOPE == this->getSlopeType())
		{
			direction = HORIZONTAL_180;
		}
		else if (INFINITE_NEG_SLOPE == this->getSlopeType())
		{
			direction = VERTICAL_270;
		}
		else if (this->origin.getX() < this->destination.getX())
		{
			direction = HORIZONTAL_0;
		}
		else
		{
			direction = HORIZONTAL_180;
		}
	}
	else if (this->getSlope() > 0.0)
    {
        // First quadrant.
		if (this->origin.getY() < this->destination.getY())
        {
			if (this->getSlope() > 1.0)
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_45_TO_90.");
#endif
				direction = FROM_45_TO_90;
			}
			else
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_0_TO_45.");
#endif
				direction = FROM_0_TO_45;
			}
        }
        // Third quadrant.
        else
        {
			if (this->getSlope() > 1.0)
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_180_TO_225.");
#endif
				direction = FROM_180_TO_225;
			}
			else
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_135_TO_180.");
#endif
				direction = FROM_135_TO_180;
			}
        }
    }
    // (this->getSlope() < 0.0) -> Second or fourth quadrant
    else
    {
        // Fourth quadrant.
    	if (this->origin.getY() > this->destination.getY())
        {
			if (this->getSlope() < (-1.0))
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_270_TO_315.");
#endif
				direction = FROM_270_TO_315;
			}
			else
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_315_TO_360.");
#endif
				direction = FROM_315_TO_360;
			}
        }
        // Second quadrant.
        else
        {
        	if (this->getSlope() < (-1.0))
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_135_TO_180.");
#endif
				direction = FROM_135_TO_180;
			}
			else
			{
#ifdef LINE_GET_DIRECTION
				Logging::buildText(__FUNCTION__, __FILE__, " FROM_90_TO_135.");
#endif
				direction = FROM_90_TO_135;
			}
        }
    }

#ifdef LINE_GET_DIRECTION
	Logging::write(true, Info);
#endif
	return(direction);
}
