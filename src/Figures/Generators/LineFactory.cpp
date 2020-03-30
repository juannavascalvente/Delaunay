//
// Created by delaunay on 30/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "LineFactory.h"
#include "PointFactory.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void LineFactory::generateRandom(size_t szNum, vector<Line> &vLines)
{
    // Create n lines.
    for (size_t i=0; i<szNum ;i++)
    {
        // Create random points
        vector<Point<TYPE>> vPoints;
        PointFactory::generateRandom(2, vPoints);

        // Create line
        Line line(vPoints.at(0), vPoints.at(1));
        vLines.push_back(line);
    }
}


void LineFactory::readFromConfig(vector<Line> &vLines)
{
    // Create line
    Line line(Config::getOriginPoint(), Config::getDestinationPoint());
    vLines.push_back(line);
}
