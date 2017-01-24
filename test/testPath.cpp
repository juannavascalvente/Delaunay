/*
 * TestPath.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */


#include "Delaunay.h"
#include "Line.h"
#include "Set.h"
#include "testPath.h"

#define N_POINTS_TEST_PATH	10
#define N_TEST_PATH_TESTS	10
#define DEFAULT_QUEUE_SIZE	10

typedef Point<TYPE> PointT;

void TestPath::init()
{
}

void TestPath::main()
{
	Dcel			dcel;		// Dcel data.
	Delaunay		delaunay;	// Delaunay data.
	bool error=false;			// Bool control flag.
	int	nPoints=0;				// # points in set.
	int	nTests=0;				// # test to execute.
	int	testIndex=0;			// Test index.

	string baseFileName;
	string fileName;

	// Initialize test data.
	baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/paths/delaunayPath/findPathDelaunay";
	nTests = N_TEST_PATH_TESTS;
	nPoints = N_POINTS_TEST_PATH;
	delaunay.setDCEL(&dcel);

	while ((!error) && (testIndex < nTests))
	{
		cout << "Test " << (testIndex+1) << endl;
		testIndex++;

		// Execute current test.
		if (!dcel.generateRandom(nPoints))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
			Logging::write( true, Error);
			error = true;
		}
		else if (!delaunay.incremental())
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
			Logging::write( true, Error);
			error = true;
		}
		else
		{
			Point<TYPE> p1, p2;						// Segment points.
			Line line;								// Segment line.
			Set<PointT> points(1);					// List of points.
			Set<int> faces(DEFAULT_QUEUE_SIZE);		// Set of faces.
			ostringstream convert;

			// Generate random points.
			p1.random();
			p2.random();
			line = Line(p1, p2);

			// Dump data.
			convert << testIndex;
			fileName = baseFileName + "_" + convert.str() + "_";
			this->dump(fileName, p1, p2, dcel);

			Logging::buildText(__FUNCTION__, __FILE__, "Start find path");
			Logging::write( true, Info);

			// Compute triangles path between two points.
			if (!delaunay.findPath(line, faces))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Line does not intersect convex hull");
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Faces set computed");
			}
			Logging::write( true, Info);
			faces.write(fileName + "faces.txt");
		}
	}
}

void TestPath::dump(string fileName, Point<TYPE> &p1, Point<TYPE> &p2, Dcel &dcel)
{
	ofstream ofs;			// Output file.
	string pointsFileName;	// Points file name.
	string dcelFileName;	// DCEL file name.

	// Create file names.
	pointsFileName = fileName + "Points.txt";
	dcelFileName = fileName + "DCEL.txt";

	// Open file.
	ofs.open(pointsFileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Points loop.
		ofs << p1 << endl;
		ofs << p2 << endl;

		// Close file.
		ofs.close();

		// Write DCEL data.
		if (!dcel.writePoints(dcelFileName, dcel.getNVertex()))
		{
			cout << "Cannot open file " << dcelFileName << " to write dcel data" << endl;
		}
	}
	// Error opening points file.
	else
	{
		cout << "Cannot open file " << pointsFileName << " to write points" << endl;
	}
}

void TestPath::finish()
{

}
