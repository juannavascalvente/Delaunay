/*
 * test.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#include "Parameter.h"
#include "tester.h"
#include "testDcel.h"
#include "testDelaunay.h"
#include "testPath.h"
#include "testSet.h"
#include "testVoronoi.h"

#define OUT_FOLDER_DCEL					"./data/tests/log/dcelTest/"
#define OUT_FOLDER_SET					"./data/tests/log/setTest/"
#define OUT_FOLDER_DELAUNAY				"./data/tests/log/delaunay/"
#define OUT_FOLDER_DELAUNAY_COMP		"./data/tests/log/delaunayCompare/"
#define OUT_FOLDER_VORONOI				"./data/tests/log/voronoi/"
#define OUT_FOLDER_VORONOI_COMP			"./data/tests/log/voronoiCompare/"
#define OUT_FOLDER_DELAUNAY_PATH		"./data/tests/log/delaunayPath/"
#define OUT_FOLDER_DELAUNAY_PATH_COMP 	"./data/tests/log/delaunayPathCompare/"
#define OUT_FOLDER_VORONOI_PATH			"./data/tests/log/voronoiPath/"
#define OUT_FOLDER_VORONOI_PATH_COMP	"./data/tests/log/voronoiPathCompare/"

//#define DEBUG_TESTER_READTESTS
//#define DEBUG_TESTER_CREATEINSTANCE

void Tester::main()
{
	int	 i=0;						// Loop counter.
	Test *currentTest;				// Pointer to current test.
	int	nTests=0;					// # tests to execute.

	// Read tests.
	if (this->readTests())
	{
		// Execute all tests.
		nTests = this->tests.getNElements();
		for(i=0; i<nTests ;i++)
		{
			currentTest = *this->tests.at(i);
			cout << endl << endl << endl << endl;
			cout << "**********************************************" << endl;
			cout << "Executing test " << (i+1) << "/" << nTests << endl;
			cout << "Test name: " <<  currentTest->getTestName() << endl;
			cout << "**********************************************" << endl;
			currentTest->run();
		}

		// Deallocate resources.
		this->finish();
	}
}

bool Tester::readTests()
{
	bool testsToExecute=false;	// Return value.
	TestType type;				// Test type label.
	Test *test=NULL;			// Test data.
	int	 testId=0;				// Test identifier.

	// Open file.
	this->ifs.open(this->fileName.c_str(), ios::in);
	if (!this->ifs.is_open())
	{
		cout << "Error opening input test file " << this->fileName << endl;
		testsToExecute = false;
	}
	else
	{
#ifdef DEBUG_TESTER_READTESTS
		cout << "Start reading test " << endl;
#endif
		while (!Test::read(this->ifs, labels, type))
		{
#ifdef DEBUG_TESTER_READTESTS
			cout << "Test type is " << type << endl;
#endif
			// Initialize test.
			if (type != UNKNOWN_TEST)
			{
#ifdef DEBUG_TESTER_READTESTS
				cout << "Initializing test id: " << (testId+1) << endl;
#endif
				test = this->createTestInstance(type);
				test->init(labels);
				this->tests.add(test);
				testsToExecute = true;
			}
			else
			{
				cout << "Error reading test id: " << (testId+1) << endl;
			}

			// Update test counter.
			testId++;
			labels.reset();
		}
#ifdef DEBUG_TESTER_READTESTS
		cout << "End reading test " << endl;
#endif
		// Close input file.
		this->ifs.close();
	}

	return(testsToExecute);
}

Test* Tester::createTestInstance(TestType type)
{
	Test *test;			// Return value.
	switch(type)
	{
		case TEST_SET:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_SET" << endl;
#endif
			test = new TestSet("testSet.txt", OUT_FOLDER_SET, true);
			break;
		}
		case TEST_DCEL:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DCEL" << endl;
#endif
			test = new TestDcel("testDcel.txt", OUT_FOLDER_DCEL, true);
			break;
		}
		case TEST_DELAUNAY:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DELAUNAY" << endl;
#endif
			test = new TestDelaunayBuild("testDelaunay.txt", OUT_FOLDER_DELAUNAY, true);
			break;
		}
		case TEST_DELAUNAY_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DELAUNAY" << endl;
#endif
			test = new TestDelaunayCompare("testDelaunayCompare.txt", OUT_FOLDER_DELAUNAY_COMP, true);
			break;
		}
		case TEST_VORONOI_BUILD:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_VORONOI_BUILD" << endl;
#endif
			test = new TestVoronoiBuild("testVoronoi.txt", OUT_FOLDER_VORONOI, true);
			break;
		}
		case TEST_VORONOI_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_VORONOI_COMPARE" << endl;
#endif
			test = new TestVoronoiCompare("testVoronoiCompare.txt", OUT_FOLDER_VORONOI_COMP, true);
			break;
		}
		case TEST_PATH_DELAUNAY:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_DELAUNAY" << endl;
#endif
			test = new TestPathDelaunay("testDelaunayPath.txt", OUT_FOLDER_DELAUNAY_PATH, true);
			break;
		}
		case TEST_PATH_DELAUNAY_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_DELAUNAY_COMPARE" << endl;
#endif
			test = new TestPathDelaunayCompare("testVoronoiPath.txt", OUT_FOLDER_DELAUNAY_PATH_COMP, true);
			break;
		}
		case TEST_PATH_VORONOI:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_VORONOI" << endl;
#endif
			test = new TestPathVoronoi("testVoronoiPath.txt", OUT_FOLDER_VORONOI_PATH, true);
			break;
		}
		case TEST_PATH_VORONOI_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_VORONOI" << endl;
#endif
			test = new TestPathVoronoiCompare("testVoronoiPathCompare.txt", OUT_FOLDER_VORONOI_PATH_COMP, true);
			break;
		}
		default:
		{
			break;
		}
	}

	return(test);
}

void Tester::finish()
{
	int i=0;			// Loop counter.
	Test *test;

	for (i=0; i<this->tests.getNElements() ;i++)
	{
		test = *this->tests.at(i);
		test->finish();
		delete test;
	}
}
