/*
 * Edge.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#include "Edge.h"
#include "defines.h"

#include <sstream>
using namespace std;

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Edge::Edge()
{
	// Set input values.
	this->origin = INVALID;
	this->twin = INVALID;
	this->previous = INVALID;
	this->next = INVALID;
	this->face = INVALID;
}

Edge::Edge( int origin, int twin, int prev, int next, int face)
{
	// Set input values.
	this->origin = origin;
	this->twin = twin;
	this->previous = prev;
	this->next = next;
	this->face = face;
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: set
* IN:		origin		origin point
* 			twin		twin edge
* 			previous	previous edge
* 			next		next edge
* 			face		edge face
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: starts infinite loop.
***************************************************************************/
void Edge::set( int origin, int twin, int previous, int next, int face)
{
	// Set input values.
	this->origin = origin;
	this->twin = twin;
	this->previous = previous;
	this->next = next;
	this->face = face;
}


/***************************************************************************
* Name: reset
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: reset fields to INVALID
***************************************************************************/
void Edge::reset()
{
	// Reset object values.
	this->origin = INVALID;
	this->twin = INVALID;
	this->previous = INVALID;
	this->next = INVALID;
	this->face = INVALID;
}


/***************************************************************************
* Name: 	print
* IN:		out		output stream.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: prints edge data.
***************************************************************************/
void Edge::print( std::ostream& out)
{
	// Print edge information.
	out << this->origin << " " << this->twin << " " << this->previous <<
			" " << this->next << " " << this->face << std::endl;
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	edge information as a string
* GLOBAL:	NONE
* Description: 	concat the edge information as a string
***************************************************************************/
string Edge::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print( oss);
	text = oss.str();

	return(text);
}



