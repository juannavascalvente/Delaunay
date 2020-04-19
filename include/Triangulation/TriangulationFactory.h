#ifndef DELAUNAY_TRIANGULATIONFACTORY_H
#define DELAUNAY_TRIANGULATIONFACTORY_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Delaunay.h"
#include "Point.h"
#include "StarTriangulation.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class TriangulationFactory
{
public:
    /**
     * @fn      createStar
     * @brief   Creates star triangulation using a set of points as input
     *
     * @param   vPoints     (IN)    Triangulation set of points
     * @param   isSuccess   (OUT)   Flag that stands for successful execution
     * @return  Star triangulation if successfully created
     *          nullptr otherwise
     */
    static StarTriangulation *createStar(vector<Point<TYPE>> &vPoints, bool &isSuccess);

    /**
     * @fn      createDelaunay
     * @brief   Creates Delaunay triangulation using a set of points as input
     *
     * @param   vPoints     (IN)    Triangulation set of points
     * @param   isSuccess   (OUT)   Flag that stands for successful execution
     * @return  Delaunay triangulation if successfully created
     *          nullptr otherwise
     */
    static Delaunay *createDelaunay(vector<Point<TYPE>> &vPoints, bool &isSuccess);
};


#endif //DELAUNAY_TRIANGULATIONFACTORY_H
