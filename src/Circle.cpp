/*
 * Circle.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: jnavas
 */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Circle.h"
#include "Logging.h"


/***********************************************************************************************************************
* Public class methods definitions
***********************************************************************************************************************/
Circle::Circle(Point<TYPE> *centre, TYPE radius)
{
	// Initialize fields
	this->centre = (*centre);
	this->radius = radius;

	// Add points in circle
	Point<TYPE> p(this->centre.getX(), this->centre.getY() - radius);
	vPoints.push_back(p);

	p.setX(this->centre.getX() + radius);
	p.setY(this->centre.getY());
    vPoints.push_back(p);

	p.setX(this->centre.getX());
	p.setY(this->centre.getY() + radius);
    vPoints.push_back(p);
}


bool Circle::inCircle(Point<TYPE> &p)
{
	bool	isInCircle=false;
	double 	temp[9];		// Intermediate values.

	// Compute Ax - Dx, Ay - Dy and (Ax-Dx)² + (Ay-Dy)²
	temp[0] = (vPoints.at(0).getX() - p.getX());
	temp[1] = (vPoints.at(0).getY() - p.getY());
	temp[2] = (pow((vPoints.at(0).getX() - p.getX()), 2) + pow((vPoints.at(0).getY() - p.getY()), 2));

	// Compute Bx - Dx, By - Dy and (Bx-Dx)² + (By-Dy)²
	temp[3] = (vPoints.at(1).getX() - p.getX());
	temp[4] = (vPoints.at(1).getY() - p.getY());
	temp[5] = (pow((vPoints.at(1).getX() - p.getX()), 2) + pow((vPoints.at(1).getY() - p.getY()), 2));

	// Compute Cx - Dx, Cy - Dy and (Cx-Dx)² + (Cy-Dy)²
	temp[6] = (vPoints.at(2).getX() - p.getX());
	temp[7] = (vPoints.at(2).getY() - p.getY());
	temp[8] = (pow((vPoints.at(2).getX() - p.getX()), 2) + pow((vPoints.at(2).getY() - p.getY()), 2));

	// Compute determinant.
    double value = (temp[0]*temp[4]*temp[8]) +
			(temp[1]*temp[5]*temp[6]) +
			(temp[2]*temp[3]*temp[7]) -
			(temp[2]*temp[4]*temp[6]) -
			(temp[5]*temp[7]*temp[0]) -
			(temp[8]*temp[1]*temp[3]);

	// If positive then point "s" belongs to p-q-r circumference.
	if (value > 0.0)
	{
		isInCircle = true;
	}

	return(isInCircle);
}


/**
 * @fn          computeCentre
 * @brief       Computes the circumcentre of the triangle formed by the
 * 				three points of the triangle and updates the "centre"
 * 				attribute.
 */
void Circle::computeCentre()
{
	TYPE 	slope1, slope2;
	TYPE  	n1=0.0, n2=0.0;
	int		valid1=0, valid2=0;

	// Get point between two triangle vertex.
    TYPE x1 = (vPoints.at(0).getX() + vPoints.at(1).getX()) / (float) 2.0;
    TYPE y1 = (vPoints.at(0).getY() + vPoints.at(1).getY()) / (float) 2.0;

	// Compute slope.
	if ((vPoints.at(0).getX() - vPoints.at(1).getX()) != 0)
	{
        slope1 = (vPoints.at(0).getY() - vPoints.at(1).getY()) /
				(vPoints.at(0).getX() - vPoints.at(1).getX());
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
    TYPE x2 = (vPoints.at(1).getX() + vPoints.at(2).getX()) / (float) 2.0;
    TYPE y2 = (vPoints.at(1).getY() + vPoints.at(2).getY()) / (float) 2.0;

	// Compute slope.
	if ((vPoints.at(1).getX() - vPoints.at(2).getX()) != 0)
	{
        slope2 = (vPoints.at(1).getY() - vPoints.at(2).getY()) /
				(vPoints.at(1).getX() - vPoints.at(2).getX());
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
