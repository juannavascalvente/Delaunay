/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "TriangulationFactory.h"
#include "Logging.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
StarTriangulation *TriangulationFactory::createStar(vector<Point<TYPE>> &vPoints, bool &isSuccess)
{
    StarTriangulation *triangulation=nullptr;
    try
    {
        triangulation = new StarTriangulation(vPoints);
        isSuccess = triangulation->build();
    }
    catch (bad_alloc &ex)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory");
        Logging::write( true, Error);
        isSuccess = false;
    }

    return triangulation;
}


Delaunay *TriangulationFactory::createDelaunay(vector<Point<TYPE>> &vPoints, bool &isSuccess)
{
    Delaunay *delaunay= nullptr;

    try
    {
        // Build Delaunay triangulation
        delaunay = new Delaunay(vPoints);
        isSuccess = delaunay->build();
    }
    catch (bad_alloc &ex)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory");
        Logging::write( true, Error);
        isSuccess = false;
    }

    return delaunay;
}
