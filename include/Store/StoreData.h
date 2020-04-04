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

    /*******************************************************************************************************************
    * Privae class members
    *******************************************************************************************************************/
    vector<Point<TYPE>> vPoints;        // Set of points
    Status 			    status;			// Current status.
    Dcel			    dcel;			// Dcel data.
    Delaunay		    delaunay;		// Delaunay data.
    StarTriangulation	triangulation;	// Star triangulation data.
    Voronoi			    voronoi;		// Voronoi diagram data.
    Gabriel			    gabriel;		// Gabriel graph data.

public:

    /*******************************************************************************************************************
    * Getters
    *******************************************************************************************************************/
    vector<Point<TYPE>> *getPoints()   { return &vPoints; };
    Status   *getStatus()   { return &status; };
    Dcel     *getDcel()     { return &dcel; };
    Delaunay *getDelaunay() { return &delaunay; };
    StarTriangulation *getStarTriang() { return &triangulation; };
    Voronoi *getVoronoi()   { return &voronoi; };
    Gabriel *getGabriel()   { return &gabriel; };

    /*******************************************************************************************************************
    * Setters
    *******************************************************************************************************************/
    void setPoints(vector<Point<TYPE>> &vPointsIn) { vPoints = vPointsIn; };
};


#endif //DELAUNAY_STOREDATA_H
