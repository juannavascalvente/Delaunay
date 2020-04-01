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
Command *CommandFactory::create(size_t szOptionCmdId, StoreService *storeService)
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
        case VORONOI_PATH:
        {
            command = createVoronoiPath(storeService);
            break;
        }
        case CLOSEST_POINT:
        {
            command = createClosestPoint(storeService);
            break;
        }
        case FIND_FACE:
        {
            command = createFindFace(storeService);
            break;
        }
        case TWO_CLOSEST:
        {
            command = createTwoClosest(storeService);
            break;
        }
        case FILTER_EDGES:
        {
            command = createFilterEdges(storeService, Config::getMinLengthEdge());
            break;
        }
        case CIRCUMCENTRES:
        {
            command = createCircumcentres(storeService);
            break;
        }
        case EDGE_CIRCLES:
        {
            command = createEdgeCircle(storeService);
            break;
        }
        case DCEL_INFO:
        {
            command = createDcelInfo(storeService);
            break;
        }
        case VORONOI_INFO:
        {
            command = createVoronoiInfo(storeService);
            break;
        }
        case CLEAR:
        {
            command = createClear(storeService);
            break;
        }
        case READ_POINTS_FLAT_FILE:
        {
            command = createReadPoints(storeService);
            break;
        }
        case READ_POINTS_DCEL_FILE:
        {
            command = createReadPointsDcel(storeService);
            break;
        }
        case READ_DCEL:
        {
            command = createReadDcel(storeService);
            break;
        }
        case READ_DELAUNAY:
        {
            command = createReadDelaunay(storeService);
            break;
        }
        case READ_VORONOI:
        {
            command = CommandFactory::createNull();
            cout << "Error creating command. Read Voronoi is not implemented " << endl;
            //command = createReadVoronoi(storeService);
            break;
        }
        case READ_GABRIEL:
        {
            command = CommandFactory::createNull();
            cout << "Error creating command. Read Gabriel is not implemented " << endl;
            //command = createReadGabriel(storeService);
            break;
        }
        case WRITE_POINTS:
        {
            command = createWritePoints(storeService);
            break;
        }
        case WRITE_DCEL:
        {
            command = createWriteDcel(storeService);
            break;
        }
        case WRITE_DELAUNAY:
        {
            command = createWriteDelaunay(storeService);
            break;
        }
        case WRITE_VORONOI:
        {
            command = createWriteVoronoi(storeService);
            break;
        }
        case WRITE_GABRIEL:
        {
            command = createWriteGabriel(storeService);
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

    // Create command
    return new CommandGenerateRandom(in);
}


Command *CommandFactory::createClusterGenerator(size_t szNumPoints, size_t szNumClusters, TYPE radius, StoreService *storeService)
{
    // Create parameters
    GeneratorClusterCmdParamIn in(szNumPoints, szNumClusters, radius, storeService);

    // Create command
    return new CommandGenerateCluster(in);
}


Command *CommandFactory::createStarTriangulation(StoreService *storeService)
{
    // Create parameters
    StarTriangulationParamCmdIn in(storeService->getDcel(), storeService);

    // Create command
    return new CommandStarTriangulation(in);
}


Command *CommandFactory::createDelaunay(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandDelaunay(in);
}


Command *CommandFactory::createConvexHull(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandConvexHull(in);
}


Command *CommandFactory::createVoronoi(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandVoronoi(in);
}


Command *CommandFactory::createGabriel(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandGabriel(in);
}

Command *CommandFactory::createTriangulationPath(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandTriangulationPath(in);
}

Command *CommandFactory::createVoronoiPath(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandVoronoiPath(in);
}

Command *CommandFactory::createClosestPoint(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandClosestPoint(in);
}

Command *CommandFactory::createFindFace(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandFindFace(in);
}

Command *CommandFactory::createTwoClosest(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandTwoClosest(in);
}

Command *CommandFactory::createFilterEdges(StoreService *storeService, TYPE minLen)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandFilterEdges(in, minLen);
}

Command *CommandFactory::createCircumcentres(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandCircumcentres(in);
}

Command *CommandFactory::createEdgeCircle(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandEdgeCircle(in);
}

Command *CommandFactory::createDcelInfo(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandDcelInfo(in);
}

Command *CommandFactory::createVoronoiInfo(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandVoronoiInfo(in);
}

Command *CommandFactory::createClear(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandClear(in);
}

Command *CommandFactory::createReadPoints(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandReadPoints(in);
}

Command *CommandFactory::createReadPointsDcel(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandReadPointsDcel(in);
}

Command *CommandFactory::createReadDcel(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandReadDcel(in);
}

Command *CommandFactory::createReadDelaunay(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandReadDelaunay(in);
}

//Command *CommandFactory::createReadVoronoi(StoreService *storeService)
//{
//    // Create parameters
//    CmdParamIn  in(storeService);
//
//    // Create command
//    return new CommandReadVoronoi(in);
//}
//
//Command *CommandFactory::createReadGabriel(StoreService *storeService)
//{
//    // Create parameters
//    CmdParamIn  in(storeService);
//
//    // Create command
//    return new CommandClear(in);
//}

Command *CommandFactory::createWritePoints(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandWriteFile(in);
}

Command *CommandFactory::createWriteDcel(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandWriteFileDcel(in);
}

Command *CommandFactory::createWriteDelaunay(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandWriteFileDelaunay(in);
}

Command *CommandFactory::createWriteVoronoi(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandWriteFileVoronoi(in);
}

Command *CommandFactory::createWriteGabriel(StoreService *storeService)
{
    // Create parameters
    CmdParamIn  in(storeService);

    // Create command
    return new CommandWriteFileGabriel(in);
}
