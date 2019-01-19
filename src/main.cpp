/*
 * main.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */
#include <unistd.h>
#include <time.h>

#include "defines.h"
#include "Process.h"
#include "Statistics.h"
#include "Tester.h"

#include <string.h>
#include <iostream>
using namespace std;

#define STATISTICS_EXECUTION	"-stat"
#define TEST_EXECUTION			"-test"
#define VISUAL_EXECUTION		"-visual"
#define PRINT_DEBUG_DATA		"-print"

#define UNKNOWN					-1
#define GENERAL					0
#define VISUAL					1
#define TEST					2

int	getExecutionType(char *type);
void printUsage(int type, char *exec);
int executeTests(int argc, char** argv);
int executeVisual(int argc, char** argv);

/***************************************************************************
* Name: 	getExecutionType
* IN:		type		execution type.
* OUT:		NONE
* RETURN:	execution type in integer format.
* GLOBAL:	NONE
* Description: 	returns the execution type in integer format.
***************************************************************************/
int	 getExecutionType(char *type)
{
	int	executionType=0;		// Return value.

	// Check "test" execution.
	if (strcmp( type, TEST_EXECUTION) == 0)
	{
		executionType = TEST;
	}
	// Check "visual" execution.
	else if (strcmp( type, VISUAL_EXECUTION) == 0)
	{
		executionType = VISUAL;
	}
	else
	{
		executionType = UNKNOWN;
	}

	return(executionType);
}

/***************************************************************************
* Name: 	printUsage
* IN:		type		execution type.
* OUT:		NONE
* RETURN:	SUCCESS 	if execution finished successfully
* 			FAILURE		i.o.c.
* GLOBAL:	NONE
* Description: 	prints application usage.
***************************************************************************/
void printUsage(int type, char *exec)
{
	switch(type)
	{
		case GENERAL:
		{
			std::cerr << "Usage: " << exec << " type [configFile]" << std::endl;
			std::cerr << "type(enumerated):\t\tvisual (" << VISUAL_EXECUTION <<
					") or test (" << TEST_EXECUTION << ")" << std::endl;
			std::cerr << "configFile(string):\t\tconfiguration file (only " <<
					VISUAL_EXECUTION << ")" << std::endl;
			std::cerr << std::endl << std::endl;
			break;
		}
		case VISUAL:
		{
			std::cerr << "Usage: " << exec << " -visual configFile [";
			std::cerr << PRINT_DEBUG_DATA << "]." << std::endl;
			std::cerr << std::endl << std::endl;
			break;
		}
		case TEST:
		{
			std::cerr << "Usage: " << exec << " -test configFile logFile" << std::endl;
			std::cerr << std::endl << std::endl;
			break;
		}
		default:
		{
			std::cerr << "Wrong type usage." << std::endl << std::endl;
			break;
		}
	}
}

/***************************************************************************
* Name: 	executeTests
* IN:		argc		input # arguments
* 			argv		input configuration test file.
* OUT:		NONE
* RETURN:	SUCCESS 	if execution finished successfully
* 			FAILURE		i.o.c.
* GLOBAL:	NONE
* Description: 	executes the application in test mode.
***************************************************************************/
int executeTests(int argc, char** argv)
{
	int 	ret=SUCCESS;			// Return value.

	Tester tester;
	ret = tester.Execute(argc, argv);

	return ret;
}

/***************************************************************************
* Name: 	executeVisual
* IN:		argc		input # arguments
* 			argv		input arguments
* OUT:		NONE
* RETURN:	SUCCESS 	if execution finished successfully
* 			FAILURE		i.o.c.
* GLOBAL:	NONE
* Description: 	executes the application in visual mode.
***************************************************************************/
int executeVisual(int argc, char** argv)
{
	int 	ret=SUCCESS;			// Return value.
	bool	printData=false;		// Print data into screen.

	// Check visual arguments.
	switch (argc)
	{
		case 3:
		{
			printData = false;
			break;
		}
		case 4:
		{
			if (strcmp(argv[3], PRINT_DEBUG_DATA) == 0)
			{
				printData = true;
			}
			else
			{
				std::cout << "Unknown fourth argument: " << argv[3];
				std::cout << std::endl << std::endl;
				ret = FAILURE;
			}
			break;
		}
		default:
		{
			std::cout << "Wrong number of arguments in visual mode: " << argc;
			std::cout << ". Must be 3 or 4." << std::endl << std::endl;
			ret = FAILURE;
			break;
		}
	}

	if (ret != FAILURE)
	{
		// Create process.
		Process process = Process(argc, argv, printData);
		process.setInstance(&process);

		// Main loop.
		process.start();
	}
	else
	{
		printUsage(VISUAL, argv[0]);
	}

	return ret;
}

/***************************************************************************
* Name: 	main
* IN:		argc		input # arguments
* 			argv		input arguments
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	main function.
***************************************************************************/
int main(int argc, char **argv)
{
	int 	ret=SUCCESS;			// Return value.
	int		option=0;				// Input option.

	// Check "test" execution.
	if ((argc >= 2) && (argc <= 4))
	{
		// Get input menu option.
		option=getExecutionType(argv[1]);

		switch(option)
		{
			// Visual execution.
			case VISUAL:
			{
				ret = executeVisual(argc, argv);
				break;
			}
			// Run functional tests or statistics.
			case TEST:
			{
				ret = executeTests(argc, argv);
				break;
			}
			// Unknown execution.
			default:
			{
				std::cout << "Unknown 2nd parameter " << argv[1] << std::endl;
				std::cout << "Allowed values: " << TEST_EXECUTION << "," << \
						VISUAL_EXECUTION << std::endl;
				ret = FAILURE;
				printUsage(GENERAL, argv[0]);
				break;
			}
		}
	}
	// Wrong # arguments.
	else
	{
		std::cout << "Wrong number of arguments " << argc << std::endl << std::endl;
		printUsage(GENERAL, argv[0]);
		ret = FAILURE;
	}

	return(ret);
}
