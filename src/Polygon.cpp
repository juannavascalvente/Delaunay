/*
 * Polygon.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

#include "Polygon.h"
#include <string.h>

//#define DEBUG_POLYGON_CENTROID
//#define DEBUG_POLYGON_INTERSECT

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
//Polygon::Polygon(int n)
//{
//	try
//	{
//		// Create set.
//		this->set = Set<PointSet>( n);
//	}
//	catch (exception &ex)
//	{
//		ex.what();
//	}
//}

Polygon::Polygon(int n, Point<TYPE> *points)
{
	int		i=0;			// Loop counter.

	try
	{
		// Create set.
		this->set = Set<PointSet>( n);

		// Insert all input points to the set.
		for (i=0; i<n ;i++)
		{
			this->set.add( points[i]);
		}
	}
	catch (exception &ex)
	{
		ex.what();
	}
}

//--------------------------------------------------------------------------
// Public functions.
//--------------------------------------------------------------------------
/***************************************************************************
* Name: 	resize
* IN:		size			new size
* 			copy			flag to copy data to new allocated memory
* OUT:		NONE
* RETURN:	this function must be overwritten by child class.
* GLOBAL:	NONE
* Description: 	returns 0.0 as this function must be implemented by child
* 				class
***************************************************************************/
void Polygon::resize( int size, bool copy)
{
	try
	{
		// Check if current size is enough.
		if (this->set.getSize() < size)
		{
			// Resize set of points.
			this->set.resize( size, copy);
		}
	}
	catch (bad_alloc &ex)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory. New polygon size is ");
		Logging::buildText(__FUNCTION__, __FILE__, size);
		Logging::write( true, Error);
	}
	catch (exception &ex)
	{
		std::cout << ex.what();
	}
}


/***************************************************************************
* Name: 	area
* IN:		NONE
* OUT:		NONE
* RETURN:	this function must be overwritten by child class.
* GLOBAL:	NONE
* Description: 	returns 0.0 as this function must be implemented by child
* 				class
***************************************************************************/
double Polygon::area()
{
	double area=0.0;

	return(area);
}


/***************************************************************************
* Name: 	NONE
* IN:		NONE
* OUT:		NONE
* RETURN:	Polygon perimeter
* GLOBAL:	NONE
* Description: 	computes the perimeter of the Polygon
***************************************************************************/
double Polygon::perimeter()
{
	int	i=0;				// Loop counter.
	double perimeter=0.0;	// Return value.
	Point<TYPE> *p, *q;		// Temporary points.

	// Computes distance between every pair of points.
	for (i=0; i<(this->set.getNElements()-1) ;i++)
	{
		p = this->set.at( i);
		q = this->set.at( i+1);
		perimeter += p->distance( *q);
	}

	// Distance between last and first points.
	p = this->set.at( this->set.getNElements());
	q = this->set.at( 0);
	perimeter += p->distance( *q);

	return(perimeter);
}


/***************************************************************************
* Name: 	centroid
* IN:		NONE
* OUT:		center		polygon centroid.
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	computes the centroid of the polygon.
***************************************************************************/
void Polygon::centroid( Point<TYPE> &center)
{
	int	i=0;			// Loop counter.
	TYPE div=0.0;		// Number of elements in TYPE format.
	Point<TYPE> point;	// Current point.

	// Initialize point.
	center.setOrigin();

	// Add all points in polygon.
	for (i=0; i<this->getNElements() ;i++)
	{
		point = *this->at(i);
		center = center + point;
#ifdef DEBUG_POLYGON_CENTROID
		Logging::buildText(__FUNCTION__, __FILE__, "New point is ");
		Logging::buildText(__FUNCTION__, __FILE__, &point);
		Logging::write( true, Info);
		Logging::buildText(__FUNCTION__, __FILE__, "Current centroid is ");
		Logging::buildText(__FUNCTION__, __FILE__, &center);
		Logging::write( true, Info);
#endif
	}

	div = (TYPE) this->getNElements();
	center = center / div;
#ifdef DEBUG_POLYGON_CENTROID
	Logging::buildText(__FUNCTION__, __FILE__, "Final centroid is ");
	Logging::buildText(__FUNCTION__, __FILE__, &center);
	Logging::write( true, Info);
#endif
}

/***************************************************************************
* Name: 	intersect
* IN:		line		line to check
* OUT:		nLine		number of the segment that intersects the line.
* RETURN:	true if intersect.
* GLOBAL:	NONE
* Description: 	Checks if the input line intersect any of the lines of the
* 				polygon.
***************************************************************************/
bool Polygon::intersect(Line &line, int &nLine)
{
	bool intersection=false;		// Return value.
	int lineIndex=0;				// Loop counter.
	Line currentLine;				// Current line in polygon.
	Point<TYPE> p, q;				// Line points.
	int	nSegments=0;				// # segments in polygon.

	// Compute # segments in polygon.
	nSegments = this->getNElements()-1;
#ifdef DEBUG_POLYGON_INTERSECT
	Logging::buildText(__FUNCTION__, __FILE__, "Number of lines in polyline ");
	Logging::buildText(__FUNCTION__, __FILE__, nSegments);
	Logging::write( true, Info);
#endif

	// Loop until intersection found or no more lines to check.
	while ((!intersection) && (lineIndex < nSegments))
	{
		// Get next line.
		p = *this->set.at(lineIndex);
		lineIndex++;
		q = *this->set.at(lineIndex);
		currentLine = Line( p, q);
#ifdef DEBUG_POLYGON_INTERSECT
		Logging::buildText(__FUNCTION__, __FILE__, "Checking segment ");
		Logging::buildText(__FUNCTION__, __FILE__, lineIndex);
		Logging::write( true, Info);
#endif
		// Check if lines intersect.
		if (currentLine.intersect( line))
		{
			intersection = true;
			nLine = lineIndex;
#ifdef DEBUG_POLYGON_INTERSECT
			Logging::buildText(__FUNCTION__, __FILE__, "Intersection found in segment ");
			Logging::buildText(__FUNCTION__, __FILE__, lineIndex);
			Logging::write( true, Info);
#endif
		}
	}

#ifdef DEBUG_POLYGON_INTERSECT
	if (!intersection)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Intersection NOT found.");
	}
#endif

	return(intersection);
}

/***************************************************************************
* Name: 	print
* IN:		out			output stream
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	print the set of points.
***************************************************************************/
void Polygon::print(std::ostream& out)
{
	int	i=0;			// Loop counter.
	Point<TYPE> *p;		// Temporary points.

	// Print all points.
	for (i=0; i<this->set.getNElements() ;i++)
	{
		p = this->set.at( i);
		p->print( out);
	}
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	string		set of points as text.
* GLOBAL:	NONE
* Description: 	convert to string the set of points
***************************************************************************/
string Polygon::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print( oss);
	text = oss.str();

	return(text);
}
