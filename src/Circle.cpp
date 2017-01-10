/*
 * Circle.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: jnavas
 */

#include "Circle.h"
#include "Logging.h"

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_CIRCLE_CONSTRUCTOR
//#define DEBUG_COMPUTE_CENTRE
//#define DEBUG_CIRCLE_INCIRCLE
#endif

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Circle::Circle( )
{
	// Initialize fields.
	this->centre.setOrigin();
	this->radius = 0.0;
}

Circle::Circle( Point<TYPE> *centre, TYPE radius)
{
	// Initialize fields.
	this->centre = (*centre);
	this->radius = radius;
}


Circle::Circle( Point<TYPE> *p,	Point<TYPE> *q, Point<TYPE> *r)
{
	// Initialize fields.
	this->p = *p;
	this->q = *q;
	this->r = *r;
	computeCentre();
	this->radius = this->centre.distance( *q);

#ifdef DEBUG_CIRCLE_CONSTRUCTOR
	Logging::buildText(__FUNCTION__, __FILE__, "Circle points are: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->p.toStr());
	Logging::buildText(__FUNCTION__, __FILE__, this->q.toStr());
	Logging::buildText(__FUNCTION__, __FILE__, this->r.toStr());
	Logging::buildText(__FUNCTION__, __FILE__, "Circle centre is: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->centre.toStr());
	Logging::buildText(__FUNCTION__, __FILE__, "Radius is: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->radius);
	Logging::write( true, Info);
#endif
}

/***************************************************************************
* Name: 		inCircle
* IN:			s		 point to check
* OUT:			NONE
* IN/OUT:		NONE
* RETURN:		NONE
* GLOBAL:		NONE
* Description: 	Checks if the input point "s" is in inside the circle formed
* 				by points p-q-r.
***************************************************************************/
bool Circle::inCircle( Point<TYPE> *s)
{
#ifdef DEBUG_CIRCLE_INCIRCLE
	Logging::buildText(__FUNCTION__, __FILE__, "Circle points are: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->p.toStr());
	Logging::write( true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, this->q.toStr());
	Logging::write( true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, this->r.toStr());
	Logging::write( true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, ". Point to check is: ");
	Logging::buildText(__FUNCTION__, __FILE__, s->toStr());
	Logging::write( true, Info);
#endif
	bool	inCircle=false;
	double 	value=0.0;		// Determinant value.
	double 	temp[9];		// Intermediate values.

	// Compute Ax - Dx, Ay - Dy and (Ax-Dx)² + (Ay-Dy)²
	temp[0] = (this->p.getX() - s->getX());
	temp[1] = (this->p.getY() - s->getY());
	temp[2] = (pow((this->p.getX() - s->getX()), 2) + pow((this->p.getY() - s->getY()), 2));

	// Compute Bx - Dx, By - Dy and (Bx-Dx)² + (By-Dy)²
	temp[3] = (this->q.getX() - s->getX());
	temp[4] = (this->q.getY() - s->getY());
	temp[5] = (pow((this->q.getX() - s->getX()), 2) + pow((this->q.getY() - s->getY()), 2));

	// Compute Cx - Dx, Cy - Dy and (Cx-Dx)² + (Cy-Dy)²
	temp[6] = (this->r.getX() - s->getX());
	temp[7] = (this->r.getY() - s->getY());
	temp[8] = (pow((this->r.getX() - s->getX()), 2) + pow((this->r.getY() - s->getY()), 2));

	// Compute determinant.
	value = (temp[0]*temp[4]*temp[8]) +
			(temp[1]*temp[5]*temp[6]) +
			(temp[2]*temp[3]*temp[7]) -
			(temp[2]*temp[4]*temp[6]) -
			(temp[5]*temp[7]*temp[0]) -
			(temp[8]*temp[1]*temp[3]);

	// If positive then point "s" belongs to p-q-r circumference.
	if (value > 0.0)
	{
		inCircle = true;
	}

	return(inCircle);
}

/***************************************************************************
* Name: 		computeCentre
* IN:			NONE
* OUT:			NONE
* IN/OUT:		NONE
* RETURN:		NONE
* GLOBAL:		this->centre
* Description: 	Computes the circumcentre of the triangle formed by the
* 				three points of the triangle and updates the "centre"
* 				attribute.
***************************************************************************/
void Circle::computeCentre()
{
	TYPE 	x1=0.0, y1=0.0;
	TYPE  	x2=0.0, y2=0.0;
	TYPE 	slope1=0.0, slope2=0.0;
	TYPE  	n1=0.0, n2=0.0;
	int		valid1=0, valid2=0;

	// Get point between two triangle vertex.
	x1 = (this->p.getX() + this->q.getX()) / 2.0;
	y1 = (this->p.getY() + this->q.getY()) / 2.0;

	// Compute slope.
	if ((this->p.getX() - this->q.getX()) != 0)
	{
		slope1 = (this->p.getY() - this->q.getY()) /
				(this->p.getX() - this->q.getX());
#ifdef DEBUG_COMPUTE_CENTRE
		Logging::buildText(__FUNCTION__, __FILE__, "Slope between 1st and 2nd points is ");
		Logging::buildText(__FUNCTION__, __FILE__, slope1);
		Logging::buildText(__FUNCTION__, __FILE__, ".Points coordinates are ");
		Logging::buildText(__FUNCTION__, __FILE__, &p);
		Logging::buildText(__FUNCTION__, __FILE__, " and ");
		Logging::buildText(__FUNCTION__, __FILE__, &q);
		Logging::write( true);
#endif
		if (slope1 == 0)
		{
			valid1 = 0;
		}
		else
		{
			valid1 = 1;
			slope1 = -(1/slope1);

			// Compute n1.
			n1 = y1 - slope1*x1;
		}
	}
	else
	{
		valid1 = 1;
		slope1 = 0;
		n1 = y1;
	}

	// Get point between two triangle vertex.
	x2 = (this->q.getX() + this->r.getX()) / 2.0;
	y2 = (this->q.getY() + this->r.getY()) / 2.0;

	// Compute slope.
	if ((this->q.getX() - this->r.getX()) != 0)
	{
		slope2 = (this->q.getY() - this->r.getY()) /
				(this->q.getX() - this->r.getX());
#ifdef DEBUG_COMPUTE_CENTRE
		Logging::buildText(__FUNCTION__, __FILE__, "Slope between 2nd and 3rd points is ");
		Logging::buildText(__FUNCTION__, __FILE__, slope2);
		Logging::buildText(__FUNCTION__, __FILE__, ".Points coordinates are ");
		Logging::buildText(__FUNCTION__, __FILE__, &q);
		Logging::buildText(__FUNCTION__, __FILE__, " and ");
		Logging::buildText(__FUNCTION__, __FILE__, &r);
		Logging::write( true);
#endif
		if (slope2 == 0)
		{
			valid2 = 0;
		}
		else
		{
			valid2 = 1;
			slope2 = -(1/slope2);

			// Compute n2.
			n2 = y2 - slope2*x2;
		}
	}
	else
	{
		valid2 = 1;
		slope2 = 0;
		n2 = y2;
	}

	if (valid1)
	{
		if (valid2)
		{
			// Compute x.
			this->centre.setX( (n2 - n1) / (slope1 - slope2));

			// Compute y.
			this->centre.setY( this->centre.getX()*slope1 + n1);
		}
		else
		{
			this->centre.setX( x2);
			this->centre.setY( this->centre.getX()*slope1 + n1);
		}
	}
	else
	{
		this->centre.setX( x1);
		this->centre.setY( this->centre.getX()*slope2 + n2);
	}

#ifdef DEBUG_COMPUTE_CENTRE
	Logging::buildText(__FUNCTION__, __FILE__, "Circle centre is ");
	Logging::buildText(__FUNCTION__, __FILE__, this->centre.toStr());
	Logging::write( true);
#endif
}
