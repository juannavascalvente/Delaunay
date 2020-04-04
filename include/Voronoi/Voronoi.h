/*
 * Voronoi.h
 *
 *  Created on: Aug 3, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_VORONOI_H_
#define INCLUDE_VORONOI_H_

#include "Dcel.h"
#include "Line.h"


/****************************************************************************
// 						VORONOI CLASS DEFITNION
****************************************************************************/
class Voronoi
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	bool 	valid;
	Dcel 	*triangulation;			// Reference to triangulation DCEL data.
	Dcel 	voronoi;				// Reference to Voronoi DCEL data.

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	void computeCircumcentres(bool isIncremental);
	void buildArea(int pointIndex);
	Point<TYPE> computeExtremeVoronoi(int edgeIndex, Point<TYPE> &centre);
	bool edgeExists(Edge &edge);
	bool isBottomMostFace(int faceId);

	friend class VoronoiIO;
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	// PENDING IMPLICIT CONSTRUCTORS.
	Voronoi();
	explicit Voronoi(Dcel *dcel);
	~Voronoi();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool 	init(Dcel *dcel);
	void 	reset();

	// GET/SET functions.
	inline Dcel* getRefDcel() { return(&this->voronoi); };
	bool 		 getCentre(int areaId, Point<TYPE> &centre);

	// Figures functions.
	bool 	build(bool isIncremental);
	bool 	isInnerToArea(const Point<TYPE> &p, int areaId);
};

#endif /* INCLUDE_VORONOI_H_ */
