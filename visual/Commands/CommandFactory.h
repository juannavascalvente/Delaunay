//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDFACTORY_H
#define DELAUNAY_COMMANDFACTORY_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Command.h"
#include "Dcel.h"


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
class CommandFactory
{
    /***********************************************************************************************************************
    * Private method declarations
    ***********************************************************************************************************************/
    static Command *createNull();
    static Command *createReadCfg();
    static Command *createRandomGenerator(size_t szNumPoints, StoreService *storeService);
    static Command *createClusterGenerator(size_t szNumPoints, size_t szNumClusters, TYPE radius, StoreService *storeService);
    static Command *createStarTriangulation(StoreService *storeService);
    static Command *createDelaunay(StoreService *storeService);
    static Command *createConvexHull(StoreService *storeService);
    static Command *createVoronoi(StoreService *storeService);
    static Command *createGabriel(StoreService *storeService);
    static Command *createTriangulationPath(StoreService *storeService);
    static Command *createVoronoiPath(StoreService *storeService);
    static Command *createClosestPoint(StoreService *storeService);
    static Command *createFindFace(StoreService *storeService);
    static Command *createTwoClosest(StoreService *storeService);
    static Command *createFilterEdges(StoreService *storeService, TYPE minLen);
    static Command *createCircumcentres(StoreService *storeService);

public:

    /***********************************************************************************************************************
    * Public method declarations
    ***********************************************************************************************************************/
    static Command *create(size_t szOptionCmdId, StoreService *storeService);
};


#endif //DELAUNAY_COMMANDFACTORY_H
