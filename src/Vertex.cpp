/*
 * Vertex.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: jnavas
 */

#include "defines.h"
#include "Vertex.h"

#include <iostream>
using namespace std;

/*****************************************************************************
 * Name: 		random
 * Input: 		NONE
 * Description: generate a random point coordinates and set edge as INVALID.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
void Vertex::random()
{
	// Read point coordinates.
	this->point.random();

	// Set edge as invalid.
	this->originEdge = INVALID;
}


/*****************************************************************************
 * Name: 		shake
 * Input: 		NONE
 * Description: shakes point coordinates
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
void Vertex::shake()
{
	// Read point coordinates.
	this->point.shake();
}

/*****************************************************************************
 * Name: 		read
 * Input: 		ifs		input file stream.
 * Description: reads x and y coordinates and set edge as INVALID.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
int	Vertex::read( ifstream ifs)
{
	int	ret=SUCCESS;		// Return value.

	// Read point coordinates.
	this->point.read( ifs);

	// Set edge as invalid.
	this->originEdge = INVALID;

	return(ret);
}

/*****************************************************************************
 * Name: 		write
 * Input: 		ofs		output file stream.
 * Description: writes x and y coordinates.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
int	Vertex::write( ofstream ofs)
{
	int	ret=SUCCESS;		// Return value.

	// Read point coordinates.
	this->point.write( ofs);

	return(ret);
}

/*****************************************************************************
 * Name: 		print
 * Input: 		out		output stream.
 * Description: writes x and y coordinates and edge to out stream.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
void Vertex::print( std::ostream& out)
{
	this->point.print(out);
	out << " " << this->originEdge << std::endl;
}

/*****************************************************************************
 * Name: 		printPoint
 * Input: 		out		output stream.
 * Description: writes x and y coordinates to out stream.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
void Vertex::printPoint( std::ostream& out)
{
	// Print point.
	this->point.print(out);
}

