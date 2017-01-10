/*
 * Face.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#include "defines.h"
#include "Face.h"

#include <sstream>
using namespace std;

/*------------------------------------------------------------------------
  Constructor/Destructor.
------------------------------------------------------------------------*/
Face::Face()
{
	// Update object attributes.
	this->edge = INVALID;
	//this->imaginaryFace = false;
}

//Face::Face( int edge, bool imaginary)
//{
//	// Update object attributes.
//	this->edge = edge;
//	this->imaginaryFace = imaginaryFace;
//}

Face::Face( int edge)
{
	// Update object attributes.
	this->edge = edge;
}


//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
///***************************************************************************
//* Name: set
//* IN:		edge			edge value
//* 			imaginaryFace	flag to indicate if face is imaginary
//* OUT:		NONE
//* RETURN:	NONE
//* GLOBAL:	NONE
//* Description: updates face fields
//***************************************************************************/
//void Face::set( int edge, bool imaginaryFace)
//{
//	// Update object attributes.
//	this->edge = edge;
//	this->imaginaryFace = imaginaryFace;
//}



/***************************************************************************
* Name: 	print
* IN:		out			stream to print data.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: print face data.
***************************************************************************/
void Face::print( std::ostream& out)
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
	this->print( oss);
	text = oss.str();

	return(text);
}


