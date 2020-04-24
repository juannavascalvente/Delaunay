/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "pathLib.h"

#include "DcelFigureBuilder.h"
#include "Delaunay.h"


/***********************************************************************************************************************
* API function declarations
***********************************************************************************************************************/
bool getTriangulationPath(Point<TYPE> &o, Point<TYPE> &d, vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut)
{
    bool isSuccess=false;       // Return value

    try
    {
        // Build Delaunay
        auto *delaunay = new Delaunay(vPointsIn);
        if (delaunay->build())
        {
            // Find path
            vector<int> vFacesId;
            if (delaunay->findPath(o, d, vFacesId))
            {
                // Initialize output
                isSuccess = true;
                vPointsOut.clear();

                // Iterate faces
                for (auto face : vFacesId)
                {
                    // Get current face points
                    vector<Point<TYPE>> vFacesPoints;
                    DcelFigureBuilder::getFacePoints(face, *delaunay->getRefDcel(), vFacesPoints);

                    // Add points to output
                    for (auto point : vFacesPoints)
                    {
                        vPointsOut.push_back(point);
                    }
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

bool getVoronoiPath(Point<TYPE> &o, Point<TYPE> &d, vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut)
{
    bool isSuccess=false;       // Return value

    try
    {
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
                int pointFace1;
                int pointFace2;
                Point<TYPE> closest;

                // Get faces where origin and destination points fall into
                if (delaunay->findClosestPoint(o, voronoi, closest, pointFace1) &&
                    delaunay->findClosestPoint(d, voronoi, closest, pointFace2))
                {
                    // Add faces to set whose path must be computed
                    vector<int> vFaces;
                    vFaces.push_back(pointFace1 + 1);
                    vFaces.push_back(pointFace2 + 1);

                    // Find path
                    Line line(o, d);
                    vector<int> vFacesId;
                    if (voronoi->getRefDcel()->findPath(vFaces, line, vFacesId))
                    {
                        // Initialize output
                        isSuccess = true;
                        vPointsOut.clear();

                        // Iterate path faces
                        for (auto face : vFacesId)
                        {
                            // Get current face points
                            vector<Point<TYPE>> vFacesPoints;
                            DcelFigureBuilder::getFacePoints(face, *voronoi->getRefDcel(), vFacesPoints);

                            // Add face points to output
                            for (auto point : vFacesPoints)
                            {
                                vPointsOut.push_back(point);
                            }
                        }
                    }
                }
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
