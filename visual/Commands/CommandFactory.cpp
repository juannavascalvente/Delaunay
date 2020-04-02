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
* Static members
***********************************************************************************************************************/
map<int, pfuncCommandCreate> CommandFactory::mapFactories;


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
Command *CommandFactory::create(size_t szOptionCmdId, StoreService *storeService, ConfigService *configServiceIn)
{
    if (CommandFactory::mapFactories.empty())
    {
        initialize();
    }

    Command *command;

    // Check if option is valid
    if (CommandFactory::mapFactories.find(szOptionCmdId) == CommandFactory::mapFactories.end())
    {
        command = CommandFactory::createNull(storeService, configServiceIn);
        cout << "Error creating command. Id is not valid " << szOptionCmdId << endl;
    }
    else
    {
        command = CommandFactory::mapFactories.at(szOptionCmdId)(storeService, configServiceIn);
    }

    return command;
}


/***********************************************************************************************************************
* Private method definitions
***********************************************************************************************************************/
Command *CommandFactory::createNull(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandNull(storeService, configService);
}


Command *CommandFactory::createReadCfg(StoreService *storeService, ConfigService *configService)
{
    // Create command
    return new CommandReadConfig(storeService, configService);
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
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_points_dcel_file, CommandFactory::createReadPointsDcel));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_dcel, CommandFactory::createReadDcel));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_delaunay, CommandFactory::createReadDelaunay));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_voronoi, CommandFactory::createNull));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(read_gabriel, CommandFactory::createNull));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_points, CommandFactory::createWritePoints));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_dcel, CommandFactory::createWriteDcel));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_delaunay, CommandFactory::createWriteDelaunay));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_voronoi, CommandFactory::createWriteVoronoi));
    mapFactories.insert(std::pair<int, pfuncCommandCreate>(write_gabriel, CommandFactory::createWriteGabriel));
}
