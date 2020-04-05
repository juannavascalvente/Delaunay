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
public:

    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    bool build() = 0;
    bool createVoronoi(Voronoi &voronoi) = 0;
    bool createConvexHull(Polygon &hull) = 0;
    bool closestPointSet(Point<TYPE> &p, size_t, vector<Point<TYPE>> &v) = 0;
    bool twoClosest(Point<TYPE> &p, Point<TYPE> &q) = 0;
    bool path(Point<TYPE> &origin, Point<TYPE> &dest, vector<Polygon> &path) = 0;
    bool findFace(Point<TYPE> &origin, Polygon &face) = 0;
};


#endif //DELAUNAY_TRIANGULATION_H
