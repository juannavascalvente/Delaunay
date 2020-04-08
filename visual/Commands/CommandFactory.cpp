/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreService.h"
#include "Command.h"
#include "CommandFactory.h"
#include "MenuOption.h"


/***********************************************************************************************************************
* Static members
***********************************************************************************************************************/
map<int, pfuncCommandCreate> CommandFactory::mapFactories;


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
Command *CommandFactory::create(size_t szOptionCmdId, StoreService *storeService)
{
    if (CommandFactory::mapFactories.empty())
    {
        initialize();
    }

    Command *command;

    // Check if option is valid
    if (CommandFactory::mapFactories.find(szOptionCmdId) == CommandFactory::mapFactories.end())
    {
        command = CommandFactory::createFail(storeService);
    }
    else
    {
        command = CommandFactory::mapFactories.at(szOptionCmdId)(storeService);
    }

    return command;
}


/***********************************************************************************************************************
* Private method definitions
***********************************************************************************************************************/
Command *CommandFactory::createNull(StoreService *storeService)
{
    // Create command
    return new CommandNull(storeService);
}

Command *CommandFactory::createFail(StoreService *storeService)
{
    // Create command
    return new CommandFail(storeService);
}


Command *CommandFactory::createReadCfg(StoreService *storeService)
{
    // Create command
    return new CommandReadConfig(storeService);
}


Command *CommandFactory::createRandomGenerator(StoreService *storeService)
{
    // Create command
    return new CommandGenerateRandom(storeService);
}


Command *CommandFactory::createClusterGenerator(StoreService *storeService)
{
    // Create command
    return new CommandGenerateCluster(storeService);
}


Command *CommandFactory::createStarTriangulation(StoreService *storeService)
{
    // Create command
    return new CommandStarTriangulation(storeService);
}


Command *CommandFactory::createDelaunay(StoreService *storeService)
{
    // Create command
    return new CommandDelaunay(storeService);
}


Command *CommandFactory::createConvexHull(StoreService *storeService)
{
    // Create command
    return new CommandConvexHull(storeService);
}


Command *CommandFactory::createVoronoi(StoreService *storeService)
{
    // Create command
    return new CommandVoronoi(storeService);
}


Command *CommandFactory::createGabriel(StoreService *storeService)
{
    // Create command
    return new CommandGabriel(storeService);
}

Command *CommandFactory::createTriangulationPath(StoreService *storeService)
{
    // Create command
    return new CommandTriangulationPath(storeService);
}

Command *CommandFactory::createVoronoiPath(StoreService *storeService)
{
    // Create command
    return new CommandVoronoiPath(storeService);
}

Command *CommandFactory::createClosestPoint(StoreService *storeService)
{
    // Create command
    return new CommandClosestPoint(storeService);
}

Command *CommandFactory::createFindFace(StoreService *storeService)
{
    // Create command
    return new CommandFindFace(storeService);
}

Command *CommandFactory::createTwoClosest(StoreService *storeService)
{
    // Create command
    return new CommandTwoClosest(storeService);
}

Command *CommandFactory::createFilterEdges(StoreService *storeService)
{
    // Create command
    return new CommandFilterEdges(storeService);
}

Command *CommandFactory::createCircumcentres(StoreService *storeService)
{
    // Create command
    return new CommandCircumcentres(storeService);
}

Command *CommandFactory::createEdgeCircle(StoreService *storeService)
{
    // Create command
    return new CommandEdgeCircle(storeService);
}

Command *CommandFactory::createDcelInfo(StoreService *storeService)
{
    // Create command
    return new CommandDcelInfo(storeService);
}

Command *CommandFactory::createVoronoiInfo(StoreService *storeService)
{
    // Create command
    return new CommandVoronoiInfo(storeService);
}

Command *CommandFactory::createClear(StoreService *storeService)
{
    // Create command
    return new CommandClear(storeService);
}

Command *CommandFactory::createReadPoints(StoreService *storeService)
{
    // Create command
    return new CommandReadPoints(storeService);
}


Command *CommandFactory::createReadDelaunay(StoreService *storeService)
{
    // Create command
    return new CommandReadDelaunay(storeService);
}

//Command *CommandFactory::createReadVoronoi(StoreService *storeService)
//{
//    // Create command
//    return new CommandReadVoronoi(storeService);
//}
//
//Command *CommandFactory::createReadGabriel(StoreService *storeService)
//{
//    // Create command
//    return new CommandClear(storeService);
//}

Command *CommandFactory::createWritePoints(StoreService *storeService)
{
    // Create command
    return new CommandWriteFile(storeService);
}

Command *CommandFactory::createWriteDcel(StoreService *storeService)
{
    // Create command
    return new CommandWriteFileDcel(storeService);
}

Command *CommandFactory::createWriteDelaunay(StoreService *storeService)
{
    // Create command
    return new CommandWriteFileDelaunay(storeService);
}

Command *CommandFactory::createWriteVoronoi(StoreService *storeService)
{
    // Create command
    return new CommandWriteFileVoronoi(storeService);
}

Command *CommandFactory::createWriteGabriel(StoreService *storeService)
{
    // Create command
    return new CommandWriteFileGabriel(storeService);
}

void CommandFactory::initialize()
{
    // Initialize map
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(defaultOption, CommandFactory::createNull));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(parameters, CommandFactory::createReadCfg));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(randomly, CommandFactory::createRandomGenerator));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(cluster, CommandFactory::createClusterGenerator));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(star_triangulation, CommandFactory::createStarTriangulation));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(delaunay_triangulation, CommandFactory::createDelaunay));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(convex_hull, CommandFactory::createConvexHull));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(voronoi, CommandFactory::createVoronoi));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(gabriel, CommandFactory::createGabriel));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(triangulation_path, CommandFactory::createTriangulationPath));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(voronoi_path, CommandFactory::createVoronoiPath));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(closest_point, CommandFactory::createClosestPoint));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(find_face, CommandFactory::createFindFace));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(two_closest, CommandFactory::createTwoClosest));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(filter_edges, CommandFactory::createFilterEdges));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(circumcentres, CommandFactory::createCircumcentres));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(edge_circles, CommandFactory::createEdgeCircle));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(dcel_info, CommandFactory::createDcelInfo));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(voronoi_info, CommandFactory::createVoronoiInfo));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(clear, CommandFactory::createClear));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_points_flat_file, CommandFactory::createReadPoints));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_delaunay, CommandFactory::createReadDelaunay));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_voronoi, CommandFactory::createNull));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_gabriel, CommandFactory::createNull));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_points, CommandFactory::createWritePoints));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_dcel, CommandFactory::createWriteDcel));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_delaunay, CommandFactory::createWriteDelaunay));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_voronoi, CommandFactory::createWriteVoronoi));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_gabriel, CommandFactory::createWriteGabriel));
}
