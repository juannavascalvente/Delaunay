/*
 * testConvexHull.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: juan
 */

//#define DEBUG_TEST_CONEXHULL_BUILD
//#define DEBUG_TEST_CONEXHULL_COMPARE

#include "Dcel.h"
#include "Delaunay.h"
#include "Logging.h"
#include "testConvexHull.h"
#include <unistd.h>

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
void TestConvexHullBuild::main()
{
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	int			currentNPoints=0;	// Current # points in DCEL.
	string 		dumpFileName;		// Input dcel file name.
	StatisticsConvexHullRegister statReg("statisticsConvexHull.txt", "statConvexLog.txt");

#ifdef DEBUG_TEST_CONEXHULL_BUILD
	// Print test parameters.
	this->printParameters();
#endif

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
#ifdef DEBUG_TEST_CONEXHULL_BUILD
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);
#endif
		for (testIndex=0; testIndex<this->nTests ;testIndex++)
		{
			ConexHullStatisticsData *statData = new ConexHullStatisticsData();
			statData->setPoints(currentNPoints);

			// Build failed file name test data.
			ostringstream convert;
			convert << this->nTestFailed+1;
			dumpFileName = this->outFolder + "Delaunay_" + convert.str() + ".txt";

			// Build incremental Delaunay triangulation.
			if (this->buildRandomDelaunay(currentNPoints, dcel, delaunay))
			{
				statReg.tic();
				if (delaunay.convexHull())
				{
					statReg.toc();
					Logging::buildText(__FUNCTION__, __FILE__, "Test OK ");
					Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
					Logging::write(true, Successful);
					statData->setLength(delaunay.getConvexHullEdges()->getNElements());
					statData->setExecTime(statReg.getLapse());
				}
				else
				{
					this->dump(dumpFileName, dcel);
					this->nTestFailed++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error computing convex hull ");
					Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
					Logging::buildText(__FUNCTION__, __FILE__, "/");
					Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
					Logging::write(true, Error);
				}

				// Reset Delaunay data.
				delaunay.reset();
				this->testCounter++;
			}
			else
			{
				this->dump(dumpFileName, dcel);
			}

			// Add statistics data.
			statReg.add(statData);
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}

	// Write statistics data.
	statReg.writeResults();
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
void TestConvexHullBuild::dump(string dcelFileName, Dcel &dcel)
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
void TestConvexHullCompare::main()
{
	int 	i=0;				// Loop counter.
	string 	convexHullFileName;	// Voronoi file name.
	string 	dcelFileName;		// DCEL file name.
	Set<int> originalConvexHull;// Original convex hull.
	Set<int> *computedConvexHull;// Original convex hull.
	Dcel	dcel;				// Dcel data.
	Delaunay	delaunay;		// Delaunay data.

	this->testCounter = 1;
	this->totalTests = this->filesList.getNElements() / 2;

	if ((this->filesList.getNElements() % 2) == 0)
	{
#ifdef DEBUG_TEST_CONEXHULL_COMPARE
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
			dcelFileName = *this->filesList.at(i);
			convexHullFileName = *this->filesList.at(i+1);
#ifdef DEBUG_TEST_CONEXHULL_COMPARE
			Logging::buildText(__FUNCTION__, __FILE__, "Reading DCEL file ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, convexHullFileName);
			Logging::write(true, Info);
#endif
			if (!dcel.readPoints(dcelFileName, false))
			{
				this->nTestFailed++;
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading original file: ");
				Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
				Logging::write(true, Error);
			}
			else if (!originalConvexHull.read(convexHullFileName))
			{
				this->nTestFailed++;
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading points file: ");
				Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
				Logging::write(true, Error);
			}
			else
			{
				// Reset dcel to remove all data except point coordinates.
				dcel.clean();

				// Execute current test.
				delaunay.setDCEL(&dcel);
				if (delaunay.incremental())
				{
					if (delaunay.convexHull())
					{
						computedConvexHull = delaunay.getConvexHullEdges();
						if (originalConvexHull == (*computedConvexHull))
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
							Logging::buildText(__FUNCTION__, __FILE__, "Error comparing convex hull ");
							Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
							Logging::buildText(__FUNCTION__, __FILE__, "/");
							Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
							Logging::write(true, Error);

							// Write test data.
							string 	fileName;
							ostringstream convert;
							convert << this->nTestFailed;
							fileName = outFolder + "ConvexHull" + convert.str() + ".txt";
							cout << fileName << endl;
							this->dump(fileName, computedConvexHull);
						}
					}
					else
					{
						this->nTestFailed++;
						Logging::buildText(__FUNCTION__, __FILE__, "Error computing convex hull ");
						Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
						Logging::buildText(__FUNCTION__, __FILE__, "/");
						Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
						Logging::write(true, Error);
					}
				}
				else
				{
					this->nTestFailed++;
					Logging::buildText(__FUNCTION__, __FILE__, "Failed test id: ");
					Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
					Logging::write(true, Error);
				}

				// Reset data.
				this->testCounter++;
				delaunay.reset();
				dcel.reset();
				originalConvexHull.reset();
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
void TestConvexHullCompare::dump(string fileName, Set<int> *set)
{
	ofstream ofs;			// Output file.

	// Open file.
	ofs.open(fileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Write DCEL data.
		if (!set->write(fileName))
		{
			cout << "Cannot open file " << fileName << " to write set data" << endl;
		}
	}
	// Error opening points file.
	else
	{
		cout << "Cannot open file " << fileName << " to write points" << endl;
	}
}

