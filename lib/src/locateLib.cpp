/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "locateLib.h"

#include "DcelFigureBuilder.h"
#include "Delaunay.h"


/***********************************************************************************************************************
* API function definitions
***********************************************************************************************************************/
bool getClosestPoint(Point<TYPE> &p, vector<Point<TYPE>> &vPoints, Point<TYPE> &closest)
{
    bool isSuccess=false;         // Return value

    try
    {
        // Insert points into delaunay
        auto *delaunay = new Delaunay(vPoints);

        // Build Delaunay using incremental algorithm
        if (delaunay->build())
        {
            auto *voronoi = new Voronoi(delaunay->getRefDcel());

            // Compute Voronoi diagram.
            if ( voronoi->build())
            {
                int	pointIndex=0;
                isSuccess = delaunay->findClosestPoint(p, voronoi, closest, pointIndex);
            }

            // Free resources
            delete voronoi;
        }

        // Free resources
        delete delaunay;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
    }
    catch (exception &ex)
    {
        ex.what();
    }

    return isSuccess;
}

bool get2ClosestPoints(vector<Point<TYPE>> &vPoints, Point<TYPE> &p, Point<TYPE> &q)
{
    bool isSuccess=false;         // Return value

    try
    {
        // Insert points into delaunay
        auto *delaunay = new Delaunay(vPoints);

        // Build Delaunay using incremental algorithm
        if (delaunay->build())
        {
            isSuccess = delaunay->findTwoClosest(p, q);
        }

        // Free resources
        delete delaunay;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
    }
    catch (exception &ex)
    {
        ex.what();
    }

    return isSuccess;
}

bool getPointFace(Point<TYPE> &p, vector<Point<TYPE>> &vPoints, vector<Point<TYPE>> &facePoints)
{
    bool isSuccess=false;         // Return value

    try
    {
        // Initialize putput
        facePoints.clear();

        // Insert points into delaunay
        auto *delaunay = new Delaunay(vPoints);

        // Build Delaunay using incremental algorithm
        if (delaunay->build())
        {
            int faceId=0;
            if (delaunay->findFace(p, faceId))
            {
                // Get face points
                if (faceId != EXTERNAL_FACE)
                {
                    DcelFigureBuilder::getFacePoints(faceId, *delaunay->getRefDcel(), facePoints);
                }
                isSuccess = true;
            }
        }

        // Free resources
        delete delaunay;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
    }
    catch (exception &ex)
    {
        ex.what();
    }

    return isSuccess;
}
