//
// Created by delaunay on 29/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Command.h"
#include "CommandFactory.h"
#include "MenuOption.h"


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
Command *CommandFactory::create(size_t szOptionCmdId, Dcel *dcel)
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
            command = createRandomGenerator(Config::getNPoints(), dcel);
            break;
        }
        // Generate random set of points grouped in clusters
        case CLUSTER:
        {
            command = createClusterGenerator(Config::getNPoints(), Config::getNClusters(), Config::getRadius(), dcel);
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


Command *CommandFactory::createRandomGenerator(size_t szNumPoints, Dcel *dcel)
{
    // Create parameters
    GeneratorCmdParamIn in(szNumPoints);
    GeneratorCmdParamOut out(dcel);

    // Create command
    return new CommandGenerateRandom(&in, &out);
}

Command *CommandFactory::createClusterGenerator(size_t szNumPoints, size_t szNumClusters, TYPE radius, Dcel *dcel)
{
    // Create parameters
    GeneratorClusterCmdParamIn in(szNumPoints, szNumClusters, radius);
    GeneratorCmdParamOut out(dcel);

    // Create command
    return new CommandGenerateCluster(&in, &out);
}
