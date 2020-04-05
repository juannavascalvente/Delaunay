#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"
#include "Polygon.h"
#include "Stack.h"

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
	Polygon *hull;					// Convex hull polygon.
	int		nPending;				// # pending edges.
	bool	convexHullComputed;		// Convex hull computed flag.

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
    explicit StarTriangulation(vector<Point<TYPE>> &vPoints);
	~StarTriangulation();

    StarTriangulation(const StarTriangulation &t);

    /**
     * @fn          build
     * @brief       Computes star triangulation for a given set of points
     *
     * @return      true if star triangulation computed
     *              false otherwise
     */
    bool build();
	void reset();
	bool delaunay();

	bool convexHull();
	bool findTwoClosest(int &first, int &second);
	bool findClosestPoint(Point<TYPE> &p, Point<TYPE> &q, double &distance);

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
	bool isConvexHullComputed() const {return convexHullComputed;}
	Dcel* getDcel() {return &dcel;}
	void getConvexHull(Polygon &polygon) { polygon = *hull; }
    size_t getConvexHullLen() { return hull->getNElements(); }
#ifdef STATISTICS_STAR_TRIANGULATION
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
#endif
};

#endif /* TRIANGULATION_H_ */
