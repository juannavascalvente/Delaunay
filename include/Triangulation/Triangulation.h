#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"
#include "Polygon.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Triangulation
{
protected:
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    Dcel dcel;
    ConvexHull hull;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Triangulation() = default;
    explicit Triangulation(vector<Point<TYPE>> &vPoints) : dcel(vPoints) {};
    ~Triangulation() = default;
    Triangulation(const Triangulation &t)
    {
        if (this != &t)
        {
            dcel = t.dcel;
            hull = t.hull;
        }
    }

    /**
     * @fn      build
     * @build   Builds Delaunay triangulation using incremental algorithm
     *
     * @return  true if success
     *          false otherwise
     */
    virtual bool build() = 0;

    /**
     * @fn      reset
     * @brief   Resets data
     */
    void reset()
    {
        dcel.reset();
        hull.reset();
    }

    /**
     * @fn      convexHull
     * @build   Builds the triangulation convex hull
     *
     * @return  true if success
     *          false otherwise
     */
    virtual bool convexHull() = 0;

    /**
     * @fn      findClosestPoint
     * @build   Find closest point to a given point
     *
     * @param in            (IN) Point whose closes point is returned
     * @param voronoi       (IN) Voronoi diagram
     * @param out           (OUT) Closest point
     * @param pointIndex    (OUT) Dcel point index
     * @return  true if point located
     *          false otherwise
     */
    virtual bool findClosestPoint(Point<TYPE> &in, Voronoi *voronoi, Point<TYPE> &out, int &pointIndex) = 0;

    /**
     * @fn      findTwoClosest
     * @build   Find two closest point in a point set
     *
     * @param   p   (OUT)   First of the two closest points
     * @param   q   (OUT)   Second of the two closest points
     * @return  true if found
     *          false otherwise
     */
    virtual bool findTwoClosest(Point<TYPE> &p, Point<TYPE> &q) = 0;

    /**
     * @fn      findFace
     * @build   Find face where input point falls into
     *
     * @param   origin      (IN) Point whose face is returned
     * @param   faceId      (OUT) Face where input point falls into
     * @return  true if found
     *          false otherwise
     */
    virtual bool findFace(Point<TYPE> &origin, int &faceId) = 0;

    /**
     * @fn      findPath
     * @build   Find the set of faces between two points
     *
     * @param   origin    (IN) Origin point of the path
     * @param   dest      (IN) Desination point of the path
     * @param   vFacesId  (OUT) Set of faces in the path
     * @return  true if path found
     *          false otherwise
     */
    virtual bool findPath(Point<TYPE> &origin, Point<TYPE> &dest, vector<int> &vFacesId) = 0;
};


#endif //DELAUNAY_TRIANGULATION_H
