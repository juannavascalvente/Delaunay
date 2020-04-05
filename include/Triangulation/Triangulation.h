#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"
#include "Polygon.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Triangulation
{
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Polygon hull;

public:

    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    /**
     * @fn      build
     * @build   Builds Delaunay triangulation using incremental algorithm
     *
     * @return  true if success
     *          false otherwise
     */
    virtual bool build() = 0;

    virtual bool createVoronoi(Voronoi &voronoi) = 0;

    /**
     * @fn      convexHull
     * @build   Builds the triangulation convex hull
     *
     * @return  true if success
     *          false otherwise
     */
    virtual bool convexHull(Polygon &hull) = 0;
    virtual bool findClosestPoint(Point<TYPE> &p, size_t, vector<Point<TYPE>> &v) = 0;
    virtual bool findTwoClosest(Point<TYPE> &p, Point<TYPE> &q) = 0;
    virtual bool path(Point<TYPE> &origin, Point<TYPE> &dest, vector<Polygon> &path) = 0;
    virtual bool findFace(Point<TYPE> &origin, Polygon &face) = 0;
};


#endif //DELAUNAY_TRIANGULATION_H
