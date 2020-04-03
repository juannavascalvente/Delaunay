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
#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	int nFlips;
	int nCollinear;
	int *nNodesChecked;
	int nNodesCheckedIndex;
#endif
	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	bool initializeGraph();
	void checkEdge(int edge_ID);
	void flipEdges(int edge_ID);
	bool addPointToDelaunay(int index);
	bool locateNode(const Point<TYPE> &point, int &nodeIndex);
	bool isInteriorToNode(const Point<TYPE> &point, int nodeIndex);
	bool isStrictlyInteriorToNode(Point<TYPE> &point, int nodeIndex);
	void splitNode(int pointIndex, int nodeIndex, int nTriangles);
	double signedArea(Node *node);

//	void getInitialFaces(Line &line, Set<int> &edgesSet, int &initialFace, int &finalFace);
//	void getInternalFace(Line &line, Set<int> &edgesIndex, int &initialFace);

	friend class DelaunayIO;
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Delaunay() : dcel(nullptr), graph(nullptr), graphAllocated(false), \
					convexHullComputed(false), hull(DEFAUTL_CONVEXHULL_LEN), \
					hullEdges(DEFAUTL_CONVEXHULL_LEN), algorithm(NONE)  {}
	explicit Delaunay(Dcel *inDcel) : graphAllocated(true), convexHullComputed(false), \
						hull(DEFAUTL_CONVEXHULL_LEN), \
						hullEdges(DEFAUTL_CONVEXHULL_LEN), algorithm(NONE) \

	{
		// PENDING REPLACE 10 BY DEFAULT LENGTH DEPENDING ON NPOINTS.
		this->dcel = inDcel;
		this->graph = new Graph(this->dcel->getNVertex()*10);
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
	Graph* getGraph() {return graph;}

	// Figures functions.
	bool convexHull();
	//bool internalToConvexHull(Point<TYPE> &p);
	inline Polygon* getConvexHull() {return(&this->hull);};
	inline Set<int>* getConvexHullEdges() {return(&this->hullEdges);};
	bool findTwoClosest(int &first, int &second);
	bool findFace(Point<TYPE> &point, int &faceId, bool &isImaginary);
	bool findClosestPoint(const Point<TYPE> &p, Voronoi &voronoi, Point<TYPE> &q,
															int	&poinIndex,
															double &dist);
    //bool findClosestPoint(Point<TYPE> &p, int nAnchors, Point<TYPE> &q, double &distance);
	bool findPath(Line &line, vector<int> &vFacesId);

	// GET/SET functions.
	inline void setDCEL(Dcel *dcelIn) {this->dcel = dcelIn;};
	inline Dcel *getRefDcel() {return(this->dcel); };
	inline void setAlgorithm(enum Algorithm type) {this->algorithm = type;};
	inline enum Algorithm getAlgorithm() {return(this->algorithm);};

#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
	void freeStatistics();
	int* getNodesChecked() const {return nNodesChecked;}
#endif
};

#endif /* INCLUDE_DELAUNAY_H_ */
