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
	inline void setValid(bool valid) { this->valid = valid; };
	void computeCircumcentres(bool isIncremental);
	void buildArea(int pointIndex);
	Point<TYPE> computeExtremeVoronoi(int edgeIndex, Point<TYPE> &centre);
	bool edgeExists(Edge &edge);
	bool isBottomMostFace(int faceId);
	static void setBorderPoints(int minX, int minY, int maxX, int maxY, Line *borders);

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
	void 	resize(int size, bool copy);
	void 	reset();

	// GET/SET functions.
	inline bool  isValid() { return(valid); };
	inline Dcel* getRefDcel() { return(&this->voronoi); };
	inline void  setDCEL(Dcel *dcel) {this->triangulation = dcel;};
	bool 		 getCentre(int areaId, Point<TYPE> &centre);

	// Figures functions.
	bool 	build(bool isIncremental);
	bool	findArea(Point<TYPE> &p);
	bool 	findPath(Line &line, Set<int> &faces);
	bool 	isInnerToArea(const Point<TYPE> &p, int areaId);
	void 	correctBorderPoints(int minX, int minY, int maxX, int maxY);
};

#endif /* INCLUDE_VORONOI_H_ */