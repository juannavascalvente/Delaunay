/*
 * Triangulation.h
 *
 *  Created on: Jul 22, 2016
 *      Author: jnavas
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include "Dcel.h"
#include "Polygon.h"
#include "Stack.h"

struct ConvexPoint
{
	int		vertexIndex;
	int		edgeID;
};

/****************************************************************************
// 						TRIANGULATION CLASS DEFITNION
****************************************************************************/
class Triangulation
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	Dcel 	*dcel;					// Reference to DCEL data.
	Polygon *hull;					// Convex hull polygon.

	bool	*edgeChecked;			// Edges checked flag array.
	int		nPending;				// # pending edges.

	bool	convexHullComputed;		// Convex hull computed flag.

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	bool setNotChecked( int index);

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Triangulation();
	~Triangulation();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	inline void setDCEL( Dcel *dcel) {this->dcel = dcel;};

	// Figures functions.
	bool convexHull();
	inline Polygon* getConvexHull() {return(this->hull);};

	bool findTwoClosest( int &first, int &second);
	bool findFace( Point<TYPE> &point, int &faceId);
	bool findClosestPoint( Point<TYPE> &p, Point<TYPE> &q, double &distance);

	bool build();
	bool delaunay();
};

#endif /* TRIANGULATION_H_ */
