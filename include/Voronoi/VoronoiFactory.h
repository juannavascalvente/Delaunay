#ifndef DELAUNAY_VORONOIFACTORY_H
#define DELAUNAY_VORONOIFACTORY_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"
#include "Voronoi.h"

#include <vector>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class VoronoiFactory
{
public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
    /**
     * @fn      create
     * @brief   Creates a Voronoi diagram from a set of points using an incremental Delaunay triangulation
     *
     * @param vPoints       (IN)    Triangulation set of points
     * @param isSuccess     (OUT)   Flag that stands for execution success/failure
     * @return              Voronoi object if created successfully
     *                      null otherwise
     */
    static Voronoi *create(vector<Point<TYPE>> &vPoints, bool &isSuccess);
};


#endif //DELAUNAY_VORONOIFACTORY_H
