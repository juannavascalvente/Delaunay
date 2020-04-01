//
// Created by delaunay on 29/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreService.h"
#include "Command.h"
#include "CommandFactory.h"
#include "MenuOption.h"


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
Command *CommandFactory::create(size_t szOptionCmdId, StoreService *storeService, ConfigService *configServiceIn)
{
    // Return value
    Command *command;

    // Check command option
    switch (szOptionCmdId)
    {
        case DEFAULT_OPTION:
        {
            command = createNull();
            break;
        }
        case PARAMETERS:
        {
            command = createReadCfg();
            break;
        }
        // Generate random set of points
        case RANDOMLY:
        {
            command = createRandomGenerator(storeService, configServiceIn);
            break;
        }
        // Generate random set of points grouped in clusters
        case CLUSTER:
        {
            command = createClusterGenerator(storeService, configServiceIn);
            break;
        }
        // Create start triangulation from set of points
        case STAR_TRIANGULATION:
        {
            command = createStarTriangulation(storeService, configServiceIn);
            break;
        }
        // Create Delaunay triangulation
        case DELAUNAY:
        {
            command = createDelaunay(storeService, configServiceIn);
            break;
        }
        case CONVEX_HULL:
        {
            command = createConvexHull(storeService, configServiceIn);
            break;
        }
        case VORONOI:
        {
            command = createVoronoi(storeService, configServiceIn);
            break;
        }
        case GABRIEL:
        {
            command = createGabriel(storeService, configServiceIn);
            break;
        }
        case TRIANGULATION_PATH:
        {
            command = createTriangulationPath(storeService, configServiceIn);
            break;
        }
        case VORONOI_PATH:
        {
            command = createVoronoiPath(storeService, configServiceIn);
            break;
        }
        case CLOSEST_POINT:
        {
            command = createClosestPoint(storeService, configServiceIn);
            break;
        }
        case FIND_FACE:
        {
            command = createFindFace(storeService, configServiceIn);
            break;
        }
        case TWO_CLOSEST:
        {
            command = createTwoClosest(storeService, configServiceIn);
            break;
        }
        case FILTER_EDGES:
        {
            command = createFilterEdges(storeService, configServiceIn);
            break;
        }
        case CIRCUMCENTRES:
        {
            command = createCircumcentres(storeService, configServiceIn);
            break;
        }
        case EDGE_CIRCLES:
        {
            command = createEdgeCircle(storeService, configServiceIn);
            break;
        }
        case DCEL_INFO:
        {
            command = createDcelInfo(storeService, configServiceIn);
            break;
        }
        case VORONOI_INFO:
        {
            command = createVoronoiInfo(storeService, configServiceIn);
            break;
        }
        case CLEAR:
        {
            command = createClear(storeService, configServiceIn);
            break;
        }
        case READ_POINTS_FLAT_FILE:
        {
            command = createReadPoints(storeService, configServiceIn);
            break;
        }
        case READ_POINTS_DCEL_FILE:
        {
            command = createReadPointsDcel(storeService, configServiceIn);
            break;
        }
        case READ_DCEL:
        {
            command = createReadDcel(storeService, configServiceIn);
            break;
        }
        case READ_DELAUNAY:
        {
            command = createReadDelaunay(storeService, configServiceIn);
            break;
        }
        case READ_VORONOI:
        {
            command = CommandFactory::createNull();
            cout << "Error creating command. Read Voronoi is not implemented " << endl;
            //command = createReadVoronoi(storeService, configServiceIn);
            break;
        }
        case READ_GABRIEL:
        {
            command = CommandFactory::createNull();
            cout << "Error creating command. Read Gabriel is not implemented " << endl;
            //command = createReadGabriel(storeService, configServiceIn);
            break;
        }
        case WRITE_POINTS:
        {
            command = createWritePoints(storeService, configServiceIn);
            break;
        }
        case WRITE_DCEL:
        {
            command = createWriteDcel(storeService, configServiceIn);
            break;
        }
        case WRITE_DELAUNAY:
        {
            command = createWriteDelaunay(storeService, configServiceIn);
            break;
        }
        case WRITE_VORONOI:
        {
            command = createWriteVoronoi(storeService, configServiceIn);
            break;
        }
        case WRITE_GABRIEL:
        {
            command = createWriteGabriel(storeService, configServiceIn);
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


Command *CommandFactory::createRandomGenerator(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandGenerateRandom(storeService, configService);
}


Command *CommandFactory::createClusterGenerator(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandGenerateCluster(storeService, configService);
}


Command *CommandFactory::createStarTriangulation(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandStarTriangulation(storeService, configService);
}


Command *CommandFactory::createDelaunay(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandDelaunay(storeService, configService);
}


Command *CommandFactory::createConvexHull(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandConvexHull(storeService, configService);
}


Command *CommandFactory::createVoronoi(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandVoronoi(storeService, configService);
}


Command *CommandFactory::createGabriel(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandGabriel(storeService, configService);
}

Command *CommandFactory::createTriangulationPath(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandTriangulationPath(storeService, configService);
}

Command *CommandFactory::createVoronoiPath(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandVoronoiPath(storeService, configService);
}

Command *CommandFactory::createClosestPoint(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandClosestPoint(storeService, configService);
}

Command *CommandFactory::createFindFace(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandFindFace(storeService, configService);
}

Command *CommandFactory::createTwoClosest(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandTwoClosest(storeService, configService);
}

Command *CommandFactory::createFilterEdges(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandFilterEdges(storeService, configService);
}

Command *CommandFactory::createCircumcentres(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandCircumcentres(storeService, configService);
}

Command *CommandFactory::createEdgeCircle(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandEdgeCircle(storeService, configService);
}

Command *CommandFactory::createDcelInfo(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandDcelInfo(storeService, configService);
}

Command *CommandFactory::createVoronoiInfo(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandVoronoiInfo(storeService, configService);
}

Command *CommandFactory::createClear(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandClear(storeService, configService);
}

Command *CommandFactory::createReadPoints(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandReadPoints(storeService, configService);
}

Command *CommandFactory::createReadPointsDcel(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandReadPointsDcel(storeService, configService);
}

Command *CommandFactory::createReadDcel(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandReadDcel(storeService, configService);
}

Command *CommandFactory::createReadDelaunay(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandReadDelaunay(storeService, configService);
}

//Command *CommandFactory::createReadVoronoi(StoreService *storeService, ConfigService *configService)
//{
//    // Create command
//    return new CommandReadVoronoi(storeService, configService);
//}
//
//Command *CommandFactory::createReadGabriel(StoreService *storeService, ConfigService *configService)
//{
//    // Create command
//    return new CommandClear(storeService, configService);
//}

Command *CommandFactory::createWritePoints(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandWriteFile(storeService, configService);
}

Command *CommandFactory::createWriteDcel(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandWriteFileDcel(storeService, configService);
}

Command *CommandFactory::createWriteDelaunay(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandWriteFileDelaunay(storeService, configService);
}

Command *CommandFactory::createWriteVoronoi(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandWriteFileVoronoi(storeService, configService);
}

Command *CommandFactory::createWriteGabriel(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandWriteFileGabriel(storeService, configService);
}
