/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "ConvexHull.h"


/***********************************************************************************************************************
* Public functions definitions
***********************************************************************************************************************/
bool ConvexHull::getConvexHull(Polygon &polygon)
{
    // Check hull has been computed
    if (isEmpty())
    {
        return false;
    }

    polygon = hull;
    return true;
};


bool ConvexHull::getConvexHullEdges(vector<int> &vOut)
{
    // Check hull has been computed
    if (isEmpty())
    {
        return false;
    }

    vOut = vEdges;
    return true;
}


void ConvexHull::reset()
{
    hull.reset();
    vEdges.clear();
}

void ConvexHull::add(Point<TYPE> &p, int edgeId)
{
    // Adds point
    hull.add(p);

    // Adds edge if it is a valid edge.
    if (edgeId != INVALID)
    {
        vEdges.push_back(edgeId);
    }
}
