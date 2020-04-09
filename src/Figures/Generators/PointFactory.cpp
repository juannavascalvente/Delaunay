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


void PointFactory::readFromConfig(Point<TYPE> &point)
{
    // Get point from configuration.
    point = Config::getClosestPoint();

    // Check if input point parameter provided by user.
    if (point.getX() == INVALID)
    {
        // Get min and max coordiantes.
        int minX, minY, maxX, maxY;
        Config::getScreenCoordinates(minX, minY, maxX, maxY);

        // Generate seed.
        point.random();

        // Create a random point.
        point.setX(((int) point.getX()) % (int) maxX);
        point.setY(((int) point.getY()) % (int) maxY);
    }
}
