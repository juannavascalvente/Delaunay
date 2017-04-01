/*
 * testStarTriangulation.cpp
 *
 *  Created on: Apr 1, 2017
 *      Author: juan
 */

#include "testStarTriangulation.h"

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	builds several star triangulations using different set of
* 				points and if any fails the dcel data is written to a file.
***************************************************************************/
void TestStarTriangulationBuild::main()
{
	Dcel		dcel;				// Dcel data.
	Triangulation	triangulation	// Triangulation data.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	int			currentNPoints=0;
	string		dumpFileName;		// DCEL file name.
	StatisticsStarTriangulationRegister *statReg =  						\
				new StatisticsStarTriangulationRegister(this->statFileName, \
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
			StatisticsTriangulationData *statData = new StatisticsTriangulationData();
			statData->setPoints(currentNPoints);

			// Build failed file name test data.
			ostringstream convert;
			convert << this->nTestFailed+1;
			dumpFileName = this->outFolder + "Delaunay_" + convert.str() + ".txt";

			// Build incremental Delaunay triangulation.
			if (this->buildRandomStarTriangulation(currentNPoints, dcel, triangulation))
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
			}

			// Add statistics data.
			//statData->analyzeDelaunay(delaunay);
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
void TestStarTriangulationBuild::dump(string dcelFileName, Dcel &dcel)
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
