/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "figuresLib.h"
#include "Delaunay.h"
#include "Gabriel.h"
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
        if (delaunay->build())
        {
            // Create Voronoi
            auto *voronoi = new Voronoi(delaunay->getRefDcel());

            // Compute Voronoi diagram.
            if (voronoi->build())
            {
                // Copy Voronoi Dcel
                dcelOut = *voronoi->getRefDcel();

                isSuccess = true;
            }

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


bool getGabriel(vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut)
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
            // Create Voronoi
            auto *voronoi = new Voronoi(delaunay->getRefDcel());

            // Compute Voronoi diagram.
            if (voronoi->build())
            {
                // Compute gabriel graph
                auto gabriel = new Gabriel(*delaunay->getRefDcel(), *voronoi);
                if (gabriel->build())
                {
                    isSuccess = true;

                    // Get reference to gabriel dcel.
                    Dcel *dcelRef = gabriel->getDcel();

                    // Draw Gabriel edges.
                    Point<TYPE> *p;	    // Temporary point
                    for (size_t edgeIndex=0; edgeIndex<gabriel->getSize() ;edgeIndex++)
                    {
                        // Check if current edge mamtches Gabriel restriction.s
                        if (gabriel->isSet(edgeIndex))
                        {
                            // Get origin vertex of edge.
                            p = dcelRef->getRefPoint(dcelRef->getOrigin(edgeIndex)-1);
                            vPointsOut.push_back((*p));

                            // Get destination vertex of edge.
                            p = dcelRef->getRefPoint(dcelRef->getOrigin(dcelRef->getTwin(edgeIndex)-1)-1);
                            vPointsOut.push_back((*p));
                        }
                    }
                }

                // Free resources
                delete gabriel;
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
