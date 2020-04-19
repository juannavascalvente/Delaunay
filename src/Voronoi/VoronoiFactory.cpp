/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "VoronoiFactory.h"
#include "Logging.h"
#include "TriangulationFactory.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Voronoi *VoronoiFactory::create(vector<Point<TYPE>> &vPoints, bool &isSuccess)
{
    Voronoi *voronoi=nullptr;           // Return value

    try
    {
        // Create Delaunay
        auto *delaunay = TriangulationFactory::createDelaunay(vPoints, isSuccess);
        if (isSuccess)
        {
            // Create Voronoi
            voronoi = new Voronoi(delaunay->getRefDcel());

            // Compute Voronoi diagram.
            isSuccess = voronoi->build();
            if (!isSuccess)
            {
                delete voronoi;
                voronoi = nullptr;
            }
        }

        delete delaunay;
    }
    catch (bad_alloc &ex)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory");
        Logging::write( true, Error);
        isSuccess = false;
    }

    // Free resources
    return voronoi;
}
