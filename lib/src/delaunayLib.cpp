/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "delaunayLib.h"
#include "StarTriangulation.h"


/***********************************************************************************************************************
* API function definitions
***********************************************************************************************************************/
bool getStarTriangulation(const vector<Point<TYPE>> &vPoints, Dcel &dcelOut)
{
    bool isSuccess;       // Return value

    try
    {
        // Run command
        auto *triangulation = new StarTriangulation(vPoints);
        isSuccess = triangulation->build();

        // Update output
        dcelOut = *triangulation->getDcel();

        // Free resources
        delete triangulation;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        isSuccess = false;
    }

    return isSuccess;
}


bool getDelaunay(const vector<Point<TYPE>> &vPoints, Dcel &dcelOut)
{
    bool isSuccess;       // Return value

    try
    {
        // Insert points into dcel
        auto *delaunay = new Delaunay(vPoints);

        // Build Delaunay from DCEL.
        isSuccess = delaunay->incremental();

        // Update output
        dcelOut = *delaunay->getRefDcel();

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


bool getVoronoi(const vector<TYPE> &vPoints, Voronoi &voronoi)
{
    // TODO -> pending to implement
    return false;
}


bool getConvexHull(const Dcel &dcel, vector<TYPE> &vPoints)
{
    // TODO -> pending to implement
    return false;
}
