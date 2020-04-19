/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "TriangulationFactory.h"
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
        auto *triangulation = TriangulationFactory::createStar(vPoints, isSuccess);

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
        // Build Delaunay using incremental algorithm
        auto *delaunay = TriangulationFactory::createDelaunay(vPoints, isSuccess);

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
