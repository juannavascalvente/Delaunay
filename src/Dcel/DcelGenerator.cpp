/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelGenerator.h"

#include <random>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelGenerator::generateRandom(size_t szNumPoints, vector<Point<TYPE>> &vPoints)
{
    // Number of points must be higher than 0
    if (szNumPoints == 0)
    {
        std::cout << __func__ << ".Number of points must be higher than 0 and it is " <<  szNumPoints << std::endl;
        return false;
    }

    // Generate points
    srand48((int) time(nullptr));

    // Generate set of points
    Point<TYPE> p;
    for (std::size_t  i=0; i<szNumPoints ;i++)
    {
        p.random();
        vPoints.push_back(p);
    }

    return true;
}


bool DcelGenerator::generateClusters(size_t szNumPoints, size_t szNumClusters, TYPE radius, vector<Point<TYPE>> &vPoints)
{
    // Number of points must be higher than 0
    if (szNumPoints == 0)
    {
        std::cout << __func__ << ".Number of points must be higher than 0 and it is " <<  szNumPoints << std::endl;
        return false;
    }

    // Number of points must be higher than 0
    if (szNumClusters == 0)
    {
        std::cout << __func__ << ".Number of clusters must be higher than 0 and it is " <<  szNumPoints << std::endl;
        return false;
    }

    // Number of points must be higher than 0
    if (radius <= 0.0)
    {
        std::cout << __func__ << ".Radius clusters must be higher than 0 and it is " <<  radius << std::endl;
        return false;
    }

    // Initialize output
    vPoints.clear();

    srand48((int) time(nullptr));

    // Get # elements per cluster and remaining number of points to add to last cluster
    size_t szNumElemsCluster = szNumPoints / szNumClusters;

    // Generate clusters centers.
    Point<TYPE> p;
    for (size_t i=0; i<szNumClusters ;i++)
    {
        // Generate new point.
        p.random();

        // Insert new point
        vPoints.push_back(p);

        // Generate cluster points based on p point and shifting it a maximum distance of the radius
        for (size_t j = 0; j < szNumElemsCluster - 1; j++)
        {
            // Create point using p as base point and shake it a maximum distance of radius
            Point<TYPE> q = p;
            q.shake(radius);

            // Insert new point
            vPoints.push_back(q);
        }
    }

    // Add remaining elements to last cluster
    size_t szNumRemaining = szNumPoints % szNumClusters;
    for (size_t i=0; i<szNumRemaining ; i++)
    {
        // Create point using p as base point and shake it a maximum distance of radius
        Point<TYPE> q = p;
        q.shake(radius);

        // Insert new point
        vPoints.push_back(q);
    }

    return true;
}
