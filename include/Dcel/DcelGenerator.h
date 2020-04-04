//
// Created by delaunay on 22/3/20.
//

#ifndef DELAUNAY_DCELGENERATOR_H
#define DELAUNAY_DCELGENERATOR_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <string>
#include "Dcel.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelGenerator
{

    static void clutter(Dcel &dcel);

public:
    /***************************************************************************
    * Name: 	generateRandom
    * IN:		nPoints			# points in the new set.
    * OUT:		NONE
    * RETURN:	true			if genarated.
    * 			false 			otherwise
    * GLOBAL:	NONE
    * Description: 	resizes the DCEL, empties the faces and edges array and
    * 				creates a random set of points.
    ***************************************************************************/
    static bool generateRandom(int nPoints, Dcel &dcel);

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

    /***************************************************************************
    * Name: 	generateClusters
    * IN:		nPoints			# points in the new set.
    * 			nClusters		# clusters to generate
    * 			radius			# radius of the clusters.
    * OUT:		NONE
    * RETURN:	true 			if generated.
    * 			false			i.o.c.
    * GLOBAL:	NONE
    * Description: 	creates a new set of points with "nClusters" clusters where
    * 				the points of every cluster are not further than "radius"
    * 				distance to a given initial point of the cluster.
    ***************************************************************************/
    static bool generateClusters(int nPoints, int nClusters, TYPE radius, Dcel &dcel);

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
