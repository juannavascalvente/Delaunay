
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Process.h"
#include "Command.h"
#include "CommandFactory.h"
#include "MenuOption.h"

#include <GL/glut.h>


/***********************************************************************************************************************
* Static members
***********************************************************************************************************************/
Process *Process::instance = nullptr;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Process::Process(int argc, char **argv, bool printData, StoreService *storeServiceIn)
{
	string 	fileName;			// Configuration file name.

	// Initialize configuration.
	fileName = argv[2];
	Config::readConfig(fileName);

	// Check flag to print data to screen.
	this->log = new Logging("log.txt", printData);

    storeService = storeServiceIn;
    dispManager = new DisplayManager(argc, argv);

	this->menu = Menu(storeServiceIn->getStatus());

	// Function to execute by GLUT.
	glutDisplayFunc(executeWrapper);
}


Process::~Process()
{
	// Deallocate resources
	delete log;
	delete dispManager;
	delete storeService;
}


void Process::start()
{
	// GLUT main function.
	glutMainLoop();
}


void Process::setInstance(Process *process)
{
	instance = process;
}


/**
 * @fn      executeWrapper
 * @brief   wrapper to call "execute" function.
 */
void Process::executeWrapper()
{
	instance->execute();
}


/**
 * @fn      execute
 * @brief   main loop that processes events from menu and executes the
 *			selected option and updates menu and internal status.
 */
void Process::execute()
{
    Command *cmd=nullptr;           // Command to execute
    CommandResult *result;

	static bool firstTime=true;
	int		option=0;			// Option to be executed.

	// Get option to be executed.
	option = menu.getMenuOption();

	// Execute option.
	switch(option)
	{
		// Read parameters from configuration file.
		case PARAMETERS:
        case RANDOMLY:
        case CLUSTER:
		case STAR_TRIANGULATION:
		case DELAUNAY:
		case CONVEX_HULL:
        case VORONOI:
        case GABRIEL:
        case TRIANGULATION_PATH:
        case VORONOI_PATH:
        case CLOSEST_POINT:
        case FIND_FACE:
        case TWO_CLOSEST:
        case FILTER_EDGES:
        case CIRCUMCENTRES:
        case EDGE_CIRCLES:
        case DCEL_INFO:
        case VORONOI_INFO:
        case CLEAR:
        case READ_POINTS_FLAT_FILE:
        case READ_POINTS_DCEL_FILE:
        case READ_DCEL:
        case READ_DELAUNAY:
        case READ_VORONOI:
        case READ_GABRIEL:
        case WRITE_POINTS:
        case WRITE_DCEL:
        case WRITE_DELAUNAY:
        case WRITE_VORONOI:
        case WRITE_GABRIEL:
		{
            // Create command
            cmd = CommandFactory::create(option, storeService);

            // Run command
            cmd->run();

            // Process results
            result = cmd->getResult();
            if (result->wasSuccess())
            {
                // Update menu status
                result->updateStatus();

                // Get displaybale elements
                vector<Displayable*> vDisplayable(0);
                result->createDisplayables(vDisplayable);
                dispManager->add(vDisplayable);

                dispManager->process();
            }

            // Update menu entries.
            menu.updateMenu();

			break;
		}
		// Quit application.
		case QUIT:
		{
			// Quit application.
            delete log;
            delete dispManager;
            delete storeService;
            exit(0);
		}
		default:
		{
			if (firstTime)
			{
				// Clear screen.
				firstTime = false;
                dispManager->process();
			}
			break;
		}
	}

	// Delete iteration resources
    delete cmd;

    // Reset menu option.
	menu.resetMenuOption();
}
