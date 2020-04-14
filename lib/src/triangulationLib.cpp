/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Delaunay.h"
#include "StarTriangulation.h"
#include "triangulationLib.h"


/***********************************************************************************************************************
* API function definitions
***********************************************************************************************************************/
bool getStarTriangulation(vector<Point<TYPE>> &vPoints, Dcel &dcelOut)
{
    bool isSuccess;       // Return value

    try
    {
        // Run command
        auto *triangulation = new StarTriangulation(vPoints);
        isSuccess = triangulation->build();

        // Update output
        dcelOut = *triangulation->getRefDcel();

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


bool getDelaunay(vector<Point<TYPE>> &vPoints, Dcel &dcelOut)
{
    bool isSuccess;       // Return value

    try
    {
        // Insert points into delaunay
        auto *delaunay = new Delaunay(vPoints);

        // Build Delaunay using incremental algorithm
        isSuccess = delaunay->build();

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
