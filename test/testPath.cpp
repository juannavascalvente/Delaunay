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

#include <unistd.h>

#define N_POINTS_TEST_PATH	4
#define N_TEST_PATH_TESTS	600
#define DEFAULT_QUEUE_SIZE	10

typedef Point<TYPE> PointT;

void TestPath::init()
{
}

void TestPath::main()
{
	this->VoronoiPath();
}

void TestPath::DelaunayPath()
{
	Dcel			dcel;		// Dcel data.
	Delaunay		delaunay;	// Delaunay data.
	bool error=false;			// Bool control flag.
	int	nPoints=0;				// # points in set.
	int	nTests=0;				// # test to execute.
	int	testIndex=0;			// Test index.
	int	failedTestIndex=1;		// Index of last failed test.

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
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write( true, Error);
			error = true;
		}
		else if (!delaunay.incremental())
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
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
			string pointsFileName;	// Points file name.
			string dcelFileName;	// DCEL file name.

			// Generate random points.
			p1.random();
			p2.random();
			line = Line(p1, p2);

			// Dump data.
			convert << failedTestIndex;
			fileName = baseFileName + "_" + convert.str() + "_";

			// Create file names.
			pointsFileName = fileName + "Points.txt";
			dcelFileName = fileName + "DCEL.txt";
			this->dump(pointsFileName, dcelFileName, p1, p2, dcel);

			Logging::buildText(__FUNCTION__, __FILE__, "Start find path");
			Logging::write( true, Info);

			// Compute triangles path between two points.
			if (!delaunay.findPath(line, faces))
			{
				failedTestIndex++;
				Logging::buildText(__FUNCTION__, __FILE__, "Line does not intersect convex hull");
			}
			else
			{
				remove(pointsFileName.c_str());
				remove(dcelFileName.c_str());
				Logging::buildText(__FUNCTION__, __FILE__, "Faces set computed");
			}
			Logging::write( true, Info);
		}
		sleep(1);
	}
}

void TestPath::VoronoiPath()
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
	baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/paths/voronoiPath/findPathVoronoi";
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
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write(true, Error);
			error = true;
		}
		else if (!delaunay.incremental())
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write(true, Error);
			error = true;
		}
		else if (voronoi.init(&dcel))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error initializing Voronoi in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write(true, Error);
			error = true;
		}
		else if (voronoi.build())
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error building Voronoi in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, testIndex);
			Logging::write(true, Error);
			error = true;
		}
		else
		{
			Point<TYPE> p1, p2;						// Segment points.
			Line line;								// Segment line.
			Set<PointT> points(1);					// List of points.
			Set<int> extremeFaces(DEFAULT_QUEUE_SIZE);		// Set of faces.
			Set<int> pathFaces;
			ostringstream convert;

			Point<TYPE> closest;		// Closest point.
			double distance=0.0;		// Distance between points.

			int	 initialFace=0;			// Initial face in the path.
			int	 finalFace=0;			// Final face in the path.

			string pointsFileName;	// Points file name.
			string dcelFileName;	// DCEL file name.

			// Generate random points.
			p1.random();
			p2.random();
			line = Line(p1, p2);

			// Dump data.
			convert << failedTestIndex;
			fileName = baseFileName + "_" + convert.str() + "_";
			pointsFileName = fileName + "Points.txt";
			dcelFileName = fileName + "DCEL.txt";
			this->dump(pointsFileName, dcelFileName, p1, p2, dcel);

			Logging::buildText(__FUNCTION__, __FILE__, "Start find Voronoi path");
			Logging::write( true, Info);

			if (delaunay.findClosestPoint(p1, voronoi, closest, initialFace, distance) &&
				delaunay.findClosestPoint(p2, voronoi, closest, finalFace, distance))
			{
				// Add faces to set.
				extremeFaces.add(initialFace+1);
				extremeFaces.add(finalFace+1);

				// Compute triangles path between two points.
				if (!voronoi.getRefDcel()->findPath(extremeFaces, line, pathFaces))
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error computing Voronoi path in iteration ");
				}
				else
				{
					remove(pointsFileName.c_str());
					remove(dcelFileName.c_str());
					Logging::buildText(__FUNCTION__, __FILE__, "Voronoi path computed in iteration ");
				}
				Logging::buildText(__FUNCTION__, __FILE__, testIndex);
				Logging::write( true, Info);
			}
		}

		// Reset voronoi data.
		voronoi.reset();
	}
}

void TestPath::dump(string pointsFileName, string dcelFileName, Point<TYPE> &p1, Point<TYPE> &p2, Dcel &dcel)
{
	ofstream ofs;			// Output file.

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
