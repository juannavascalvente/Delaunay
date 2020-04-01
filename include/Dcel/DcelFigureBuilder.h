//
// Created by delaunay on 28/3/20.
//

#ifndef DELAUNAY_DCELFIGUREBUILDER_H
#define DELAUNAY_DCELFIGUREBUILDER_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"
#include "Point.h"

#include <vector>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelFigureBuilder
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/


    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn      getEdgePoints
     * @brief   Get points that define a DCEL edge
     *
     * @param   szEdgeIdx   (IN)    Edge index whose points are returned
     * @param   dcel        (IN)    Dcel where edge is stored
     * @param   vPoints     (OUT)   Edge points vector
     */
    static void getEdgePoints(size_t szEdgeIdx, const Dcel &dcel, vector<Point<TYPE>> &vPoints);

    /**
     * @fn      getFacePoints
     * @brief   Get points that define a DCEL face
     *
     * @param   szFaceIdx   (IN)    Face index whose points are returned
     * @param   dcel        (IN)    Dcel where face is stored
     * @param   vPoints     (OUT)   Face points vector
     */
    static void getFacePoints(size_t szFaceIdx, const Dcel &dcel, vector<Point<TYPE>> &vPoints);
};


#endif //DELAUNAY_DCELFIGUREBUILDER_H
