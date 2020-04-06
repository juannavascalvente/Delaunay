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
#include "StoreDataRepository.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StoreService
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    StoreDataRepository *repository;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    explicit StoreService(StoreDataRepository *repoIn) : repository(repoIn) {};

    /**
     * @fn      reset
     * @brief   Resets Delaunay and Voronoi
     */
    void reset();

    /**
     * @fn      destroy
     * @brief   Deletes repository
     */
    void destroy();

    /*******************************************************************************************************************
    * Checkers
    *******************************************************************************************************************/
    bool isSetCreated() { return !getPoints().empty(); };
    bool isTriangulation() { return getStarTriang() && getStarTriang()->isValid(); };
    bool isDelaunay() { return getDelaunay() && getDelaunay()->isValid(); };
    bool isVoronoi() { return getVoronoi() && getVoronoi()->isValid(); };
    bool isGabriel() { return getGabriel() && getGabriel()->isValid(); };

    /*******************************************************************************************************************
    * Getters
    *******************************************************************************************************************/
    vector<Point<TYPE>>      &getPoints()    { return *repository->getData()->getPoints(); };
    Delaunay    *getDelaunay()  { return repository->getData()->getDelaunay(); };
    Voronoi     *getVoronoi()   { return repository->getData()->getVoronoi(); };
    Gabriel     *getGabriel()   { return repository->getData()->getGabriel(); };
    StarTriangulation *getStarTriang() { return repository->getData()->getStarTriang(); };

    /*******************************************************************************************************************
    * Setters
    *******************************************************************************************************************/
    void    save(vector<Point<TYPE>> &vPoints) { repository->getData()->save(vPoints); };
    void    save(StarTriangulation &triangulation) { repository->getData()->save(triangulation); };
    void    save(Delaunay &delaunay) { repository->getData()->save(delaunay); };
    void    save(Voronoi &voronoi) { repository->getData()->save(voronoi); };
    void    save(Gabriel &gabriel) { repository->getData()->save(gabriel); };
};

#endif //DELAUNAY_STORESERVICE_H
