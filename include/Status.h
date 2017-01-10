/*
 * Status.h
 *
 *  Created on: Jul 5, 2016
 *      Author: juan
 */

#ifndef INCLUDE_STATUS_H_
#define INCLUDE_STATUS_H_

/****************************************************************************
// 						STATUS CLASS DEFITNION
****************************************************************************/
class Status
{
	/*------------------------------------------------------------------------
	  Attributes
	------------------------------------------------------------------------*/
	bool	firstTime;			// First time execution flag.
	bool	hasSet;				// Set of points created or read flag.
	bool	hasTriangulation;	// Star triangulation created flag.
	bool	hasDelaunay;		// Delaunay triangulation created flag.
	bool	hasVoronoi;			// Voronoi diagram computed flag.
	bool	hasGabriel;			// Gabriel graph computed flag.

public:
	/*------------------------------------------------------------------------
	  Constructor/Destructor.
	------------------------------------------------------------------------*/
	Status();

	/*------------------------------------------------------------------------
	  Public functions.
	------------------------------------------------------------------------*/
	void set( bool first, bool set, bool triangulation, bool delaunay, bool voronoi, bool hasGabriel);
	inline void reset() { this->set(true, false, false, false, false, false); };

	// Get / Set.
	inline bool isFirstTime(void) { return(this->firstTime);};
	inline bool isSetCreated(void) { return(this->hasSet);};
	inline bool isTriangulationCreated(void) { return(this->hasTriangulation);};
	inline bool isDelaunayCreated(void) { return(this->hasDelaunay);};
	inline bool isVoronoiCreated(void) { return(this->hasVoronoi);};
	inline bool isGabrielCreated(void) { return(this->hasGabriel);};

	inline void setFirstTime(bool value) { this->firstTime = value;};
	inline void setSetCreated(bool value) { this->hasSet = value;};
	inline void seTriangulationCreated(bool value) { this->hasTriangulation = value;};
	inline void setDelaunayCreated(bool value) { this->hasDelaunay = value;};
	inline void setVoronoiCreated(bool value) { this->hasVoronoi = value;};
	inline void setGabrielCreated(bool value) { this->hasGabriel = value;};
};

#endif /* INCLUDE_STATUS_H_ */
