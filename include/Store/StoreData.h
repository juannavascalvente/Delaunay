//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_STOREDATA_H
#define DELAUNAY_STOREDATA_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"
#include "Delaunay.h"
#include "Gabriel.h"
#include "StarTriangulation.h"
#include "Status.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StoreData
{
    static Status 			    status;			// Current status.
    static Dcel			        dcel;			// Dcel data.
    static Delaunay		        delaunay;		// Delaunay data.
    static StarTriangulation	triangulation;	// Star triangulation data.
    static Voronoi			    voronoi;		// Voronoi diagram data.
    static Gabriel			    gabriel;		// Gabriel graph data.
public:

    static Status *getStatus() { return &status; };
    static Dcel *getDcel() { return &dcel; };
    static Delaunay *getDelaunay() { return &delaunay; };
    static StarTriangulation *getStarTriang() { return &triangulation; };
    static Voronoi *getVoronoi() { return &voronoi; };
    static Gabriel *getGabriel() { return &gabriel; };
};


#endif //DELAUNAY_STOREDATA_H
