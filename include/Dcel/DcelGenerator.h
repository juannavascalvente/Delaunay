//
// Created by delaunay on 22/3/20.
//

#ifndef DELAUNAY_DCELGENERATOR_H
#define DELAUNAY_DCELGENERATOR_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "Point.h"

#include <string>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelGenerator
{
public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    /**
     * @fn      generateRandom
     * @brief   Generates a random set of points
     *
     * @param   szNumPoints     (IN)    Number of points to generate
     * @param   vPoints         (OUT)   Generated set of points
     * @return  true if success
     *          false otherwise
     */
    static bool generateRandom(size_t szNumPoints, vector<Point<TYPE>> &vPoints);

    /**
     * @fn      generateClusters
     * @brief   creates a new set of points with "nClusters" clusters where the points of every cluster are not further
     *          than "radius" distance to a given initial point of the cluster
     *
     * @param   szNumPoints     (IN)    Number of points to generate
     * @param   szNumClusters   (IN)    Number of clusters to generate
     * @param   radius          (IN)    Radius of the clusters.
     * @param   vPoints         (OUT)   Generated set of points
     * @return  true if success
     *          false otherwise
     */
    static bool generateClusters(size_t szNumPoints, size_t szNumClusters, TYPE radius, vector<Point<TYPE>> &vPoints);
};

#endif //DELAUNAY_DCELGENERATOR_H
