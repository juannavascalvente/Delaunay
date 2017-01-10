/*
 * Status.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: juan
 */

#include "Status.h"

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Status::Status()
{
	// Initialize attributes.
	this->firstTime = true;
	this->hasSet= false;
	this->hasTriangulation = false;
	this->hasDelaunay = false;
	this->hasVoronoi = false;
	this->hasGabriel = false;
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: set
* IN:		first			initial status flag
* 			set				set of points created flag
* 			triangulation	triangulation created flag
* 			delaunay		Delaunay triangulation created flag
* 			voronoi			Voronoi diagram created flag
* 			gabriel			Gabriel graph created flag.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: sets attributes to input values.
***************************************************************************/
void Status::set(bool first, bool set, bool triangulation, bool delaunay,
												bool voronoi, bool gabriel)
{
	// Update attributes.
	this->setFirstTime(first);
	this->setSetCreated(set);
	this->seTriangulationCreated(triangulation);
	this->setDelaunayCreated(delaunay);
	this->setVoronoiCreated(voronoi);
	this->setGabrielCreated(gabriel);
}
