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
//#define DEBUG_VORONOI_COMPARE_PREPARE
//#define DEBUG_TEST_VORONOI_COMPARE

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
void TestVoronoiBuild::main()
{
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	Voronoi		voronoi;			// Voronoi data.
	int			failedTestIndex=0;	// Index of last failed test.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	int			currentNPoints=0;

#ifdef DEBUG_TEST_VORONOI_BUILD
	// Print test parameters.
	this->printParameters();
#endif

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
#ifdef DEBUG_TEST_VORONOI_BUILD
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);
#endif
		for (testIndex=0; testIndex< this->nTests ;testIndex++)
		{
#ifdef DEBUG_TEST_VORONOI_BUILD
			Logging::buildText(__FUNCTION__, __FILE__," Start building voronoi diagram test ");
			Logging::buildText(__FUNCTION__, __FILE__, testIndex + 1);
			Logging::buildText(__FUNCTION__, __FILE__, "/");
			Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
			Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
			Logging::write(true, Info);
#endif
			// Generate random set of points.
			if (!dcel.generateRandom(currentNPoints))
			{
				Logging::buildText(__FUNCTION__, __FILE__,
						"Error generating data set in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex + 1));
				Logging::write(true, Error);
			}
			else
			{
				// Write test data.
				string 	fileName;
				ostringstream convert;
				convert << failedTestIndex;
				fileName = outFolder + "Voronoi_" + convert.str() + ".txt";
				this->dump(fileName, dcel);

				// Build Voronoi area.
				if (this->buildVoronoi(dcel, voronoi))
				{
					remove(fileName.c_str());
					Logging::buildText(__FUNCTION__, __FILE__, "Test OK ");
					Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
					Logging::write(true, Successful);
				}
				else
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error building Voronoi diagram in test ");
					Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
					Logging::write(true, Error);
				}
			}

			this->testCounter++;
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}

	Logging::buildText(__FUNCTION__, __FILE__, "Tests executed successfully ");
	Logging::buildText(__FUNCTION__, __FILE__, this->totalTests-failedTestIndex);
	Logging::buildText(__FUNCTION__, __FILE__, "/");
	Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
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

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	compares the Voronoi read from input files with the voronoi
* 				computed using the points in the input files
***************************************************************************/
void TestVoronoiCompare::main()
{
	int 	i=0;				// Loop counter.
	string 	voronoiFileName;	// Voronoi file name.
	string 	dcelFileName;		// DCEL file name.
	Dcel	voronoiDcel;		// Original dcel data.
	Dcel	dcel;				// Dcel data.
	Voronoi voronoi;

	this->testCounter = 1;
	this->totalTests = this->filesList.getNElements() / 2;

	if ((this->filesList.getNElements() % 2) == 0)
	{
#ifdef DEBUG_TEST_VORONOI_COMPARE
		// Print test parameters.
		this->printParameters();

		Logging::buildText(__FUNCTION__, __FILE__, "Number of files to compare: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
		Logging::write(true, Info);
#endif
		// Read all files.
		for (i=0; i<this->filesList.getNElements() ;i=i+2)
		{
			// Open file.
			voronoiFileName = *this->filesList.at(i);
			dcelFileName = *this->filesList.at(i+1);
#ifdef DEBUG_TEST_VORONOI_COMPARE
			Logging::buildText(__FUNCTION__, __FILE__, "Comparing with Voronoi from ");
			Logging::buildText(__FUNCTION__, __FILE__, voronoiFileName);
			Logging::write(true, Info);
#endif
			if (!voronoiDcel.read(voronoiFileName, false))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading original file: ");
				Logging::buildText(__FUNCTION__, __FILE__, voronoiFileName);
				Logging::write(true, Error);
			}
			else if (!dcel.readPoints(dcelFileName, false))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading points file: ");
				Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
				Logging::write(true, Error);
			}
			else
			{
				// Reset dcel to remove all data except point coordinates.
				dcel.clean();

				// Execute current test.
				if (this->buildVoronoi(dcel, voronoi))
				{
					if ((*voronoi.getRefDcel()) == voronoiDcel)
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Test OK ");
						Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
						Logging::buildText(__FUNCTION__, __FILE__, "/");
						Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
						Logging::write(true, Successful);
					}
					else
					{
						this->nTestFailed++;
						Logging::buildText(__FUNCTION__, __FILE__, "Test failed when comparing dcel. Test id:");
						Logging::buildText(__FUNCTION__, __FILE__, i+1);
						Logging::write(true, Error);

						// Write test data.
						string 	fileName;
						ostringstream convert;
						convert << this->nTestFailed;
						fileName = outFolder + "Voronoi_" + convert.str() + ".txt";
						cout << fileName << endl;
						//this->dump(fileName, voronoi.getRefDcel());
					}
				}
				else
				{
					this->nTestFailed++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error building Voronoi");
					Logging::write(true, Error);

					// Write test data.
					string 	fileName;
					ostringstream convert;
					convert << this->nTestFailed;
					fileName = outFolder + "Voronoi_" + convert.str() + ".txt";
					this->dump(fileName, dcel);
				}

				// Reset Delaunay data.
				voronoiDcel.reset();
				dcel.reset();
				this->testCounter++;
			}
		}
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Number of files must be multiple of 2 and is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
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
void TestVoronoiCompare::dump(string dcelFileName, Dcel &dcel)
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
