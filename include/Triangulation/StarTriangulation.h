#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "ConvexHull.h"
#include "Dcel.h"
#include "Polygon.h"
#include "Triangulation.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StarTriangulation : public Triangulation
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	int		nPending;				// # pending edges.
    bool    isBuilt;
#ifdef STATISTICS_STAR_TRIANGULATION
	int 	nCollinear;				// # collinear points.
	int		nFlips;					// # flipped edges.
#endif

    /*******************************************************************************************************************
    * Private methods
    *******************************************************************************************************************/
    bool setNotChecked(int index, bool *isEdgeChecked);
    void getConvexFacesIntersections(Line &line, vector<int> &vFaces, vector<int> &vEdges);

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    StarTriangulation() : nPending(0), isBuilt(false) {};
    explicit StarTriangulation(vector<Point<TYPE>> &vPoints) : Triangulation(vPoints), nPending(0), isBuilt(false) {};
	~StarTriangulation() = default;
    StarTriangulation(const StarTriangulation &t) : Triangulation(t)
    {
        if (this != &t)
        {
            nPending = t.nPending;
            isBuilt = t.isBuilt;
        }
    }

    /**
     * @fn          build
     * @brief       Computes star triangulation for a given set of points
     *
     * @return      true if star triangulation computed
     *              false otherwise
     */
    bool build() override ;

	bool delaunay();

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
    Dcel* getRefDcel() {return &dcel;}
    bool isValid() const { return isBuilt; };

    /*******************************************************************************************************************
    * Convex hull functions
    *******************************************************************************************************************/
    bool isConvexHullComputed() { return !hull.isEmpty(); };
    bool getConvexHull(Polygon &polygon) { return hull.getConvexHull(polygon); };
    size_t getConvexHullLen() { return hull.size(); }

    /*******************************************************************************************************************
    * Triangulation interface functions implementation
    *******************************************************************************************************************/
    bool convexHull() override ;
    bool findClosestPoint(Point<TYPE> &in, Voronoi *voronoi, Point<TYPE> &out, int &pointIndex) override ;
    bool findTwoClosest(Point<TYPE> &p, Point<TYPE> &q) override ;
    bool findFace(Point<TYPE> &point, int &faceId) override ;
    bool findPath(Point<TYPE> &origin, Point<TYPE> &dest, vector<int> &vFacesId) override ;
};

#endif /* TRIANGULATION_H_ */
