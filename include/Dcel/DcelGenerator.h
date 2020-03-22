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
};

#endif //DELAUNAY_DCELGENERATOR_H
