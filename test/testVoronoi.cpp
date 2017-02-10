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

#define N_POINTS_TEST_VORONOI	4
#define N_TEST_VORONOI			600


void TestVoronoi::init()
{
}

void TestVoronoi::main()
{
	Dcel			dcel;		// Dcel data.
	Delaunay		delaunay;	// Delaunay data.
	Voronoi			voronoi;	// Voronoi data.
	bool error=false;			// Bool control flag.
	int	nPoints=0;				// # points in set.
	int	nTests=0;				// # test to execute.
	int	testIndex=0;			// Test index.
	int	failedTestIndex=1;		// Index of last failed test.

	string baseFileName;
	string fileName;

	// Initialize test data.
	baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/voronoi/voronoi";
	nTests = N_TEST_VORONOI;
	nPoints = N_POINTS_TEST_VORONOI;
	delaunay.setDCEL(&dcel);

	while ((!error) && (testIndex < nTests))
	{
		cout << "Test " << (testIndex+1) << endl;
		testIndex++;

		// Execute current test.
		if (!dcel.generateRandom(nPoints))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, (testIndex));
			Logging::write(true, Error);
			error = true;
		}
		else if (!delaunay.incremental())
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, (testIndex));
			Logging::write(true, Error);
			error = true;
		}
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Start building voronoi diagram test ");
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write(true, Info);

			// Write test data.
			ostringstream convert;
			convert << failedTestIndex;
			fileName = baseFileName + "_" + convert.str() + ".txt";
			this->dump(fileName, dcel);

			error = voronoi.init(&dcel);
			if (!error)
			{
				// Compute Voronoi diagram.
				error = voronoi.build();
				if (!error)
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
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write(true, Info);
			sleep(1);

			// Reset voronoi data.
			voronoi.reset();
		}
	}
}

void TestVoronoi::dump(string fileName, Dcel &dcel)
{
	ofstream ofs;			// Output file.

	// Write DCEL data.
	if (!dcel.writePoints(fileName, dcel.getNVertex()))
	{
		cout << "Cannot open file " << fileName << " to write dcel data" << endl;
	}
}

void TestVoronoi::finish()
{
	Logging::buildText(__FUNCTION__, __FILE__, "Finished VORONOI diagram test");
	Logging::write(true, Info);
}
