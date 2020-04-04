#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include "Dcel.h"
#include "Polygon.h"
#include "Stack.h"

struct ConvexPoint
{
	int		vertexIndex;
	int		edgeID;
};

/****************************************************************************
// 						TRIANGULATION CLASS DEFITNION
****************************************************************************/
class StarTriangulation
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    Dcel 	dcel;					// DCEL data

	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------

	Polygon *hull;					// Convex hull polygon.
	int		nPending;				// # pending edges.
	bool	convexHullComputed;		// Convex hull computed flag.
#ifdef STATISTICS_STAR_TRIANGULATION
	int 	nCollinear;				// # collinear points.
	int		nFlips;					// # flipped edges.
#endif

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
    bool setNotChecked(int index, bool *isEdgeChecked);

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    explicit StarTriangulation(const vector<Point<TYPE>> &vPoints);
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

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void reset();

	bool delaunay();

	//------------------------------------------------------------------------
	// Figures functions.
	//------------------------------------------------------------------------
	bool convexHull();
	bool findTwoClosest(int &first, int &second);
//	bool findFace(Point<TYPE> &point, int &faceId);
	bool findClosestPoint(Point<TYPE> &p, Point<TYPE> &q, double &distance);

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	bool isConvexHullComputed() const {return convexHullComputed;}
	Dcel* getDcel() {return &dcel;}
	Polygon* getConvexHull() const {return hull;}
#ifdef STATISTICS_STAR_TRIANGULATION
	int getCollinear() const {return nCollinear;}
	int getFlips() const {return nFlips;}
#endif
};

#endif /* TRIANGULATION_H_ */
