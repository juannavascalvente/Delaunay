/*
 *
 *  Created on: Mar 4, 2017
 *      Author: juan
 */
#include "testDelaunay.h"

#include "Delaunay.h"
#include "generic.h"
#include "Label.h"
#include "Statistics.h"
#include <unistd.h>

//#define DEBUG_TEST_DELAUNAY_BUILD
//#define DEBUG_DELAUNAY_COMPARE_PREPARE

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
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	int			currentNPoints=0;
	string		dumpFileName;		// DCEL file name.
	StatisticsDelaunayRegister *statReg =  							 		\
						new StatisticsDelaunayRegister(this->statFileName,  \
						this->nSteps, this->nTests);
	this->stat = statReg;

#ifdef DEBUG_TEST_DELAUNAY_BUILD
	// Print test parameters.
	this->printParameters();
#endif

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
			StatisticsDelaunayData *statData = new StatisticsDelaunayData();
			statData->setPoints(currentNPoints);
			statData->setNodesChecked(new int[currentNPoints]);

			// Build failed file name test data.
			ostringstream convert;
			convert << this->nTestFailed+1;
			dumpFileName = this->outFolder + "Delaunay_" + convert.str() + ".txt";

			// Build incremental Delaunay triangulation.
			if (this->buildRandomDelaunay(currentNPoints, dcel, delaunay))
			{
				statData->setExecTime(this->stat->getLapse());
				Logging::buildText(__FUNCTION__, __FILE__, "Test OK ");
				Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
				Logging::write(true, Successful);
			}
			else
			{
				this->dump(dumpFileName, dcel);
				Logging::buildText(__FUNCTION__, __FILE__, "Test FALIED ");
				Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
				Logging::write(true, Error);
				this->nTestFailed++;
			}

			// Add statistics data.
			statData->analyzeDelaunay(delaunay);
			statReg->add(statData);

#ifdef INCREMENTAL_DELAUNAY_STATISTICS
			memcpy(statData->getNodesChecked(), delaunay.getNodesChecked(), sizeof(int)*currentNPoints);
			delaunay.freeStatistics();
#endif
			// Reset Delaunay data.
			delaunay.reset();
			this->testCounter++;
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
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
	int 	 i=0;				// Loop counter.
	string 	 dcelFileName;		// DCEL file name.
	string 	 outFileName;		// Output file name.
	Dcel	 originalDcel;		// Original dcel data.
	Dcel	 dcel;				// Dcel data.
	Delaunay delaunay;			// Delaunay data.

	this->testCounter = 1;
	this->totalTests = filesList.getNElements();

#ifdef DEBUG_DELAUNAY_COMPARE_PREPARE
	// Print test parameters.
	this->printParameters();
	Logging::buildText(__FUNCTION__, __FILE__, "Number of files to compare: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
	Logging::write(true, Info);
#endif
	// Read all files.
	for (i=0; i<this->filesList.getNElements() ;i++)
	{
		// Build file name where data is written if test fails.
		ostringstream convert;
		convert << this->nTestFailed;
		outFileName = this->outFolder + "Delaunay_" + convert.str() + ".txt";

		// Open file.
		dcelFileName = *this->filesList.at(i);
		if (!originalDcel.read(dcelFileName, false))
		{
			this->nTestFailed++;
			Logging::buildText(__FUNCTION__, __FILE__, "Error reading original file: ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::buildText(__FUNCTION__, __FILE__, " in test ");
			Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
			Logging::buildText(__FUNCTION__, __FILE__, "/");
			Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
			Logging::write(true, Error);
		}
		else if (this->readDelaunay(dcelFileName, dcel, delaunay))
		{
			if (dcel == originalDcel)
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

				// Write test data.
				this->dump(outFileName, dcel);

				// Print log error.
				Logging::buildText(__FUNCTION__, __FILE__, "Test failed when comparing dcel. Test id:");
				Logging::buildText(__FUNCTION__, __FILE__, this->testCounter);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
				Logging::write(true, Error);
			}
		}
		else
		{
			this->dump(outFileName, dcel);
		}

		// Reset test data.
		this->testCounter++;
		delaunay.reset();
		originalDcel.reset();
		dcel.reset();
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

