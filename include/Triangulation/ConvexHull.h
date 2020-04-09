#ifndef DELAUNAY_CONVEXHULL_H
#define DELAUNAY_CONVEXHULL_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Polygon.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class ConvexHull
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    Polygon     hull;
    vector<int> vEdges;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    ConvexHull() = default;
    ~ConvexHull() = default;
    ConvexHull(const ConvexHull &h)
    {
        if (this != &h)
        {
            this->hull = h.hull;
            this->vEdges = h.vEdges;
        }
    }

    /**
     * @fn      isEmpty
     * @brief   Return if convex hull exists
     *
     * @return
     */
    bool isEmpty() { return (hull.getNElements() == 0); }

     /**
     * @fn      getConvexHull
     * @brief   Return convex hull
     *
     * @param   polygon     (OUT) convex hull
     * @return  true if convex hull has exists
     *          false otherwise
     */
    bool getConvexHull(Polygon &polygon);

    /**
     * @fn      getConvexHullEdges
     * @brief   Return convex hull edges
     *
     * @param   vEdges      (OUT) Convex hull edges
     * @return  true if convex hull has exists
     *          false otherwise
     */
    bool getConvexHullEdges(vector<int> &vEdges);

    /**
     * @fn      reset
     * @brief   Reset data
     */
    void reset();

    /**
     * @fn      add
     * @brief   Adds point to convex hull
     *
     * @param p
     * @param edgeId
     */
    void add(Point<TYPE> &p, int edgeId=INVALID);

    /**
     * @fn      size
     * @brief   Get convex hull number of edges
     *
     * @return  Convex hull number of edges
     */
    size_t size() { return vEdges.size(); };
};

#endif //DELAUNAY_CONVEXHULL_H
