/*
 * Face.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#include "defines.h"
#include "Dcel/Face.h"

#include <sstream>
using namespace std;

/****************************************************************************
//	 						DEFINES SECTION
****************************************************************************/
#define INVALID_FACE		(-1)


/*------------------------------------------------------------------------
  Constructor/Destructor.
------------------------------------------------------------------------*/
Face::Face()
{
	// Update object attributes.
	this->edge = INVALID_FACE;
}


Face::Face(int edge)
{
	// Update object attributes.
	this->edge = edge;
}


//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	isInvalid
* IN:		NONE
* OUT:		NONE
* RETURN:	true		if edge is invalid
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: checks if face has an invalid edge
***************************************************************************/
bool Face::isInvalid(void)
{
	return (this->edge == INVALID_FACE);
}

/***************************************************************************
* Name: 	print
* IN:		out			stream to print data.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: print face data.
***************************************************************************/
void Face::print(std::ostream& out) const
{
	out << this->edge << std::endl;
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	face information as a string
* GLOBAL:	NONE
* Description: 	concat the face information as a string
***************************************************************************/
string Face::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print(oss);
	text = oss.str();

	return(text);
}


