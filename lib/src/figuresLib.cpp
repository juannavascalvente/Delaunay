/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "figuresLib.h"
#include "Delaunay.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* API function definitions
***********************************************************************************************************************/
bool getVoronoi(vector<Point<TYPE>> &vPoints, Dcel &dcelOut)
{
    bool isSuccess;       // Return value

    try
    {
        // Create Delaunay
        auto *delaunay = new Delaunay(vPoints);

        // Build Delaunay using incremental algorithm
        isSuccess = delaunay->build();

        // Check init was success
        if (isSuccess)
        {
            // Create Voronoi
            auto *voronoi = new Voronoi(delaunay->getRefDcel());

            // Compute Voronoi diagram.
            isSuccess = voronoi->build();

            dcelOut = *voronoi->getRefDcel();
            delete voronoi;
        }

        // Free resources
        delete delaunay;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        isSuccess = false;
    }

    return isSuccess;
}


bool getConvexHull(vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut)
{
    bool isSuccess;       // Return value

    try
    {
        // Create Delaunay
        auto *delaunay = new Delaunay(vPointsIn);

        // Build Delaunay using incremental algorithm
        isSuccess = delaunay->build();

        // Check init was success
        if (isSuccess)
        {
            // Compute convex hull
            isSuccess = delaunay->convexHull();

            // Update output
            Polygon hull;
            delaunay->getConvexHull(hull);
            vPointsOut.clear();
            hull.getPoints(vPointsOut);
        }

        // Free resources
        delete delaunay;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        isSuccess = false;
    }

    return isSuccess;
}
