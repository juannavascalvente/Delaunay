/*
 * Process.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef PROCESS_H_
#define PROCESS_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "ConfigService.h"
#include "DisplayManager.h"
#include "Logging.h"
#include "Menu.h"
#include "StoreService.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Process
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    DisplayManager      *dispManager;
    ConfigService       *configService;
    StoreService        *storeService;

	Menu 			    menu;		    // Menu object.
	Logging 		    *log;			// Log file.

    /*******************************************************************************************************************
     * Private methods declaration
     *******************************************************************************************************************/
	void execute();
	static void executeWrapper();

protected:
	static Process *instance;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Process(int argc, char **argv, bool printData, StoreService *storeServiceIn, ConfigService *configService);
	~Process();

    /**
     * @fn      start
     * @brief   starts infinite loop
     */
    void start();

    /**
     * @fn      setInstance
     * @brief   Sets "instance" to the object that is going to be executed
     *          by the main loop process
     * @param   process   instance to be executed by main loop
     */
	void setInstance(Process *process);
};

#endif /* PROCESS_H_ */
