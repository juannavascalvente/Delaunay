/*
 * Triangle.cpp
 *
 *  Created on: Jul 16, 2016
 *      Author: juan
 */

#include "Logging.h"
#include "Triangle.h"

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_CENTROID
#endif

/*------------------------------------------------------------------------
  Constructor/Destructor.
------------------------------------------------------------------------*/
Triangle::Triangle(Point <TYPE> p, Point <TYPE> q, Point <TYPE> r)
{
	// Copy points.
	this->vertex[0] = p;
	this->vertex[1] = q;
	this->vertex[2] = r;
}

Triangle::Triangle(TYPE x1, TYPE y1, TYPE x2, TYPE y2, TYPE x3, TYPE y3)
{
	// Copy points coordinates.
	this->vertex[0].setX(x1);
	this->vertex[0].setY(y1);
	this->vertex[1].setX(x2);
	this->vertex[1].setY(y2);
	this->vertex[2].setX(x3);
	this->vertex[2].setY(y3);
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	area
* IN:		NONE
* OUT:		NONE
* RETURN:	triangle area
* GLOBAL:	NONE
* Description: 	Computes the triangle area.
***************************************************************************/
double 	Triangle::area()
{
	double triangleArea=0.0;		// Return value.

	// Compute triangle area.
	triangleArea = this->vertex[0].signedArea( this->vertex[1], this->vertex[2])*2;

	return(triangleArea);
}

/***************************************************************************
* Name: 	perimeter
* IN:		NONE
* OUT:		NONE
* RETURN:	triangle perimeter.
* GLOBAL:	NONE
* Description: 	Computes the triangle perimeter.
***************************************************************************/
double Triangle::perimeter()
{
	double perimeter=0.0;

	// Compute triangle perimeter adding distances between vertices.
	perimeter = this->vertex[0].distance( this->vertex[1]);
	perimeter += this->vertex[1].distance( this->vertex[2]);
	perimeter += this->vertex[2].distance( this->vertex[0]);

	return(perimeter);
}

/***************************************************************************
* Name: 	centroid
* IN:		NONE
* OUT:		NONE
* RETURN:	triangle centroid.
* GLOBAL:	NONE
* Description: 	Computes the triangle centroid.
***************************************************************************/
void Triangle::centroid(Point<TYPE> &centroid)
{
	TYPE x=0.0, y=0.0;

	// Compute x coordinate.
	x = this->vertex[0].getX() + this->vertex[1].getX() + this->vertex[2].getX();
	x = x / 3.0;
	centroid.setX( x);

	// Compute x coordinate.
	y = this->vertex[0].getY() + this->vertex[1].getY() + this->vertex[2].getY();
	y = y / 3.0;
	centroid.setY( y);
}

/***************************************************************************
* Name: 	centroid
* IN:		NONE
* OUT:		NONE
* RETURN:	triangle centroid.
* GLOBAL:	NONE
* Description: 	Computes the triangle centroid formed by p-q-r.
***************************************************************************/
void Triangle::centroid(Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r, Point<TYPE> &c)
{
	TYPE x=0.0, y=0.0;

	// Compute x coordinate.
	x = p.getX() + q.getX() + r.getX();
	x = x / 3.0;
	c.setX( x);

	// Compute x coordinate.
	y = p.getY() + q.getY() + r.getY();
	y = y / 3.0;
	c.setY( y);

#ifdef DEBUG_CENTROID
	Logging::buildText( __FUNCTION__, __FILE__, "Triangle points are: ");
	Logging::write( true);
	Logging::buildText( __FUNCTION__, __FILE__, &p);
	Logging::buildText( __FUNCTION__, __FILE__, &q);
	Logging::buildText( __FUNCTION__, __FILE__, &r);
	Logging::write( true);
	Logging::buildText( __FUNCTION__, __FILE__, ". Centroid is: ");
	Logging::buildText( __FUNCTION__, __FILE__, &c);
	Logging::write( true);
#endif
}


/***************************************************************************
* Name: 	print
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints to standard output the triangle vertices.
***************************************************************************/
void	Triangle::print()
{
	std::cout << "Triangle vertex are:" << std::endl;
	this->vertex[0].print(std::cout);
	this->vertex[1].print(std::cout);
	this->vertex[2].print(std::cout);
}
