#ifndef DELAUNAY_STORESERVICE_H
#define DELAUNAY_STORESERVICE_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
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
    * Config
    *******************************************************************************************************************/
    size_t getNumPoints() { return Config::getNPoints(); };
    size_t getNumClusters() { return Config::getNClusters(); };
    size_t getRadius() { return Config::getRadius(); };
    size_t getMinLengthEdge() { return Config::getMinLengthEdge(); };


    /*******************************************************************************************************************
    * Delete
    *******************************************************************************************************************/
    void deleteTriangulation() { repository->getData()->deleteStar(); };
    void deleteDelaunay() { repository->getData()->deleteDelaunay(); };
    void deleteVoronoi() { repository->getData()->deleteVoronoi(); };
    void deleteGabriel() { repository->getData()->deleteGabriel(); };

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
    Delaunay    *getDelaunay()  { return repository->getData()->getDelaunayData(); };
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
