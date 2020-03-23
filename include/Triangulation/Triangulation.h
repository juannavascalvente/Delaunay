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
#ifdef STATISTICS_STAR_TRIANGULATION
	int 	nCollinear;				// # collinear points.
	int		nFlips;					// # flipped edges.
#endif

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	bool setNotChecked(int index);

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Triangulation();
	~Triangulation();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void reset();
	bool build(Dcel *dcel);
	bool delaunay();

	//------------------------------------------------------------------------
	// Figures functions.
	//------------------------------------------------------------------------
	bool convexHull();
	bool findTwoClosest(int &first, int &second);
	bool findFace(Point<TYPE> &point, int &faceId);
	bool findClosestPoint(Point<TYPE> &p, Point<TYPE> &q, double &distance);

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	bool isConvexHullComputed() const {return convexHullComputed;}
	Dcel* getDcel() const {return dcel;}
	Polygon* getConvexHull() const {return hull;}
#ifdef STATISTICS_STAR_TRIANGULATION
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
#endif
};

#endif /* TRIANGULATION_H_ */
