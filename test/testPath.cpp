/*
 * TestPath.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */


#include "Line.h"
#include "Set.h"
#include "testPath.h"

#include <unistd.h>

#define DEFAULT_N_POINTS		4
#define DEFAULT_N_TESTS			10
#define DEFAULT_N_STEPS			10
#define DEFAULT_STEP			2
#define DEFAULT_QUEUE_SIZE		10

typedef Point<TYPE> PointT;

bool TestPath::prepare()
{
	bool success=true;

	cout << "PREPARE TEST PATH" << endl;

	this->delaunay.setDCEL(&this->dcel);

	// Check test type.
	switch(this->testType)
	{
		//
		case TESTPATH_DELAUNAY_COMPARE_TO_GOLD:
		{
			cout << "Test 1" << endl;
			// Input flat files
			// Gold file folder.
			break;
		}
		case TESTPATH_VORONOI_COMPARE_TO_GOLD:
		{
			cout << "Test 2" << endl;
			// Input flat files
			// Gold file folder.
			break;
		}
		case TESTPATH_DELAUNAY_RANDOM_INCREMENTAL_STEP:
		{
			// Initialize test file basename.
			this->baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/paths/delaunayPath/findPathDelaunay";
			cout << "Test 3" << endl;

			// Initialize Delaunay path test.
			this->initializeDelaunay();
			break;
		}
		case TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP:
		{
			cout << "Test 4" << endl;
			// Initialize test file basename.
			this->baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/paths/voronoiPath/findPathVoronoi";

			// Initialize Voronoi path test.
			this->initializeVoronoi();

			break;
		}
		default:
		{
			cout << "Unknown test type. Value is " << this->testType << \
					" and must be between " <<
					TESTPATH_DELAUNAY_COMPARE_TO_GOLD << " and " <<
					TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP << endl;
			success = false;
			break;
		}
	}

	return(success);
}

void TestPath::main()
{
	cout << "Test PATH MAIN" << endl;
	// Check test type.
	switch(this->testType)
	{
		//
		case TESTPATH_DELAUNAY_COMPARE_TO_GOLD:
		{
			break;
		}
		case TESTPATH_VORONOI_COMPARE_TO_GOLD:
		{
			break;
		}
		case TESTPATH_DELAUNAY_RANDOM_INCREMENTAL_STEP:
		{
			this->DelaunayPath();
			break;
		}
		case TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP:
		{
			this->VoronoiPath();
			break;
		}
		default:
		{
			cout << "Unknown test type. Value is " << this->testType << \
					" and must be between " <<
					TESTPATH_DELAUNAY_COMPARE_TO_GOLD << " and " <<
					TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP << endl;
			break;
		}
	}
}

void TestPath::DelaunayPath()
{
	bool error=false;			// Bool control flag.
	int	testIndex=0;			// Test index.
	int	failedTestIndex=1;		// Index of last failed test.

	while ((!error) && (testIndex < this->nTests))
	{
		cout << "Test " << (testIndex+1) << endl;
		testIndex++;

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
		this->fileName = this->baseFileName + "_" + convert.str() + "_";

		// Create file names.
		pointsFileName = this->fileName + "Points.txt";
		dcelFileName = this->fileName + "DCEL.txt";
		this->dump(pointsFileName, dcelFileName, p1, p2, this->dcel);

		Logging::buildText(__FUNCTION__, __FILE__, "Start find path");
		Logging::write( true, Info);

		// Compute triangles path between two points.
		if (!this->delaunay.findPath(line, faces))
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

		sleep(1);
	}
}

void TestPath::VoronoiPath()
{
	bool error=false;			// Bool control flag.
	int	testIndex=0;			// Test index.
	int	failedTestIndex=1;		// Index of last failed test.

	while ((!error) && (testIndex < this->nTests))
	{
		cout << "Test " << (testIndex+1) << endl;
		testIndex++;

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
		this->fileName = this->baseFileName + "_" + convert.str() + "_";
		pointsFileName = this->fileName + "Points.txt";
		dcelFileName = this->fileName + "DCEL.txt";
		this->dump(pointsFileName, dcelFileName, p1, p2, this->dcel);

		Logging::buildText(__FUNCTION__, __FILE__, "Start find Voronoi path");
		Logging::write( true, Info);

		if (this->delaunay.findClosestPoint(p1, this->voronoi, closest, initialFace, distance) &&
			this->delaunay.findClosestPoint(p2, this->voronoi, closest, finalFace, distance))
		{
			// Add faces to set.
			extremeFaces.add(initialFace+1);
			extremeFaces.add(finalFace+1);

			// Compute triangles path between two points.
			if (!this->voronoi.getRefDcel()->findPath(extremeFaces, line, pathFaces))
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

		// Reset voronoi data.
		this->voronoi.reset();
		sleep(1);
	}
}

bool TestPath::createSet()
{
	bool created=true;
	cout << "CREATE SET" << endl;

	// Create set of points.
	if (!this->dcel.generateRandom(this->nPoints))
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set.");
		Logging::write(true, Error);
		created = false;
	}

	return(created);
}

