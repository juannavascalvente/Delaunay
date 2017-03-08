/*
 * test.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#include "Parameter.h"
#include "tester.h"
#include "testDelaunay.h"
#include "testPath.h"
#include "testSet.h"
#include "testVoronoi.h"

#define DEBUG_TESTER_MAIN
//#define DEBUG_TESTER_READTESTS
//#define DEBUG_TESTER_CREATEINSTANCE

void Tester::main()
{
	int	 i=0;						// Loop counter.
	Test *currentTest;

	// Read tests.
	if (this->readTests())
	{
		// Execute all tests.
		for(i=0; i<this->tests.getNElements() ;i++)
		{
#ifdef DEBUG_TESTER_MAIN
			cout << endl << endl << endl << endl;
			cout << "**********************************************" << endl;
			cout << "Executing test " << (i+1) << endl;
			cout << "**********************************************" << endl;
#endif
			currentTest = *this->tests.at(i);
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
			test = NULL;
			break;
		}
		case TEST_DELAUNAY:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DELAUNAY" << endl;
#endif
			test = new TestDelaunayBuild("testDelaunay.txt", true);
			break;
		}
		case TEST_DELAUNAY_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DELAUNAY" << endl;
#endif
			test = new TestDelaunayCompare("testDelaunayCompare.txt", true);
			break;
		}
		case TEST_VORONOI_BUILD:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_VORONOI_BUILD" << endl;
#endif
			test = new TestVoronoiBuild("testVoronoi.txt", true);
			break;
		}
		case TEST_VORONOI_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_VORONOI_COMPARE" << endl;
#endif
			test = new TestVoronoiCompare("testVoronoiCompare.txt", true);
			break;
		}
		case TEST_PATH_DELAUNAY:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_DELAUNAY" << endl;
#endif
			test = new TestPathDelaunay("testDelaunayPath.txt", DEF_DELAUNAY_PATH_FOLDER, true);
			break;
		}
		case TEST_PATH_DELAUNAY_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_DELAUNAY_COMPARE" << endl;
#endif
			test = new TestPathDelaunayCompare("testVoronoiPath.txt", DEF_DELAUNAY_PATH_COMPARE_FOLDER, true);
			break;
		}
		case TEST_PATH_VORONOI:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_VORONOI" << endl;
#endif
			test = new TestPathVoronoi("testVoronoiPath.txt", DEF_VORONOI_PATH_FOLDER, true);
			break;
		}
		case TEST_PATH_VORONOI_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_VORONOI" << endl;
#endif
			test = new TestPathVoronoiCompare("testVoronoiPathCompare.txt", DEF_VORONOI_PATH_COMPARE_FOLDER, true);
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

	cout << "Tester: finish" << endl;
	for (i=0; i<this->tests.getNElements() ;i++)
	{
		cout << "Tester: deallocating test " << (i+1) << endl;
		test = *this->tests.at(i);
		test->finish();
	}
}
