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
    bool isSuccess=false;       // Return value

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

            // TODO copy dcel
            dcelOut = *voronoi->getRefDcel();
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


bool getConvexHull(vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut)
{
    bool isSuccess=false;       // Return value

    try
    {
        // Initialize output
        vPointsOut.clear();

        // Create Delaunay
        auto *delaunay = new Delaunay(vPointsIn);

        // Build Delaunay using incremental algorithm
        if (delaunay->build())
        {
            // Compute convex hull
            if (delaunay->convexHull())
            {
                // Update output
                Polygon hull;
                if (delaunay->getConvexHull(hull))
                {
                    hull.getPoints(vPointsOut);
                    isSuccess = true;
                }
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


bool getGabriel(vector<Point<TYPE>> &vPointsIn)
{
    bool isSuccess=false;       // Return value
//    // Build Gabriel graph.
//    Delaunay *delaunay = in.getStoreService()->getDelaunay();
//    Voronoi *voronoi = in.getStoreService()->getVoronoi();
//    auto *gabriel = new Gabriel(*delaunay->getRefDcel(), *voronoi);
//    bool isRunSuccess = gabriel->build();
//
//    if (isRunSuccess)
//    {
//        Point<TYPE> *vertex1;	    // First vertex.
//        Point<TYPE> *vertex2;	    // Second vertex.
//        Dcel	*dcelRef;
//
//        // Get reference to gabriel dcel.
//        dcelRef = gabriel->getDcel();
//
//        // Draw Gabriel edges.
//        // TODO https://github.com/juannavascalvente/Delaunay/issues/60 -> Add dashed lines to highlight results
//        for (size_t edgeIndex=0; edgeIndex<gabriel->getSize() ;edgeIndex++)
//        {
//            // Check if current edge mamtches Gabriel restriction.s
//            if (gabriel->isSet(edgeIndex))
//            {
//                // Get origin vertex of edge.
//                vertex1 = dcelRef->getRefPoint(dcelRef->getOrigin(edgeIndex)-1);
//
//                // Get destination vertex of edge.
//                vertex2 = dcelRef->getRefPoint(dcelRef->getOrigin(dcelRef->getTwin(edgeIndex)-1)-1);
//
//                Line line(*vertex1, *vertex2);
//                vLines.push_back(line);
//            }
//        }
//
//        // Save result
//        in.getStoreService()->save(*gabriel);
//    }
    return isSuccess;
}
