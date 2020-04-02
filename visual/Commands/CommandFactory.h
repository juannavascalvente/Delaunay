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

#include <map>

typedef Command * (*pfuncCommandCreate)(StoreService *storeService, ConfigService *configService);


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
class CommandFactory
{
    /***********************************************************************************************************************
    * Private memebers
    ***********************************************************************************************************************/
    static map<int, pfuncCommandCreate> mapFactories;

    /***********************************************************************************************************************
    * Private method declarations
    ***********************************************************************************************************************/
    static Command *createNull(StoreService *storeService, ConfigService *configService);
    static Command *createReadCfg(StoreService *storeService, ConfigService *configService);
    static Command *createRandomGenerator(StoreService *storeService, ConfigService *configService);
    static Command *createClusterGenerator(StoreService *storeService, ConfigService *configService);
    static Command *createStarTriangulation(StoreService *storeService, ConfigService *configService);
    static Command *createDelaunay(StoreService *storeService, ConfigService *configService);
    static Command *createConvexHull(StoreService *storeService, ConfigService *configService);
    static Command *createVoronoi(StoreService *storeService, ConfigService *configService);
    static Command *createGabriel(StoreService *storeService, ConfigService *configService);
    static Command *createTriangulationPath(StoreService *storeService, ConfigService *configService);
    static Command *createVoronoiPath(StoreService *storeService, ConfigService *configService);
    static Command *createClosestPoint(StoreService *storeService, ConfigService *configService);
    static Command *createFindFace(StoreService *storeService, ConfigService *configService);
    static Command *createTwoClosest(StoreService *storeService, ConfigService *configService);
    static Command *createFilterEdges(StoreService *storeService, ConfigService *configService);
    static Command *createCircumcentres(StoreService *storeService, ConfigService *configService);
    static Command *createEdgeCircle(StoreService *storeService, ConfigService *configService);
    static Command *createDcelInfo(StoreService *storeService, ConfigService *configService);
    static Command *createVoronoiInfo(StoreService *storeService, ConfigService *configService);
    static Command *createClear(StoreService *storeService, ConfigService *configService);
    static Command *createReadPoints(StoreService *storeService, ConfigService *configService);
    static Command *createReadPointsDcel(StoreService *storeService, ConfigService *configService);
    static Command *createReadDcel(StoreService *storeService, ConfigService *configService);
    static Command *createReadDelaunay(StoreService *storeService, ConfigService *configService);
//    static Command *createReadVoronoi(StoreService *storeService, ConfigService *configService);
//    static Command *createReadGabriel(StoreService *storeService, ConfigService *configService);
    static Command *createWritePoints(StoreService *storeService, ConfigService *configService);
    static Command *createWriteDcel(StoreService *storeService, ConfigService *configService);
    static Command *createWriteDelaunay(StoreService *storeService, ConfigService *configService);
    static Command *createWriteVoronoi(StoreService *storeService, ConfigService *configService);
    static Command *createWriteGabriel(StoreService *storeService, ConfigService *configService);

    static void initialize();

public:

    /***********************************************************************************************************************
    * Public method declarations
    ***********************************************************************************************************************/
    static Command *create(size_t szOptionCmdId, StoreService *storeService, ConfigService *configServiceIn);
};


#endif //DELAUNAY_COMMANDFACTORY_H
