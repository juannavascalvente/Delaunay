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


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Delaunay
{
    /*******************************************************************************************************************
    * Private class members
    *******************************************************************************************************************/
	Dcel 	dcel;				// Reference to DCEL data.
	Graph 	graph;				// Graph used in incremental algorithm.

	// Convex hull data.
	bool 	convexHullComputed;
	Polygon hull;
	vector<int> vHullEdges;

	// Type of algorithm executed to compute the Delaunay algorithm.
	enum Algorithm algorithm;
#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	int nFlips;
	int nCollinear;
	int *nNodesChecked;
	int nNodesCheckedIndex;
#endif
    /*******************************************************************************************************************
    * Private methods declarations
    *******************************************************************************************************************/
    void initGraph();
	void checkEdge(int edge_ID);
	void flipEdges(int edge_ID);
	bool addPointToDelaunay(int index);
	bool locateNode(const Point<TYPE> &point, int &nodeIndex);
	bool isInteriorToNode(const Point<TYPE> &point, int nodeIndex);
	bool isStrictlyInteriorToNode(Point<TYPE> &point, int nodeIndex);
	void splitNode(int pointIndex, int nodeIndex, int nTriangles);
	double signedArea(Node *node);

	friend class DelaunayIO;
public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
	Delaunay() : convexHullComputed(false), hull(DEFAUTL_CONVEXHULL_LEN), algorithm(NONE)  {}
    explicit Delaunay(vector<Point<TYPE>> &vPoints);
	~Delaunay() = default;

    Delaunay(const Delaunay &d)
    {
        this->dcel = d.dcel;
        this->convexHullComputed = d.convexHullComputed;
        this->hull = d.hull;
        this->vHullEdges = d.vHullEdges;
        this->algorithm = d.algorithm;
        this->graph = d.graph;
    }

    /**
     * @fn      reset
     * @build   Resets triangulation state
     *
     */
	void reset();

	// Get/Set functions.
	bool isConvexHullComputed() {return(this->convexHullComputed);};
	void setConvexHullComputed(bool v) {this->convexHullComputed = v;};
	Graph* getGraph() {return &graph;}

	//bool internalToConvexHull(Point<TYPE> &p);
	Polygon* getConvexHull() {return(&this->hull);};
	vector<int> *getConvexHullEdges() {return &this->vHullEdges; };

    /*******************************************************************************************************************
    * Getter/Setters
    *******************************************************************************************************************/
	Dcel *getRefDcel() { return &this->dcel; };
	void setAlgorithm(enum Algorithm type) {this->algorithm = type;};
	enum Algorithm getAlgorithm() {return(this->algorithm);};

    /*******************************************************************************************************************
    * Triangulation interface functions implementation
    *******************************************************************************************************************/
    bool build();
    bool convexHull();
    bool findTwoClosest(int &first, int &second);
    bool findFace(Point<TYPE> &point, int &faceId, bool &isImaginary);
    bool findClosestPoint(const Point<TYPE> &p, Voronoi &voronoi, Point<TYPE> &q, int	&poinIndex, double &dist);
    bool findPath(Line &line, vector<int> &vFacesId);

#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
	void freeStatistics();
	int* getNodesChecked() const {return nNodesChecked;}
#endif
};

#endif /* INCLUDE_DELAUNAY_H_ */
