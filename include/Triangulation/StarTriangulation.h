#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "ConvexHull.h"
#include "Dcel.h"
#include "Polygon.h"

struct ConvexPoint
{
	int		vertexIndex;
	int		edgeID;
};

/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StarTriangulation
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    Dcel 	dcel;					// DCEL data
	int		nPending;				// # pending edges.
	ConvexHull hull;

#ifdef STATISTICS_STAR_TRIANGULATION
	int 	nCollinear;				// # collinear points.
	int		nFlips;					// # flipped edges.
#endif

    /*******************************************************************************************************************
    * Private methods
    *******************************************************************************************************************/
    bool setNotChecked(int index, bool *isEdgeChecked);

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    explicit StarTriangulation(vector<Point<TYPE>> &vPoints) : dcel(vPoints), nPending(0) {};
	~StarTriangulation() = default;
    StarTriangulation(const StarTriangulation &t)
    {
        nPending = t.nPending;
        this->dcel = t.dcel;
        hull = t.hull;
    }

    /**
     * @fn          build
     * @brief       Computes star triangulation for a given set of points
     *
     * @return      true if star triangulation computed
     *              false otherwise
     */
    bool build();

    /**
     * @fn      reset
     * @brief   Resets data
     */
	void reset() { this->hull.reset(); };
	bool delaunay();

	bool convexHull();
	bool findTwoClosest(int &first, int &second);
	bool findClosestPoint(Point<TYPE> &p, Point<TYPE> &q, double &distance);

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
    Dcel* getDcel() {return &dcel;}

    /*******************************************************************************************************************
    * Convex hull functions
    *******************************************************************************************************************/
    bool isConvexHullComputed() { return !hull.isEmpty(); };
    bool getConvexHull(Polygon &polygon) { return hull.getConvexHull(polygon); };
//    bool getConvexHullEdges(vector<int> &vEdges) { return hull.getConvexHullEdges(vEdges);  };
    size_t getConvexHullLen() { return hull.size(); }
#ifdef STATISTICS_STAR_TRIANGULATION
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
#endif
};

#endif /* TRIANGULATION_H_ */
