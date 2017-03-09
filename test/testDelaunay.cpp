/*
 *
 *  Created on: Mar 4, 2017
 *      Author: juan
 */
#include "testDelaunay.h"

#include "Delaunay.h"
#include "generic.h"
#include "Label.h"
#include <unistd.h>

//#define DEBUG_DELAUNAY_COMPARE_PREPARE
//#define DEBUG_TEST_DELAUNAY_BUILD_INIT
//#define DEBUG_TEST_DELAUNAY_BUILD

/***************************************************************************
* Name: 	initParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	registers the test parameters
***************************************************************************/
void TestDelaunayBuild::initParameters()
{
	// Add # points parameter.
	NumericValidator *nPointsDefinition = new NumericValidator(3, INT_MAX);
	ParameterInt *nPointsParameter = new ParameterInt(TYPE_N_POINTS_LABEL, nPointsDefinition);
	this->parameters.add(nPointsParameter);

	// Add # tests parameter.
	NumericValidator *nTestsDefinition = new NumericValidator(1, INT_MAX);
	ParameterInt *nTestsParameter = new ParameterInt(TYPE_N_TESTS_LABEL, nTestsDefinition);
	this->parameters.add(nTestsParameter);

	// Add delta # points parameter.
	NumericValidator *deltaPointsDefinition = new NumericValidator(1, 100);
	ParameterInt *deltaPointsParameter = new ParameterInt(TYPE_DELTA_STEP_LABEL, deltaPointsDefinition);
	this->parameters.add(deltaPointsParameter);

	// Add # steps parameter.
	NumericValidator *stepsDefinition = new NumericValidator(0, INT_MAX);
	ParameterInt *stepsParameter = new ParameterInt(TYPE_N_STEPS_LABEL, stepsDefinition);
	this->parameters.add(stepsParameter);
#ifdef DEBUG_TEST_DELAUNAY_BUILD_INIT
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
void TestDelaunayBuild::printParameters()
{
	Logging::buildText("", "", "Number of points ");
	Logging::buildText("", "", this->nPoints);
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, "Delta points ");
	Logging::buildText(__FUNCTION__, __FILE__, this->deltaPoints);
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, "Number of steps ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, "Number of test ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, "Output folder ");
	Logging::buildText(__FUNCTION__, __FILE__, this->outFolder);
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, \
						"**********************************************");
	Logging::write(true, Info);
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
void TestDelaunayBuild::applyParameter(Parameter *parameter, string value)
{
	string parameterName;
	int convertedValue=0;

	// Get parameter label.
	parameterName = parameter->getName();
	if (parameterName == TYPE_N_POINTS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;
		this->setPoints(convertedValue);
	}
	else if (parameterName == TYPE_N_TESTS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setTests(convertedValue);
	}
	else if (parameterName == TYPE_DELTA_STEP_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setDeltaPoints(convertedValue);
	}
	else if (parameterName == TYPE_N_STEPS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setSteps(convertedValue);
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Unknown label parameter:");
		Logging::buildText(__FUNCTION__, __FILE__, parameterName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	builds several Delaunay triangulation using different set of
* 				points and if any fails the dcel data is written to a file.
***************************************************************************/
void TestDelaunayBuild::main()
{
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	int			failedTestIndex=0;	// Index of last failed test.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	string 		fileName;
	int			currentNPoints=0;
	int			nTests=0;
	int			testId=1;

	nTests = this->nSteps*this->nTests;

	// Print test parameters.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
#ifdef DEBUG_TEST_DELAUNAY_BUILD
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);
#endif
		for (testIndex=0; testIndex<this->nTests ;testIndex++)
		{
			// Execute current test.
			delaunay.setDCEL(&dcel);
			if (!dcel.generateRandom(currentNPoints))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else
			{
#ifdef DEBUG_TEST_DELAUNAY_BUILD
				Logging::buildText(__FUNCTION__, __FILE__, "Start building Delaunay incremental test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);
#endif
				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				fileName = this->outFolder + "Delaunay_" + convert.str() + ".txt";
				this->dump(fileName, dcel);
				if (delaunay.incremental())
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Test OK ");
					Logging::buildText(__FUNCTION__, __FILE__, testId);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, nTests);
					Logging::write(true, Successful);
					remove(fileName.c_str());
				}
				else
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay diagram in test ");
					Logging::buildText(__FUNCTION__, __FILE__, testId);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, nTests);
					Logging::write(true, Error);
				}
				sleep(1);

				// Reset Delaunay data.
				delaunay.reset();
				testId++;
			}
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}

	Logging::buildText(__FUNCTION__, __FILE__, "Tests executed successfully ");
	Logging::buildText(__FUNCTION__, __FILE__, nTests-failedTestIndex);
	Logging::buildText(__FUNCTION__, __FILE__, "/");
	Logging::buildText(__FUNCTION__, __FILE__, nTests);
	if (failedTestIndex == 0)
	{
		Logging::write(true, Successful);
	}
	else
	{
		Logging::write(true, Error);
	}
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
void TestDelaunayBuild::dump(string dcelFileName, Dcel &dcel)
{
	ofstream ofs;			// Output file.

	// Open file.
	ofs.open(dcelFileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Write DCEL data.
		if (!dcel.writePoints(dcelFileName, dcel.getNVertex()))
		{
			cout << "Cannot open file " << dcelFileName << " to write dcel data" << endl;
		}
	}
	// Error opening points file.
	else
	{
		cout << "Cannot open file " << dcelFileName << " to write points" << endl;
	}
}

