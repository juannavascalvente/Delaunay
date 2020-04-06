
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Process.h"
#include "Command.h"
#include "CommandFactory.h"
#include "MenuOption.h"


/***********************************************************************************************************************
* Static members
***********************************************************************************************************************/
Process         *Process::instance = nullptr;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Process::Process(int argc, char **argv, bool printData, StoreService *storeServiceIn)
{
	string 	fileName;			// Configuration file name.

	// Initialize configuration.
	fileName = argv[2];
	Config::readConfig(fileName);

	// Allocate resources
	this->log = new Logging("log.txt", printData);
    storeService = storeServiceIn;

    // Initialize display
    dispManager = new DisplayManager(argc, argv, storeServiceIn);
    dispManager->updateMenu();
    dispManager->setLoopFunction(executeWrapper);
}


Process::~Process()
{
	// Deallocate resources
    deallocateResources();
}


void Process::start()
{
	// GLUT main function.
    dispManager->startLoop();
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
	// Get option to be executed.
    int option = Menu::getMenuOption();
	if (option == quit)
    {
        // Quit application.
        deallocateResources();
        exit(0);
    }

    // Create command
    Command *cmd = CommandFactory::create(option, storeService);

    // Run command
    cmd->run();

    // Process results
    CommandResult *result = cmd->getResult();
    if (result->wasSuccess())
    {
        // Get displayable elements
        vector<Displayable*> vDisplayable(0);
        result->getItems(vDisplayable);
        dispManager->add(vDisplayable);
        dispManager->process();

        // Update menu entries.
        dispManager->updateMenu();
    }

	// Delete iteration resources
    delete cmd;

    // Reset menu option.
	Menu::resetMenuOption();
}


/**
 * @fn      deallocateResources
 * @brief   Free allocated resources
 */
void Process::deallocateResources() const
{
    delete log;
    delete dispManager;
    delete storeService;
}
