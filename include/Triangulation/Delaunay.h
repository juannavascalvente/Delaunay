#ifndef INCLUDE_DELAUNAY_H_
#define INCLUDE_DELAUNAY_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "ConvexHull.h"
#include "Dcel.h"
#include "defines.h"
#include "Graph.h"
#include "Polygon.h"
#include "Triangulation.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Delaunay: public Triangulation
{
    /*******************************************************************************************************************
    * Private class members
    *******************************************************************************************************************/
	Graph 	graph;				// Graph used in incremental algorithm.
    bool    isBuilt;

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
	Delaunay() : isBuilt(false) {};
    explicit Delaunay(vector<Point<TYPE>> &vPoints) : Triangulation(vPoints), isBuilt(false)  {};
	~Delaunay() = default;
    Delaunay(const Delaunay &d) : Triangulation(d)
    {
        if(this != &d)
        {
            this->graph = d.graph;
            this->isBuilt = d.isBuilt;
        }
    }

    /*******************************************************************************************************************
    * Convex hull functions
    *******************************************************************************************************************/
	bool isConvexHullComputed() { return !hull.isEmpty(); };
	bool getConvexHull(Polygon &polygon) { return hull.getConvexHull(polygon); };
	bool getConvexHullEdges(vector<int> &vEdges) { return hull.getConvexHullEdges(vEdges);  };
	size_t getConvexHullLen() { return hull.size(); }

    /*******************************************************************************************************************
    * Getter/Setters
    *******************************************************************************************************************/
    Graph* getGraph() {return &graph;}
	Dcel *getRefDcel() { return &this->dcel; };
    bool isValid() const { return isBuilt; };

    /*******************************************************************************************************************
    * Triangulation interface functions implementation
    *******************************************************************************************************************/
    bool build() override ;
    bool convexHull() override ;
    bool findTwoClosest(Point<TYPE> &p, Point<TYPE> &q) override ;
    bool findFace(Point<TYPE> &origin, int &faceId) override ;

    bool findClosestPoint(Point<TYPE> &in, Voronoi *voronoi, Point<TYPE> &out, int &pointIndex) override ;
    bool findPath(Point<TYPE> &origin, Point<TYPE> &dest, vector<int> &vFacesId) override ;
#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
	void freeStatistics();
	int* getNodesChecked() const {return nNodesChecked;}
#endif
};

#endif /* INCLUDE_DELAUNAY_H_ */
