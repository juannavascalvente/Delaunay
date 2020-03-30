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
    static void generateRandom(size_t szNum, vector<Point<TYPE>> &vPoints);
};


#endif //DELAUNAY_POINTFACTORY_H
