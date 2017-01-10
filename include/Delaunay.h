/*
 * Delaunay.h
 *
 *  Created on: Jul 11, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_DELAUNAY_H_
#define INCLUDE_DELAUNAY_H_

#include "Dcel.h"
#include "defines.h"
#include "Graph.h"
#include "Polygon.h"
#include "Voronoi.h"

//****************************************************************************
//                           ENUM DEFINITION
//****************************************************************************
// Algorithm used to compute the Delaunay triangulation.
enum Algorithm { NONE, INCREMENTAL, FROM_STAR};

//****************************************************************************
//                           	DELAUNAY CLASS
//****************************************************************************
class Delaunay
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	Dcel 	*dcel;				// Reference to DCEL data.
	Graph 	*graph;				// Graph used in incremental algorithm.

	bool 	graphAllocated;		// Allocated memory for graph flag.

	// Convex hull data.
	bool 	convexHullComputed;
	Polygon hull;
	Set<int> hullEdges;

	// Type of algorithm executed to compute the Delaunay algorithm.
	enum Algorithm algorithm;

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	bool 	initializeGraph();
	void 	checkEdge( int edge_ID);
	void 	flipEdges( int edge_ID);
	bool 	insertPoint(int index);
	bool 	locateNode( Point<TYPE> &point, int &nodeIndex);
	bool 	isInteriorToNode( Point<TYPE> &point, int nodeIndex);
	bool 	isStrictlyInteriorToNode( Point<TYPE> &point, int nodeIndex);
	void 	splitNode( int pointIndex, int nodeIndex, int nTriangles);
	double	signedArea( Node *node);

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Delaunay() : hull(DEFAUTL_CONVEXHULL_LEN), hullEdges(DEFAUTL_CONVEXHULL_LEN)
	{
		this->algorithm = NONE;
		this->dcel = NULL;
		this->graph = NULL;
		this->graphAllocated = false;
		this->convexHullComputed = false;
	}
	Delaunay( Dcel *dcel) : hull(DEFAUTL_CONVEXHULL_LEN), hullEdges(DEFAUTL_CONVEXHULL_LEN)
	{
		this->algorithm= NONE;
		this->dcel = dcel;
		this->graphAllocated = true;
		this->graph = new Graph( this->dcel->getNVertex()*10);
		this->convexHullComputed = false;
	}
	~Delaunay();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void reset();
	bool incremental();

	// Get/Set functions.
	inline bool isGraphAllocated() {return(this->graphAllocated);};
	inline bool isConvexHullComputed() {return(this->convexHullComputed);};
	inline void setGraphAllocated(bool v) {this->graphAllocated = v;};
	inline void setConvexHullComputed(bool v) {this->convexHullComputed = v;};

	// Figures functions.
	bool convexHull();
	inline Polygon* getConvexHull() {return(&this->hull);};
	inline Set<int>* getConvexHullEdges() {return(&this->hullEdges);};
	bool findTwoClosest( int &first, int &second);
	bool findFace( Point<TYPE> &point, int &faceId);
	bool findClosestPoint(Point<TYPE> &p, Voronoi &voronoi, Point<TYPE> &q,
															int	&poinIndex,
															double &dist);
	bool findClosestPoint( Point<TYPE> &p, int nAnchors, Point<TYPE> &q,
														double &distance);
	bool findPath(Line &line, Set<int> &faces);

	// GET/SET functions.
	inline void setDCEL( Dcel *dcel) {this->dcel = dcel;};
	inline Dcel *getDCEL() {return(this->dcel); };
	inline void setAlgorithm( enum Algorithm type) {this->algorithm = type;};
	inline enum Algorithm getAlgorithm( ) {return(this->algorithm);};

	// I/O functions.
	void print();
	void print(Node *node);
	bool read(string fileName, string graphFileName);
	bool write(string fileName, string graphFileName);
};

#endif /* INCLUDE_DELAUNAY_H_ */
