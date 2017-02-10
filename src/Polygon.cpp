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
//#define DEBUG_POLYGON_IS_INTERNAL

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
* Name: 	getIntersections
* IN:		line		line to check
* OUT:		set			set of edges that intersect line.
* RETURN:	true 		if intersect.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	gets the set of edges(two maximum) that intersects the
* 				polygon.
***************************************************************************/
bool Polygon::getIntersections(Line &line, Set<int> &set)
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

	// Loop until polygon checked or both edges found.
	while ((set.getNElements() < 2) && (lineIndex < nSegments))
	{
		// Get next line.
		p = *this->set.at(lineIndex);
		q = *this->set.at(lineIndex+1);
		currentLine = Line( p, q);
#ifdef DEBUG_POLYGON_INTERSECT
		Logging::buildText(__FUNCTION__, __FILE__, "Checking segment index ");
		Logging::buildText(__FUNCTION__, __FILE__, lineIndex);
		Logging::write( true, Info);
#endif
		// Check if lines intersect.
		if (currentLine.intersect(line))
		{
			intersection = true;
			set.add(lineIndex);
#ifdef DEBUG_POLYGON_INTERSECT
			Logging::buildText(__FUNCTION__, __FILE__, "Intersection found");
			Logging::write( true, Info);
#endif
		}

		// Next line in polygon.
		lineIndex++;
	}

#ifdef DEBUG_POLYGON_INTERSECT
	if (!intersection)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Intersection NOT found.");
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Intersection finished.");
	}
	Logging::write( true, Info);
#endif

	return(intersection);
}

/***************************************************************************
* Name: 	isInternal
* IN:		p			point to check
* OUT:		NONE
* RETURN:	true 		if point is interior to polygon.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	Checks if the input point is interior to the polygon
***************************************************************************/
bool Polygon::isInternal(Point<TYPE> &p)
{
	bool isInternal=true;		// Return value.
	int	 i=0;					// Loop counter.
	int	 nElements=0;			// # elements to check in loop.
	Point<TYPE> currentPoint;
	Point<TYPE> nextPoint;

#ifdef DEBUG_POLYGON_IS_INTERNAL
	Logging::buildText(__FUNCTION__, __FILE__, "# lines in polygon is ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getNElements());
	Logging::write( true, Info);
#endif

	// Check all polygon segments.
	i = 0;
	nElements = this->getNElements() - 1;
	while ((i<nElements) && isInternal)
	{
		currentPoint = *this->at(i);
		i++;
		nextPoint = *this->at(i);

#ifdef DEBUG_POLYGON_IS_INTERNAL
		Logging::buildText(__FUNCTION__, __FILE__, "Checking line index ");
		Logging::buildText(__FUNCTION__, __FILE__, i);
		Logging::write( true, Info);
#endif

		// If RIGHT_TURN then it is not internal.
		if (currentPoint.check_Turn(nextPoint, p) == RIGHT_TURN)
		{
			isInternal = false;
		}
	}

	// Check if point already external.
	if (isInternal)
	{
		// Check line between first and last point and input point.
		currentPoint = *this->at(nElements);
		nextPoint = *this->at(0);
		if (currentPoint.check_Turn(nextPoint, p) == RIGHT_TURN)
		{
			isInternal = false;
		}
	}

	return(isInternal);
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
