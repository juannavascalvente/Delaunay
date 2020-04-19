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
    // Clear output
    vPoints.clear();

    // Generate random seed for set points
    srand48(int(time(nullptr)));

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
        // Get min and max coordinates
        int minX, minY, maxX, maxY;
        Config::getScreenCoordinates(minX, minY, maxX, maxY);

        // Generate random point
        // Set seed
        srand48(int(time(nullptr)));
        point.random();

        // Fit to screen
        if (point.getX() > (double) maxX)
        {
            point.setX(point.getX() - (double) maxX);
        }
        if (point.getY() > (double) maxY)
        {
            point.setY(point.getY() - (double) maxY);
        }
    }
}
