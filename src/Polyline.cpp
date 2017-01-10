/*
 * Polyline.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

#include "Polyline.h"
#include <string.h>

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Polyline::Polyline(int n)
{
	try
	{
		// Create set.
		this->set = Set<Line>( n);
	}
	catch (exception &ex)
	{
		ex.what();
	}
}

Polyline::Polyline(int n, Point<TYPE> *points)
{
	int		i=0;			// Loop counter.
	Line	line;			// Line to add.

	try
	{
		// Create set.
		this->set = Set<Line>( n);

		// Insert all input points to the set.
		for (i=0; i<n ;i=i+2)
		{
			line = Line( points[i], points[i+1]);
			this->set.add( line);
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
* Name: 		length
* IN:			NONE
* OUT:			NONE
* RETURN:		Polyline length
* GLOBAL:		NONE
* Description: 	computes the length of the Polyline
***************************************************************************/
double Polyline::length()
{
	int	i=0;				// Loop counter.
	double perimeter=0.0;	// Return value.
	Line *line;				// Current line.

	// Computes distance between every pair of points.
	for (i=0; i<(this->set.getNElements()-1) ;i++)
	{
		line = this->set.at( i);
		perimeter += line->getOriginRef()->distance( *line->getDestRef());
	}

	return(perimeter);
}


/***************************************************************************
* Name: 	print
* IN:		out			output stream
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	print the set of points.
***************************************************************************/
void Polyline::print(std::ostream& out)
{
	int	i=0;			// Loop counter.
	Line *line;			// Current line.

	// Print all points.
	for (i=0; i<this->set.getNElements() ;i++)
	{
		line = this->set.at( i);
		line->print( out);
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
string Polyline::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print( oss);
	text = oss.str();

	return(text);
}
