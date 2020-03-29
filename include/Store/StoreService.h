//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_STORESERVICE_H
#define DELAUNAY_STORESERVICE_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"
#include "Delaunay.h"
#include "Gabriel.h"
#include "StarTriangulation.h"
#include "StoreData.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StoreService
{
public:
    static Status *getStatus()  { return StoreData::getStatus(); };
    static Dcel *getDcel()      { return StoreData::getDcel(); };
    static Delaunay *getDelaunay() { return StoreData::getDelaunay(); };
    static StarTriangulation *getStarTriang() { return StoreData::getStarTriang(); };
    static Voronoi *getVoronoi() { return StoreData::getVoronoi(); };
    static Gabriel *getGabriel() { return StoreData::getGabriel(); };
};


#endif //DELAUNAY_STORESERVICE_H