bool TestPath::buildDelaunay()
{
	bool built=true;

	// Create set of points.
	if (!this->delaunay.incremental())
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation.");
		Logging::write(true, Error);
		built = false;
	}

	return(built);
}

bool TestPath::buildVoronoi()
{
	bool built=true;

	if (this->voronoi.init(&this->dcel))
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error initializing Voronoi.");
		Logging::write(true, Error);
		built = false;
	}
	else if (this->voronoi.build())
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error building Voronoi.");
		Logging::write(true, Error);
		built = false;
	}

	return(built);
}

bool TestPath::initializeDelaunay()
{
	bool built=true;
	cout << "INITIALIZE DELAUNAY" << endl;

	built = !this->createSet() ||
			!this->buildDelaunay();

	return(built);
}

bool TestPath::initializeVoronoi()
{
	bool built=true;
	cout << "INITIALIZE VORONOI" << endl;

	built = !this->initializeDelaunay() ||
			!this->buildVoronoi();

	return(built);
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

bool TestPath::parseParameters(Set<Label> &labels)
{
	bool success=true;

	cout << "Test PATH READ" << endl;

	// Check test type.
	switch(this->testType)
	{
		//
		case TESTPATH_DELAUNAY_COMPARE_TO_GOLD:
		{
			// Input flat files
			// Gold file folder.
			break;
		}
		case TESTPATH_VORONOI_COMPARE_TO_GOLD:
		{
			// Input flat files
			// Gold file folder.
			break;
		}
		case TESTPATH_DELAUNAY_RANDOM_INCREMENTAL_STEP:
		{
			// Initialize test file basename.
			this->baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/paths/delaunayPath/findPathDelaunay";
			break;
		}
		case TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP:
		{
			// Initialize test file basename.
			this->baseFileName = "/home/juan/projects/delaunay/code/data/samples/errors/paths/voronoiPath/findPathVoronoi";
			break;
		}
		default:
		{
			cout << "Unknown test type. Value is " << this->testType << \
					" and must be between " <<
					TESTPATH_DELAUNAY_COMPARE_TO_GOLD << " and " <<
					TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP << endl;
			success = false;
			break;
		}
	}

	return(success);
}

void TestPath::print()
{
	cout << "Test Path parameters"<< endl;
	cout << "Test type " << this->testType << endl;

	// Check test type.
	switch(this->testType)
	{
		//
		case TESTPATH_DELAUNAY_COMPARE_TO_GOLD:
		{
			// Input flat files
			// Gold file folder.
			break;
		}
		case TESTPATH_VORONOI_COMPARE_TO_GOLD:
		{
			// Input flat files
			// Gold file folder.
			break;
		}
		case TESTPATH_DELAUNAY_RANDOM_INCREMENTAL_STEP:
		{
			cout << "# tests " << this->nTests << endl;
			cout << "# points " << this->nPoints << endl;
			cout << "# steps " << this->nSteps << endl;
			cout << "Step " << this->step << endl;
			cout << "BaseFileName" << this->baseFileName << endl;
			break;
		}
		case TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP:
		{
			cout << "# tests " << this->nTests << endl;
			cout << "# points " << this->nPoints << endl;
			cout << "# steps " << this->nSteps << endl;
			cout << "Step " << this->step << endl;
			cout << "BaseFileName" << this->baseFileName << endl;
			break;
		}
		default:
		{
			cout << "Unknown test type. Value is " << this->testType << \
					" and must be between " <<
					TESTPATH_DELAUNAY_COMPARE_TO_GOLD << " and " <<
					TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP << endl;
			break;
		}
	}
}

void TestPath::setDefault()
{
	cout << "DEFAULT" << endl;

	// Initialize test data.
	this->testType = TESTPATH_DELAUNAY_RANDOM_INCREMENTAL_STEP;
	this->nTests = DEFAULT_N_TESTS;
	this->nPoints = DEFAULT_N_POINTS;
	this->nSteps = DEFAULT_N_STEPS;
	this->step = DEFAULT_STEP;
}
