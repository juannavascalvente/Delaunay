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
#include "Voronoi.h"


/***********************************************************************************************************************
* Public declarations
***********************************************************************************************************************/
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
    ConvexHull hull;            // Convex hull data.

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
	Delaunay() : algorithm(NONE)  {}
    explicit Delaunay(vector<Point<TYPE>> &vPoints);
	~Delaunay() = default;

    Delaunay(const Delaunay &d)
    {
        if(this != &d)
        {
            this->dcel = d.dcel;
            this->hull = d.hull;
            this->hull = d.hull;
            this->algorithm = d.algorithm;
            this->graph = d.graph;
        }
    }

    /**
     * @fn      reset
     * @build   Resets triangulation state
     *
     */
	void reset();

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
	void setAlgorithm(enum Algorithm type) {this->algorithm = type;};
	enum Algorithm getAlgorithm() {return(this->algorithm);};

    /*******************************************************************************************************************
    * Triangulation interface functions implementation
    *******************************************************************************************************************/
    bool build();
    bool convexHull();
    bool findClosestPoint(Point<TYPE> &in, Point<TYPE> &out, Voronoi *voronoi);
    bool findTwoClosest(Point<TYPE> &p, Point<TYPE> &q);
    bool findFace(Point<TYPE> &origin, int &faceId);
    bool findPath(Point<TYPE> &origin, Point<TYPE> &dest, vector<int> &vFacesId);

#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
	void freeStatistics();
	int* getNodesChecked() const {return nNodesChecked;}
#endif
};

#endif /* INCLUDE_DELAUNAY_H_ */
