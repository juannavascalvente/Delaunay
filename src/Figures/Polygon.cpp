/*
 * Polygon.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Figures/Polygon.h"

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

	// Loop until polygon checked or both edges found.
	while ((intersection.size() < 2) && (lineIndex < nSegments))
	{
		// Get next line.
		p = vPoints.at(lineIndex);
		q = vPoints.at(lineIndex + 1);
		currentLine = Line( p, q);

		// Check if lines intersect.
		if (currentLine.intersect(line))
		{
			isSuccess = true;
            intersection.push_back(lineIndex);
		}

		// Next line in polygon.
		lineIndex++;
	}

	return(isSuccess);
}


void Polygon::getIntersections(Line &line, vector<Point<TYPE>> &vOut)
{
    // Initialize output
    vOut.clear();

    // Compute # segments in polygon.
    int nSegments = this->getNElements();

    // Loop until all polygon segments have been checked
    int lineIndex=0;
    while (lineIndex < nSegments)
    {
        // Get next line
        Point<TYPE> p = vPoints.at(lineIndex);
        Point<TYPE> q = vPoints.at((lineIndex + 1) % nSegments);
        Line currentLine( p, q);

        // Check if lines intersect
        Point<TYPE> intersection;
        currentLine.getIntersection(line, intersection);
        vOut.push_back(intersection);

        // Next line in polygon
        lineIndex++;
    }
}


bool Polygon::isInternal(Point<TYPE> &p)
{
	bool isInternal=true;		// Return value

    // Get first turn
    Point<TYPE> currentPoint = vPoints.at(0);
    Point<TYPE> nextPoint = vPoints.at(1);
    Turn_T enFirstTurn = currentPoint.check_Turn(nextPoint, p);

	// Check all polygon segments
	size_t szIter=1;
	size_t szIndex=1;
	while ((szIter < this->getNElements()) && isInternal)
	{
	    // Get next edge extreme points
		currentPoint = vPoints.at(szIndex);
        szIndex++;
        szIndex = szIndex% this->getNElements();
		nextPoint = vPoints.at(szIndex);

		// Check all turns are in the same direction
		if (currentPoint.check_Turn(nextPoint, p) != enFirstTurn)
		{
			isInternal = false;
		}

		// Next segment
        szIter++;
	}

	return isInternal;
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
