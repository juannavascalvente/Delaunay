/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "pathLib.h"

#include "DcelFigureBuilder.h"
#include "Delaunay.h"


/***********************************************************************************************************************
* API function declarations
***********************************************************************************************************************/
bool getTriangulationPath(Point<TYPE> &o, Point<TYPE> &d, vector<Point<TYPE>> &vPointsIn, vector<FaceT> &vFaces)
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
                vFaces.clear();

                // Iterate faces
                for (auto faceId : vFacesId)
                {
                    // Get current face points
                    vector<Point<TYPE>> vFacesPoints;
                    DcelFigureBuilder::getFacePoints(faceId, *delaunay->getRefDcel(), vFacesPoints);

                    // Add points to face
                    FaceT face = {};
                    for (auto point : vFacesPoints)
                    {
                        face.push_back(point);
                    }

                    // Add face to output
                    vFaces.push_back(face);
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

bool getVoronoiPath(Point<TYPE> &o, Point<TYPE> &d, vector<Point<TYPE>> &vPointsIn, vector<FaceT> &vFaces)
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
                    vector<int> vPathFaces;
                    vPathFaces.push_back(pointFace1 + 1);
                    vPathFaces.push_back(pointFace2 + 1);

                    // Find path
                    Line line(o, d);
                    vector<int> vFacesId;
                    if (voronoi->getRefDcel()->findPath(vPathFaces, line, vFacesId))
                    {
                        // Initialize output
                        isSuccess = true;
                        vFaces.clear();

                        // Iterate path faces
                        for (auto faceId : vFacesId)
                        {
                            // Get current face points
                            vector<Point<TYPE>> vFacesPoints;
                            DcelFigureBuilder::getFacePoints(faceId, *voronoi->getRefDcel(), vFacesPoints);

                            // Add points to face
                            FaceT face = {};
                            for (auto point : vFacesPoints)
                            {
                                face.push_back(point);
                            }

                            // Add face to output
                            vFaces.push_back(face);
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
