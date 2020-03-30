//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDPARAMIN_H
#define DELAUNAY_COMMANDPARAMIN_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "StoreService.h"

#include <cstddef>


class CmdParamIn
{
    StoreService *storeService;
public:
    explicit CmdParamIn(StoreService *storeServiceIn) : storeService(storeServiceIn) {};

    StoreService *getStoreService() const { return storeService; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GeneratorCmdParamIn : public CmdParamIn
{
    size_t szNumPoints;

public:
    GeneratorCmdParamIn(size_t szNumPointsIn, StoreService *storeService) : szNumPoints(szNumPointsIn),
                                                                            CmdParamIn(storeService) {};

    /**
     * Getter/Setter
     */
    size_t getNumPoints() const { return szNumPoints;}
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GeneratorClusterCmdParamIn : public GeneratorCmdParamIn
{
    TYPE fRadius;
    size_t szNumClusters;
public:
    GeneratorClusterCmdParamIn(size_t szNumPointsIn, size_t szNumClustersIn, TYPE fRadiusIn, StoreService *storeService) :
                                                                        GeneratorCmdParamIn(szNumPointsIn, storeService),
                                                                        szNumClusters(szNumClustersIn),
                                                                        fRadius(fRadiusIn)
                                                                        {};
    /**
     * Getter/Setter
     */
    TYPE getFRadius() const { return fRadius; }
    size_t getSzNumClusters() const { return szNumClusters; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StarTriangulationCmdIn : public CmdParamIn
{
    Dcel *dcel;
public:
    explicit StarTriangulationCmdIn(Dcel *dcelIn, StoreService *storeService) : dcel(dcelIn), CmdParamIn(storeService) {};

    /**
     * Getter/Setter
     */
    Dcel &getDcel() { return(*dcel); }
};

#endif //DELAUNAY_COMMANDPARAMIN_H
