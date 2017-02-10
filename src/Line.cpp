/*
 * Line.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#include "Line.h"
#include <float.h>
#include <limits.h>

//#define DEBUG_EXTEND_SEGMENT
//#define DEBUG_COMPUTE_SLOPE_N
//#define DEBUG_LINE_INTERSECT

//#define LINE_GET_DIRECTION
/*****************************************************************************
* 							LINE class implementation
*****************************************************************************/
Line::Line()
{
	// Initialize origin and destination points to (0,0).
	this->origin.setOrigin();
	this->destination.setOrigin();
	this->m = 0.0;
	this->n = 0.0;
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
void Line::extendToBoundary(Point<TYPE> &extreme)
{
	int			error;		// Error during intermediate computations.
    Direction_E direction; 	// Line direction.

    // Compute line m and n.
#ifdef DEBUG_EXTEND_SEGMENT
	Logging::buildText(__FUNCTION__, __FILE__, "Segment points are ");
	Logging::buildText(__FUNCTION__, __FILE__, &this->origin);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, &this->destination);
	Logging::write(true, Info);
#endif
	error = this->getSlopeAndN();
    if (error == SUCCESS)
    {
		// Compute direction of p-q line.
		direction = this->getDirection();

		// Line towards positive x coordinates values.
		if ((direction == FROM_0_TO_90) || (direction == FROM_270_TO_360))
		{
			extreme.setX(INT_MAX);

			// Set y value y=mx + n
			extreme.setY(this->getSlope()*extreme.getX() + this->getN());
		}
		// Line towards negative x coordinates values.
		else if ((direction == FROM_90_TO_180) || (direction == FROM_180_TO_270))
		{
			extreme.setX(INT_MIN);

			// Set y value y=mx + n
			extreme.setY(this->getSlope()*extreme.getX() + this->getN());
		}
		// Line is parallel to X coordinates axis towards right.
		else if (direction == HORIZONTAL_0)
		{
			extreme.setX(INT_MAX);
			extreme.setY(this->origin.getY());
		}
		// Line is parallel to X coordinates axis towards left.
		else if (direction == HORIZONTAL_180)
		{
			extreme.setX(INT_MIN);
			extreme.setY(this->origin.getY());
		}
		// Line is parallel to Y coordinates axis upwards.
		else if (direction == VERTICAL_90)
		{
			extreme.setX(this->origin.getX());
			extreme.setY(INT_MAX);
		}
		// Line is parallel to Y coordinates axis downwards.
		else
		{
			extreme.setX(this->origin.getX());
			extreme.setY(INT_MIN);
		}
    }
    else
    {
    	// PENDING: What to do if there is an error?
    	// Set common value.
		extreme.setX(this->origin.getX());

    	// Check error value.
    	if (error == -1)
    	{
			extreme.setY(-MAX_Y_COORD);
    	}
    	else
    	{
			extreme.setY(MAX_Y_COORD);
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
}

//--------------------------------------------------------------------------
// Private functions.
//--------------------------------------------------------------------------
/*****************************************************************************
 * 	FUNCTION:      	getSlopeAndN
 * 	DESCRIPTION:   	compute m and n values in y=mx + n.
 * 	INPUT:         	NONE
 * 	OUTPUT:			NONE
 * 	IN/OUTPUT:		NONE
 * 	PRE:            N/A
 * 	POST:           m = slope between origin and destination
 * 					n = n value in y=mx + n
 * 	COMPLEXITY:     O(1)
*****************************************************************************/
int Line::getSlopeAndN()
{
	int error=SUCCESS;	// Return value.
	TYPE divisor;

	// Avoid division by 0.
	divisor = this->destination.getX() - this->origin.getX();
	if (divisor != 0)
	{
		// Compute slope.
		this->setSlope((this->destination.getY() - this->origin.getY()) / divisor);

		// Compute n.
		this->setN(this->origin.getY() - (this->getSlope()*this->origin.getX()));
	}
	else
	{
		if (this->destination.getY() < this->origin.getY())
		{
			this->setSlope(-FLT_MAX);
			error = -1;
		}
		else
		{
			this->setSlope(-FLT_MAX);
			error = -2;
		}
#ifdef DEBUG_COMPUTE_SLOPE_N
			printf("Slope is +/- infinite and set to %f\n", this->getSlope());
#endif
	}

	return(error);
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
Direction_E Line::getDirection()
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
	if (this->getSlope() > 0)
    {
        // First quadrant.
		if (this->origin.getY() < this->destination.getY())
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " between [0,90].");
#endif
            direction = FROM_0_TO_90;
        }
        // Third quadrant.
        else
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " between [180,270].");
#endif
            direction = FROM_180_TO_270;
        }
    }
    // Second or fourth quadrant.
    else if (this->getSlope() < 0)
    {
        // First quadrant.
    	if (this->origin.getY() > this->destination.getY())
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " between [270,360].");
#endif
            direction = FROM_270_TO_360;
        }
        // Third quadrant.
        else
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " between [90,180].");
#endif
            direction = FROM_90_TO_180;
        }
    }
    // Slope is zero -> line parallel to X axis.
    else if (this->getSlope() == 0)
    {
        // Parallel to X axis to the right.
    	if (this->origin.getX() < this->destination.getX())
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " horizontal 0.");
#endif
            direction = HORIZONTAL_0;
        }
        // Parallel to X axis to the left.
        else
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " horizontal 180.");
#endif
            direction = HORIZONTAL_180;
        }
    }
    // Slope is infinite -> line parallel to Y axis.
    else
    {
        // Parallel to Y axis upwards.
    	if (this->origin.getY() < this->destination.getY())
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " vertical 90.");
#endif
            direction = VERTICAL_90;
        }
        // Parallel to Y axis downwards.
        else
        {
#ifdef LINE_GET_DIRECTION
			Logging::buildText(__FUNCTION__, __FILE__, " vertical 270.");
#endif
            direction = VERTICAL_270;
        }
    }
#ifdef LINE_GET_DIRECTION
	Logging::write(true, Info);
#endif
	return(direction);
}