/***************************************************************************
* Name: 	initParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	registers the test parameters
***************************************************************************/
void TestDelaunayCompare::initParameters()
{
	// Add files list parameter.
	FileValidator *filesValidator = new FileValidator();
	ParameterFile *fileParameter = new ParameterFile(TEST_FILE_LABEL, filesValidator);
	this->parameters.add(fileParameter);
#ifdef DEBUG_TEST_DELAUNAY_COMPARE
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
void TestDelaunayCompare::printParameters()
{
	Logging::buildText(__FUNCTION__, __FILE__, "Files list: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->filesNamesFile);
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, \
						"**********************************************");
	Logging::write(true, Info);
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
void TestDelaunayCompare::applyParameter(Parameter *parameter, string value)
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
bool TestDelaunayCompare::prepare()
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
#ifdef DEBUG_DELAUNAY_COMPARE_PREPARE
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
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	compares the Delaunay read from input files with the Delaunay
* 				computed using the points in the input files
***************************************************************************/
void TestDelaunayCompare::main()
{
	int 	i=0;				// Loop counter.
	int		nFailedTests=0;		// # tests failed.
	string 	fileName;			// File name.
	Dcel	originalDcel;		// Original dcel data.
	Dcel	dcel;				// Dcel data.
	Delaunay	delaunay;		// Delaunay data.
	int		nTests=0;
	int		testId=1;

	nTests = this->filesList.getNElements();

	// Print test parameters.
	this->printParameters();

	Logging::buildText(__FUNCTION__, __FILE__, "Number of files to compare: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
	Logging::write(true, Info);

	// Read all files.
	for (i=0; i<this->filesList.getNElements() ;i++)
	{
		// Open file.
		fileName = *this->filesList.at(i);
		if (!originalDcel.read(fileName, false))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error reading original file: ");
			Logging::buildText(__FUNCTION__, __FILE__, fileName);
			Logging::write(true, Error);
		}
		else if (!dcel.readPoints(fileName, false))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error reading points file: ");
			Logging::buildText(__FUNCTION__, __FILE__, fileName);
			Logging::write(true, Error);
		}
		else
		{
			// Reset dcel to remove all data except point coordinates.
			dcel.clean();

			// Execute current test.
			delaunay.setDCEL(&dcel);
			if (!delaunay.incremental())
			{
				nFailedTests++;
				Logging::buildText(__FUNCTION__, __FILE__, "Failed test id: ");
				Logging::buildText(__FUNCTION__, __FILE__, i+1);
				Logging::write(true, Error);
			}
			else
			{
				if (dcel == originalDcel)
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Test OK ");
					Logging::buildText(__FUNCTION__, __FILE__, testId);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, nTests);
					Logging::write(true, Successful);
				}
				else
				{
					nFailedTests++;

					// Write test data.
					ostringstream convert;
					convert << nFailedTests;
					fileName = this->outFolder + "Delaunay_" + convert.str() + ".txt";
					cout << "CREATING " << fileName << endl;
					this->dump(fileName, dcel);

					// Print log error.
					Logging::buildText(__FUNCTION__, __FILE__, "Test failed when comparing dcel. Test id:");
					Logging::buildText(__FUNCTION__, __FILE__, testId);
					Logging::write(true, Error);
				}
			}

			// Reset Delaunay data.
			testId++;
			delaunay.reset();
			originalDcel.reset();
			dcel.reset();
		}
	}

	Logging::buildText(__FUNCTION__, __FILE__, "Tests executed successfully ");
	Logging::buildText(__FUNCTION__, __FILE__, nTests-nFailedTests);
	Logging::buildText(__FUNCTION__, __FILE__, "/");
	Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
	if (nFailedTests == 0)
	{
		Logging::write(true, Successful);
	}
	else
	{
		Logging::write(true, Error);
	}
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
void TestDelaunayCompare::dump(string dcelFileName, Dcel &dcel)
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
			cout << "Cannot open file " << dcelFileName << " to write dcel data" << endl;
		}
	}
	// Error opening points file.
	else
	{
		cout << "Cannot open file " << dcelFileName << " to write points" << endl;
	}
}

