//
// Created by delaunay on 29/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <Store/StoreService.h>
#include "Command.h"
#include "CommandFactory.h"
#include "MenuOption.h"


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
Command *CommandFactory::create(size_t szOptionCmdId, StoreService *storeService)
{
    // Return value
    Command *command;

    // Check command option
    switch (szOptionCmdId)
    {
        case PARAMETERS:
        {
            command = createReadCfg();
            break;
        }
        // Generate random set of points
        case RANDOMLY:
        {
            command = createRandomGenerator(Config::getNPoints(), storeService);
            break;
        }
        // Generate random set of points grouped in clusters
        case CLUSTER:
        {
            command = createClusterGenerator(Config::getNPoints(), Config::getNClusters(), Config::getRadius(), storeService);
            break;
        }
        // Create start triangulation from set of points
        case STAR_TRIANGULATION:
        {
            command = createStarTriangulation(storeService);
            break;
        }
        // Create Delaunay triangulation
        case DELAUNAY:
        {
            command = createDelaunay(storeService);
            break;
        }
        case CONVEX_HULL:
        {
            command = createConvexHull(storeService);
            break;
        }
        case VORONOI:
        {
            command = createVoronoi(storeService);
            break;
        }
        case GABRIEL:
        {
            command = createGabriel(storeService);
            break;
        }
        case TRIANGULATION_PATH:
        {
            command = createTriangulationPath(storeService);
            break;
        }
        default:
        {
            command = CommandFactory::createNull();
            cout << "Error creating command. Id is not valid " << szOptionCmdId << endl;
            break;
        }
    }

    return command;
}


/***********************************************************************************************************************
* Private method definitions
***********************************************************************************************************************/
Command *CommandFactory::createNull()
{
    // Create command
    return new CommandNull();
}


Command *CommandFactory::createReadCfg()
{
    // Create command
    return new CommandReadConfig();
}


Command *CommandFactory::createRandomGenerator(size_t szNumPoints, StoreService *storeService)
{
    // Create parameters
    GeneratorCmdParamIn in(szNumPoints, storeService);
    GeneratorCmdParamOut out(storeService->getDcel());

    // Create command
    return new CommandGenerateRandom(in, out);
}


Command *CommandFactory::createClusterGenerator(size_t szNumPoints, size_t szNumClusters, TYPE radius, StoreService *storeService)
{
    // Create parameters
    GeneratorClusterCmdParamIn in(szNumPoints, szNumClusters, radius, storeService);
    GeneratorCmdParamOut out(storeService->getDcel());

    // Create command
    return new CommandGenerateCluster(in, out);
}


Command *CommandFactory::createStarTriangulation(StoreService *storeService)
{
    // Create parameters
    StarTriangulationParamCmdIn in(storeService->getDcel(), storeService);
    GeneratorCmdParamOut out(storeService->getDcel());

    // Create command
    return new CommandStarTriangulation(in, out);
}


Command *CommandFactory::createDelaunay(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);
    CmdParamOut out(storeService);

    // Create command
    return new CommandDelaunay(in, out);
}


Command *CommandFactory::createConvexHull(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);
    CmdParamOut out(storeService);

    // Create command
    return new CommandConvexHull(in, out);
}


Command *CommandFactory::createVoronoi(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);
    CmdParamOut out(storeService);

    // Create command
    return new CommandVoronoi(in, out);
}


Command *CommandFactory::createGabriel(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);
    CmdParamOut out(storeService);

    // Create command
    return new CommandGabriel(in, out);
}

Command *CommandFactory::createTriangulationPath(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);
    CmdParamOut out(storeService);

    // Create command
    return new CommandTriangulationPath(in, out);
}