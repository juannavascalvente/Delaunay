/*
 * Logging.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: jnavas
 */

#include <Logging.h>
#include <string.h>
#include <sstream>
#include <stdarg.h>

bool forceStdOutput;
bool opened;					// Log file opened flag.
bool concat;					// Flag to concatenate text to be printed.
string file;					// File source code name.
string function;				// Function name.
string message;					// Message text.
ofstream *ofs;					// Output stream.
int	fileCounter;

// Internal static variables.
//Logging *Logging::instance=NULL;

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Logging::Logging(const string fileName, bool force)
{
	// Initialize attributes.
	concat = false;
	ofs = new ofstream;
	opened = true;
	// PENDING THROW EXCEPTION IF NOT OPEN.
	ofs->open(fileName.c_str(), std::ofstream::out);
	function = "";
	file = "";
	message = "";
	fileCounter = 0;
	forceStdOutput = force;
}

Logging::~Logging()
{
	// Check if file opened.
	if (opened)
	{
		ofs->close();
		opened = false;
		delete ofs;
	}
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	buildText
* IN:		text		text to print
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: adds a string to the message to print
***************************************************************************/
void Logging::buildText(string text)
{
	message = message + text;
	concat = true;
}

/***************************************************************************
* Name: write
* IN:		function		function that writes the log
* 			text			message to be logged
* 			value			integer to be logged
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: starts infinite loop.
***************************************************************************/
void Logging::buildText(int value)
{
	string Result;
	ostringstream convert;
	convert << value;

	// Check if text must be concatenated.
	if (concat)
	{
		message = message + convert.str();
	}
	else
	{
		// Copy function and text to be printed.
		message = convert.str();
		concat = true;
	}
}

/***************************************************************************
* Name: 	buildTestHeader
* IN:		testId		test id
* 			nTest		number of tests to execute
* 			testName	test name
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: updates function name and text to be printed.
***************************************************************************/
void Logging::buildTestHeader(int testId, int nTests, string testName)
{
	ostringstream convertTestId;
	convertTestId << testId;

	ostringstream convertnTests;
	convertnTests << nTests;

	Logging::buildText("**********************************************\n");
	Logging::buildText("Executing test ");
	Logging::buildText(convertTestId.str());
	Logging::buildText("/");
	Logging::buildText(convertnTests.str());
	Logging::buildText("\nTest name: ");
	Logging::buildText(testName);
	Logging::buildText("\n**********************************************");
	Logging::write(true, Testing);
}

/***************************************************************************
* Name: buildText
* IN:		function		function that writes the log
* 			f				file where function is implemented
* 			text			message to be logged
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: updates function name and text to be printed.
***************************************************************************/
void Logging::buildText(string func, string f, string text)
{
	// Check if text must be concatenated.
	if (concat)
	{
		message = message + text;
	}
	else
	{
		// Copy function and text to be printed.
		function = func;
		file = f;
		message = text;
		concat = true;
	}
}


/***************************************************************************
* Name: write
* IN:		function		function that writes the log
* 			text			message to be logged
* 			value			integer to be logged
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: starts infinite loop.
***************************************************************************/
void Logging::buildText(string func, string f, int value)
{
	string Result;
	ostringstream convert;
	convert << value;

	// Check if text must be concatenated.
	if (concat)
	{
		message = message + convert.str();
	}
	else
	{
		// Copy function and text to be printed.
		function = func;
		file = f;
		message = convert.str();
		concat = true;
	}
}

/***************************************************************************
* Name: write
* IN:		func		function that writes the log
* 			f			message to be logged
* 			value		TYPE to be logged
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: writes TYPE to the logging file
***************************************************************************/
void Logging::buildText(string func, string f, TYPE value)
{
	string Result;
	ostringstream convert;
	convert << value;

	// Check if text must be concatenated.
	if (concat)
	{
		message = message + convert.str();
	}
	else
	{
		// Copy function and text to be printed.
		function = func;
		file = f;
		message = convert.str();
		concat = true;
	}
}

/***************************************************************************
* Name: write
* IN:		func		function that writes the log
* 			f			message to be logged
* 			value		double to be logged
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: writes a double to the logging file
***************************************************************************/
#ifdef FLOAT_TYPE
void Logging::buildText(string func, string f, double value)
{
	string Result;
	ostringstream convert;
	convert << value;

	// Check if text must be concatenated.
	if (concat)
	{
		message = message + convert.str();
	}
	else
	{
		// Copy function and text to be printed.
		function = func;
		file = f;
		message = convert.str();
		concat = true;
	}
}
#endif

/***************************************************************************
* Name: 	buildText
* IN:		function		function that writes the log
* 			file			file where info is produced.
* 			point			point to write
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: starts infinite loop.
***************************************************************************/
void Logging::buildText(string func, string f, Point<TYPE> *point)
{
	string Result;
	ostringstream convert;
	convert << "(" << point->getX() << "," << point->getY() << ")";

	// Check if text must be concatenated.
	if (concat)
	{
		message = message + convert.str();
	}
	else
	{
		// Copy function and text to be printed.
		function = func;
		file = f;
		message = convert.str();
		concat = true;
	}
}

/***************************************************************************
* Name: 	buildRange
* IN:		function		function that writes the log
* 			text			message to be logged
* 			start			lower range bound
* 			end				upper range bound
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: Writes range boundaries
***************************************************************************/
void Logging::buildRange(string func, string f, int start, int end)
{
	string Result;
	ostringstream convert;
	convert << "Range is [" << start << "," << end << "]";

	// Check if text must be concatenated.
	if (concat)
	{
		message = message + convert.str();
	}
	else
	{
		// Copy function and text to be printed.
		function = func;
		file = f;
		message = convert.str();
		concat = true;
	}
}

/***************************************************************************
* Name: write
* IN:		print			flag to print in screen or not.
*
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	writes data to the log file. Is "print" is true then it
* 				also prints the data to standard output.
***************************************************************************/
void Logging::write(bool print, enum MessageCategory category)
{
	// Settings colors.
	std::string red = "\x1B[31m";
	std::string yellow = "\x1B[33m";
	std::string green = "\x1B[32m";
	std::string blue = "\x1b[34m";
	std::string reset = "\x1B[0m";


	// Check log file is already open.
	if (opened)
	{
		// Write log text.
		if ((category != Testing) && (category != Successful))
		{
			(*ofs) << file << "\t\tFunction " << function << "():\t\t" << message << std::endl;
		}
		else
		{
			(*ofs) << message << std::endl;
		}

		// Check message category.
		switch (category)
		{
			// Error. Force output and set red color.
			case Error:
			{
				std::cout << file << "\t\tFunction " << function << "():\t\t" << red << message << reset << std::endl;
				break;
			}
			// Error. Force output and set yellow color.
			case Warning:
			{
				std::cout << file << "\t\tFunction " << function << "():\t\t" << yellow << message << reset << std::endl;
				break;
			}
			// Successful. Print green for test successfully executions.
			case Successful:
			{
				std::cout << green << message << reset << std::endl;
				break;
			}
			// Testing. Print blue for test headers executions.
			case Testing:
			{
				std::cout << blue << message << reset << std::endl;
				break;
			}
			default:
			{
				// Print to standard output.
				if (print && forceStdOutput)
				{
					std::cout << file << "\t\tFunction " << function << "():\t\t" << message << std::endl;
				}
				break;
			}
		}

		// Empty texts.
		concat = false;
		message = "";
		function = "";
		file = "";
	}
}


