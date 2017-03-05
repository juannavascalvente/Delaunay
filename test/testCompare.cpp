/*
 * testCompare.cpp
 *
 *  Created on: Mar 5, 2017
 *      Author: juan
 */

#include "Delaunay.h"
#include "generic.h"
#include "testCompare.h"

//#define DEBUG_TEST_COMPARE_INIT
//#define DEBUG_TEST_COMPARE_PREPARE

/***************************************************************************
* Name: 	initParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	registers the test parameters
***************************************************************************/
void TestCompare::initParameters()
{
	// Add files list parameter.
	FileValidator *filesValidator = new FileValidator();
	ParameterFile *fileParameter = new ParameterFile(TEST_FILE_LABEL, filesValidator);
	this->parameters.add(fileParameter);
#ifdef DEBUG_TEST_COMPARE_INIT
	// Print parameters data.
	this->printParameters();
#endif
}

/***************************************************************************
* Name: 	printParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints the test parameters.
***************************************************************************/
void TestCompare::printParameters()
{
	cout << "Test Delaunay printParameters" << endl;
	cout << "Files list: " << this->filesNamesFile << endl;
}

/***************************************************************************
* Name: 	applyParameter
* IN:		parameter		parameter to update
* 			value			value to set
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	set a class attribute to "value"
***************************************************************************/
void TestCompare::applyParameter(Parameter *parameter, string value)
{
	string parameterName;

	// Get parameter label.
	parameterName = parameter->getName();
	if (parameterName == TEST_FILE_LABEL)
	{
		// Copy file name list.
		this->setFileNamesFile(value);
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Unknown label parameter:");
		Logging::buildText(__FUNCTION__, __FILE__, parameterName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	prepare
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	executes any previous checks or actions previous to the
* 				tests execution
***************************************************************************/
bool TestCompare::prepare()
{
	bool success=true;
	string 	line;
	ifstream ifs;
	ifstream fileTestIfs;

	// Open configuration file.
	ifs.open(this->filesNamesFile.c_str(), ios::in);

	// Check file is opened.
	if (!ifs.is_open())
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->filesNamesFile.c_str());
		Logging::write(true, Error);
		success = false;
	}
	else
	{
		// Check all files exist.
		while (getline(ifs, line))
		{
			// Skip empty lines.
			line = trim(line);
			if (line.length() > 0)
			{
				// Open file.
				fileTestIfs.open(line.c_str(), ios::in);

				// Check file exists.
				if (fileTestIfs.is_open())
				{
#ifdef DEBUG_TEST_COMPARE_PREPARE
					Logging::buildText(__FUNCTION__, __FILE__, "Adding file to list: ");
					Logging::buildText(__FUNCTION__, __FILE__, line);
					Logging::write(true, Info);
#endif
					this->filesList.add(line);
					fileTestIfs.close();
				}
				else
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Skip file that does not exist: ");
					Logging::buildText(__FUNCTION__, __FILE__, line);
					Logging::write(true, Error);
				}
			}
		}

		// Close file list.
		ifs.close();
	}

	return(success);
}


/***************************************************************************
* Name: 	dump
* IN:		dcelFileName	dcelFilefile where dcel points are written
* 			dcel			dcel data to write
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Writes to output files the data required to reproduce the fail
***************************************************************************/
void TestCompare::dump(string dcelFileName, Dcel &dcel)
{
	ofstream ofs;			// Output file.

	// Open file.
	ofs.open(dcelFileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Write DCEL data.
		if (!dcel.write(dcelFileName, false))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Cannot write to file: ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::write(true, Error);
		}
	}
	// Error opening points file.
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Cannot open file: ");
		Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	deallocate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	free test resources. The data allocated are the parameters
* 				and validators allocated in initParameters
***************************************************************************/
void TestCompare::deallocate()
{
	cout << "Deallocating TestCompare" << endl;
	// PENDING https://github.com/juannavascalvente/Delaunay/issues/2
}
