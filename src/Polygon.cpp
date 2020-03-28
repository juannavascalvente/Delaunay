/*
 * Polygon.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Polygon.h"

//#define DEBUG_POLYGON_CENTROID
//#define DEBUG_POLYGON_INTERSECT
//#define DEBUG_POLYGON_IS_INTERNAL

/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
///***************************************************************************
//* Name: 	NONE
//* IN:		NONE
//* OUT:		NONE
//* RETURN:	Polygon perimeter
//* GLOBAL:	NONE
//* Description: 	computes the perimeter of the Polygon
//***************************************************************************/
//double Polygon::perimeter()
//{
//	int	i=0;				// Loop counter.
//	double perimeter=0.0;	// Return value.
//	Point<TYPE> p, q;		// Temporary points.
//
//	// Computes distance between every pair of points.
//	for (i=0; i<(vPoints.size()-1) ;i++)
//	{
//		p = vPoints.at( i);
//		q = vPoints.at( i+1);
//		perimeter += p.distance(q);
//	}
//
//	// Distance between last and first points.
//	p = vPoints.at(vPoints.size());
//	q = vPoints.at(0);
//	perimeter += p.distance(q);
//
//	return(perimeter);
//}


void Polygon::centroid(Point<TYPE> &center)
{
	// Initialize point.
	center.setOrigin();

	// Add all points in polygon.
	for (size_t i=0; i<this->getNElements() ;i++)
	{
        Point<TYPE> point = vPoints.at(i);
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

    TYPE div = (TYPE) this->getNElements();
	center = center / div;
#ifdef DEBUG_POLYGON_CENTROID
	Logging::buildText(__FUNCTION__, __FILE__, "Final centroid is ");
	Logging::buildText(__FUNCTION__, __FILE__, &center);
	Logging::write( true, Info);
#endif
}


bool Polygon::getIntersections(Line &line, vector<int> &intersection)
{
	bool isSuccess=false;		// Return value.
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
	while ((intersection.size() < 2) && (lineIndex < nSegments))
	{
		// Get next line.
		p = vPoints.at(lineIndex);
		q = vPoints.at(lineIndex + 1);
		currentLine = Line( p, q);
#ifdef DEBUG_POLYGON_INTERSECT
		Logging::buildText(__FUNCTION__, __FILE__, "Checking segment index ");
		Logging::buildText(__FUNCTION__, __FILE__, lineIndex);
		Logging::write( true, Info);
#endif
		// Check if lines intersect.
		if (currentLine.intersect(line))
		{
			isSuccess = true;
            intersection.push_back(lineIndex);
#ifdef DEBUG_POLYGON_INTERSECT
			Logging::buildText(__FUNCTION__, __FILE__, "Intersection found");
			Logging::write( true, Info);
#endif
		}

		// Next line in polygon.
		lineIndex++;
	}

#ifdef DEBUG_POLYGON_INTERSECT
	if (!isSuccess)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Intersection NOT found.");
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Intersection finished.");
	}
	Logging::write( true, Info);
#endif

	return(isSuccess);
}


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
		currentPoint = vPoints.at(i);
		i++;
		nextPoint = vPoints.at(i);

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
		currentPoint = vPoints.at(nElements);
		nextPoint = vPoints.at(0);
		if (currentPoint.check_Turn(nextPoint, p) == RIGHT_TURN)
		{
			isInternal = false;
		}
	}

	return(isInternal);
}


void Polygon::print(std::ostream& out)
{
	// Print all points.
	for (auto point : vPoints)
    {
	    point.print(out);
    }
}


string Polygon::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print( oss);
	text = oss.str();

	return(text);
}
