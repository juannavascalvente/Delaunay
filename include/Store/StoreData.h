#ifndef DELAUNAY_STOREDATA_H
#define DELAUNAY_STOREDATA_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Delaunay.h"
#include "Gabriel.h"
#include "StarTriangulation.h"
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
    Delaunay		    *delaunay;		// Delaunay data.
    StarTriangulation	*triangulation;	// Star triangulation data.
    Voronoi			    *voronoi;		// Voronoi diagram data.
    Gabriel			    *gabriel;		// Gabriel graph data.

public:

    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    StoreData() : delaunay(nullptr), triangulation(nullptr), voronoi(nullptr), gabriel(nullptr) {};


    /*******************************************************************************************************************
    * Getters
    *******************************************************************************************************************/
    void deleteStar();
    void deleteDelaunay();
    void deleteVoronoi();
    void deleteGabriel();

    /*******************************************************************************************************************
    * Getters
    *******************************************************************************************************************/
    vector<Point<TYPE>> *getPoints()   { return &vPoints; };
    Delaunay *getDelaunayData() { return delaunay; };
    StarTriangulation *getStarTriang() { return triangulation; };
    Voronoi *getVoronoi()   { return voronoi; };
    Gabriel *getGabriel()   { return gabriel; };

    /*******************************************************************************************************************
    * Savers
    *******************************************************************************************************************/
    void save(vector<Point<TYPE>> &v);
    void save(StarTriangulation &t);
    void save(Delaunay &d);
    void save(Voronoi &v);
    void save(Gabriel &g);
};


#endif //DELAUNAY_STOREDATA_H
