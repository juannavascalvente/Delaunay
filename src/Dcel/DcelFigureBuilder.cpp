//
// Created by delaunay on 28/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelFigureBuilder.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void DcelFigureBuilder::getEdgePoints(size_t szEdgeIdx, Dcel &dcel, vector<Point<TYPE>> &vPoints)
{
    // Check out of bounds access
    if (szEdgeIdx >= dcel.getNumEdges())
    {
        throw std::runtime_error(&"Out of bounds access in getEdgePoints: " [ szEdgeIdx]);
    }

    // Get origin and destination points of the edge.
    vPoints.push_back(*dcel.getRefPoint(dcel.getOrigin(szEdgeIdx) - 1));
    vPoints.push_back(*dcel.getRefPoint(dcel.getOrigin(dcel.getTwin(szEdgeIdx) - 1) - 1));
}


void DcelFigureBuilder::getFacePoints(size_t szFaceIdx, Dcel &dcel, vector<Point<TYPE>> &vPoints)
{
    // Get first and current face edges
    size_t szFirstEdgeIdx = dcel.getFaceEdge(szFaceIdx) - 1;        // First face edge
    size_t szEdgeIdx = szFirstEdgeIdx;                              // Current edge

    // Face edges iteration
    do
    {
        // Check edge is not in imaginary face (incremental Delauay algorithm)
        if (!dcel.hasNegativeVertex(szEdgeIdx + 1))
        {
            // Get edge origin and destination points.
            vector<Point<TYPE>> vEdgePoints;
            DcelFigureBuilder::getEdgePoints(szEdgeIdx, dcel, vEdgePoints);

            // Add element but do not add the same vertex twice (end edge vertex is origin in next edge).
            for (auto point : vEdgePoints)
            {
                if (vPoints.empty() || (point != vPoints.back()))
                {
                    vPoints.push_back(point);
                }
            }
        }

        // Get next edge.
        szEdgeIdx = dcel.getNext(szEdgeIdx) - 1;
    } while (szEdgeIdx != szFirstEdgeIdx);
}
