//
// Created by delaunay on 30/3/20.
//

#ifndef DELAUNAY_POINTFACTORY_H
#define DELAUNAY_POINTFACTORY_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"
#include <vector>


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
class PointFactory
{
public:
    /**
     * @fn      generateRandom
     * @brief   Generates a set of random points
     *
     * @param   szNum       (IN)    Number of points to generate
     * @param   vPoints     (OUT)   Random points set
     */
    static void generateRandom(size_t szNum, vector<Point<TYPE>> &vPoints);

    /**
     * @fn      readFromConfig
     * @brief   Reads popint from configuration file. If point is not set, then it generates a random point
     *
     * @param   point   (OUT)   Point read from configuration file (or random if no point set in configuration file)
     */
    static void readFromConfig(Point<TYPE> &point);
};


#endif //DELAUNAY_POINTFACTORY_H
