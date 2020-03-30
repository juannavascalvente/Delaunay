//
// Created by delaunay on 30/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "PointFactory.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void PointFactory::generateRandom(size_t szNum, vector<Point<TYPE>> &vPoints)
{
    // Create n lines.
    for (size_t i=0; i<szNum ;i++)
    {
        // Create a random point
        Point<TYPE> p;
        p.random();

        // Add point to vector
        vPoints.push_back(p);
    }
}
