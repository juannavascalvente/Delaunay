/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "delaunayLib.h"
#include "StarTriangulation.h"


/***********************************************************************************************************************
* API function definitions
***********************************************************************************************************************/
bool starTriangulation(const vector<Point<TYPE>> &vPoints, Dcel &dcel)
{
    bool isSuccess;       // Return value

    try
    {
        // Run command
        auto *triangulation = new StarTriangulation(vPoints);
        isSuccess = triangulation->build();

        // Update output
        dcel = *triangulation->getDcel();

        // Delete resources
        delete triangulation;
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        isSuccess = false;
    }

    return isSuccess;
}


bool delaunay(const vector<TYPE> &vPoints, Delaunay &delaunay)
{
    bool isSuccess;       // Return value

    try
    {
        // Insert points into dcel
        Dcel *dcel = new Dcel();
        delaunay.setDCEL(dcel);

        // Build Delaunay from DCEL.
        isSuccess = delaunay.incremental();
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        isSuccess = false;
    }

    return isSuccess;
}


bool voronoi(const vector<TYPE> &vPoints, Voronoi &voronoi)
{
    // TODO -> pending to implement
    return false;
}


bool convexHull(const Dcel &dcel, vector<TYPE> &vPoints)
{
    // TODO -> pending to implement
    return false;
}
