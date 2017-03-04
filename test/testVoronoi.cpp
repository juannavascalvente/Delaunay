/*
 * testVoronoi.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: juan
 */

#include "Delaunay.h"
#include "Voronoi.h"
#include "testVoronoi.h"
#include <stdio.h>
#include <unistd.h>

//#define DEBUG_TEST_VORONOI_BUILD_INIT
//#define DEBUG_TEST_VORONOI_BUILD
#define DEBUG_TEST_VORONOI_COMPARE

void TestVoronoiBuild::initParameters()
{
	string nPointsStr = TYPE_N_POINTS_LABEL;
	string nTestsStr = TYPE_N_TESTS_LABEL;
	string nDeltaStr = TYPE_DELTA_STEP_LABEL;
	string nStepsStr = TYPE_N_STEPS_LABEL;

	// Add # points parameter.
	NumericValidator *nPointsDefinition = new NumericValidator(3, INT_MAX);
	ParameterInt *nPointsParameter = new ParameterInt(nPointsStr, nPointsDefinition);
	this->parameters.add(nPointsParameter);

	// Add # tests parameter.
	NumericValidator *nTestsDefinition = new NumericValidator(1, INT_MAX);
	ParameterInt *nTestsParameter = new ParameterInt(nTestsStr, nTestsDefinition);
	this->parameters.add(nTestsParameter);

	// Add delta # points parameter.
	NumericValidator *deltaPointsDefinition = new NumericValidator(1, 100);
	ParameterInt *deltaPointsParameter = new ParameterInt(nDeltaStr, deltaPointsDefinition);
	this->parameters.add(deltaPointsParameter);

	// Add # steps parameter.
	NumericValidator *stepsDefinition = new NumericValidator(0, INT_MAX);
	ParameterInt *stepsParameter = new ParameterInt(nStepsStr, stepsDefinition);
	this->parameters.add(stepsParameter);
#ifdef DEBUG_TEST_VORONOI_BUILD_INIT
	// Print parameters data.
	this->printParameters();
#endif
}

void TestVoronoiBuild::printParameters()
{
	cout << "---------------------------------------------------" << endl;
	cout << "Test Voronoi printParameters" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Number of points " << this->nPoints << endl;
	cout << "Delta points " << this->deltaPoints << endl;
	cout << "Number of steps " << this->nSteps << endl;
	cout << "Number of test  " << this->nTests << endl;
	cout << "Output folder " << this->outputFolder << endl;
	cout << "---------------------------------------------------" << endl;
}

void TestVoronoiBuild::applyParameter(Parameter *parameter, string value)
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
		cout << "different label" << endl;
	}
}

void TestVoronoiBuild::main()
{
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	Voronoi		voronoi;			// Voronoi data.
	int			failedTestIndex=1;	// Index of last failed test.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	string 		fileName;
	int			currentNPoints=0;

	// Print parameters data.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);

		for (testIndex=0; testIndex< this->nTests ;testIndex++)
		{
			// Execute current test.
			delaunay.setDCEL(&dcel);
			if (!dcel.generateRandom(currentNPoints))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else if (!delaunay.incremental())
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Start building voronoi diagram test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);

				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				fileName = this->outputFolder + "Voronoi_" + convert.str() + ".txt";
				this->dump(fileName, dcel);
				if (!voronoi.init(&dcel))
				{
					// Compute Voronoi diagram.
					if (!voronoi.build())
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Voronoi diagram built for test ");
						remove(fileName.c_str());
					}
					else
					{
						failedTestIndex++;
						Logging::buildText(__FUNCTION__, __FILE__, "Error building voronoi diagram in test ");
					}
				}
				else
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error initializing voronoi in test ");
				}
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::write(true, Info);
				sleep(1);

				// Reset voronoi data.
				voronoi.reset();
			}
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}
}


void TestVoronoiBuild::dump(string dcelFileName, Dcel &dcel)
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

void TestVoronoiBuild::deallocate()
{
	int i=0;			// Loop counter.
	int nElements=0;	// # loop iterations.

	cout << "Test Voronoi build deallocate" << endl;

	nElements = this->parameters.getNElements();
	// PENDING DEALLOCATE DEFINITIONS
	/*
	for (i=0; i<nElements ;i++)
	{

	}
	*/
}


void TestVoronoiCompare::initParameters()
{
	// Add # points parameter.
	//FileValidator fileListDefinition(nullString);
	//Parameter fileListParameter(TEST_FILE_LABEL, &fileListDefinition);
	//this->parameters.add(fileListParameter);
#ifdef DEBUG_TEST_VORONOI_COMPARE
	// Print parameters data.
	this->printParameters();
#endif
}

void TestVoronoiCompare::printParameters()
{
	cout << "Test Voronoi printParameters" << endl;

	cout << "Input file list " << this->filesNamesFile << endl;
}

bool TestVoronoiCompare::prepare()
{
	bool success=true;
	string 	line;
	ifstream ifs;
	ifstream fileTestIfs;

	cout << "Test Voronoi compare PREPARE" << endl;

	// Open configuration file.
	ifs.open(this->filesNamesFile.c_str(), ios::in);

	// Check file is opened.
	if (!ifs.is_open())
	{
		success = false;
	}
	else
	{
		// Check all files exist.
		while (getline(ifs, line))
		{
			// Open file.
			fileTestIfs.open(line.c_str(), ios::in);

			// Check file exists.
			if (fileTestIfs.is_open())
			{
				filesList.add(line);
				fileTestIfs.close();
			}
		}

		// Close file list.
		ifs.close();
	}

	return(success);
}

void TestVoronoiCompare::main()
{

}



