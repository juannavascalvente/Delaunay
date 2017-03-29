/*
 * test.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#include "Parameter.h"
#include "tester.h"
#include "testConvexHull.h"
#include "testDcel.h"
#include "testDelaunay.h"
#include "testPath.h"
#include "testSet.h"
#include "testVoronoi.h"

//#define DEBUG_TESTER_READTESTS
//#define DEBUG_TESTER_CREATEINSTANCE

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	read all tests from file and executes them.
***************************************************************************/
void Tester::main()
{
	int	 i=0;						// Loop counter.
	Test *currentTest;				// Pointer to current test.
	int	nTests=0;					// # tests to execute.
	int	totalTests=0;
	int	totalTestsFailed=0;

	// Read tests.
	if (this->readTests())
	{
		// Execute all tests.
		nTests = this->tests.getNElements();
		for(i=0; i<nTests ;i++)
		{
			currentTest = *this->tests.at(i);

			// Print test header.
			this->log.buildTestHeader((i+1), nTests, currentTest->getTestName());

			// Execute test.
			currentTest->run();

			// Update # tests.
			totalTests = totalTests + currentTest->getTotalTests();
			totalTestsFailed = totalTestsFailed + currentTest->getTestFailed();
		}

		ostringstream convertnTestsSuccess;
		convertnTestsSuccess << totalTests - totalTestsFailed;
		ostringstream convertnTests;
		convertnTests << totalTests;

		// Print tests execution resume.
		Logging::buildText("**********************************************\n");
		Logging::buildText("Tests resume ");
		Logging::buildText(convertnTestsSuccess.str());
		Logging::buildText("/");
		Logging::buildText(convertnTests.str());
		Logging::buildText("\n**********************************************");
		if (totalTestsFailed == 0)
		{
			Logging::write(true, Testing);
		}
		else
		{
			Logging::write(true, Error);
		}

		// Deallocate resources.
		this->finish();
	}
}

/***************************************************************************
* Name: 	readTests
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	read all tests from file and creates tests instances.
***************************************************************************/
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
				// Create test instance.
				test = this->createTestInstance(type);

				// Create apply parameters values.
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

/***************************************************************************
* Name: 	createTestInstance
* IN:		type			test type to create.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	create a test depending on "type" value.
***************************************************************************/
Test* Tester::createTestInstance(TestType type)
{
	Test *test=NULL;			// Return value.
	switch(type)
	{
		case TEST_SET:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_SET" << endl;
#endif
			test = new TestSet(true);
			break;
		}
		case TEST_DCEL:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DCEL" << endl;
#endif
			test = new TestDcel(true);

			break;
		}
		case TEST_DELAUNAY:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DELAUNAY_BUILD" << endl;
#endif
			test = new TestDelaunayBuild(true);
			break;
		}
		case TEST_DELAUNAY_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_DELAUNAY_COMPARE" << endl;
#endif
			test = new TestDelaunayCompare(true);
			break;
		}
		case TEST_CONVEXHULL:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_CONVEX_HULL_BUILD" << endl;
#endif
			test = new TestConvexHullBuild(true);
			break;
		}
		case TEST_CONVEXHULL_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_CONVEX_HULL_COMPARE" << endl;
#endif
			test = new TestConvexHullCompare(true);
			break;
		}
		case TEST_VORONOI_BUILD:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_VORONOI_BUILD" << endl;
#endif
			test = new TestVoronoiBuild(true);
			break;
		}
		case TEST_VORONOI_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_VORONOI_COMPARE" << endl;
#endif
			test = new TestVoronoiCompare(true);
			break;
		}
		case TEST_PATH_DELAUNAY:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_DELAUNAY" << endl;
#endif
			test = new TestPathDelaunay(true);
			break;
		}
		case TEST_PATH_DELAUNAY_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_DELAUNAY_COMPARE" << endl;
#endif
			test = new TestPathDelaunayCompare(true);
			break;
		}
		case TEST_PATH_VORONOI:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_VORONOI" << endl;
#endif
			test = new TestPathVoronoi(true);
			break;
		}
		case TEST_PATH_VORONOI_COMPARE:
		{
#ifdef DEBUG_TESTER_CREATEINSTANCE
			cout << "Creating TEST_PATH_VORONOI" << endl;
#endif
			test = new TestPathVoronoiCompare(true);
			break;
		}
		default:
		{
			break;
		}
	}

	return(test);
}

/***************************************************************************
* Name: 	finish
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	deallocates tests.
***************************************************************************/
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
