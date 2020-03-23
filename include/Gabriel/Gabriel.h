/*
 * Gabriel.h
 *
 *  Created on: Nov 11, 2016
 *      Author: juan
 */

#ifndef INCLUDE_GABRIEL_H_
#define INCLUDE_GABRIEL_H_

#include "Dcel.h"
#include "Voronoi.h"

#include <iostream>
using namespace std;

/****************************************************************************
//	 						GABRIEL CLASS DEFITNION
****************************************************************************/
class Gabriel
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	bool 	valid;				// Valid data flag.
	int	 	size;				// Size of the bool array.
	bool 	*validEdge;			// Valid edge array.
	Dcel 	*dcel;				// DCELto check.
	Voronoi *voronoi;			// Voronoi graph associated to DCEL.

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	void validateEdges( int edgeIndex);
	bool resize( int size, bool copy);

	friend class GabrielIO;
public:

	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Gabriel();
	Gabriel( Dcel *dcel, Voronoi *voronoi);
	~Gabriel();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void init( Dcel *dcel, Voronoi *voronoi);

	inline bool isSet( int index) { return(this->validEdge[index]); };
	inline void set( int index) { this->validEdge[index] = true; };
	inline void unset( int index) { this->validEdge[index] = false; };
	inline bool at( int index) const { return(this->validEdge[index]); };

	// Get/Set functions.
	inline bool isValid() {return(this->valid); };
	inline int  getSize() const {return(this->size); };
	inline Dcel *getDcel() {return(this->dcel); };

	bool build();
};

#endif /* INCLUDE_GABRIEL_H_ */
