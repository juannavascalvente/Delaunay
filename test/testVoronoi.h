/*
 * testVoronoi.h
 *
 *  Created on: Feb 8, 2017
 *      Author: juan
 */

#ifndef TEST_TESTVORONOI_H_
#define TEST_TESTVORONOI_H_

#define OUT_FOLDER_VORONOI					 "./data/tests/log/voronoi/"
#define DEFAULT_TEST_VORONOI_NAME			 "Test Voronoi Build"

#define OUT_FOLDER_VORONOI_COMP				 "./data/tests/log/voronoiCompare/"
#define DEFAULT_TEST_VORONOI_NAME_COMP		 "Test Voronoi Compare"

// Default parameters in Voronoi Build test.
#define VORONOI_DEFAULT_N_POINTS		3
#define VORONOI_DEFAULT_DELTA_POINTS	2
#define VORONOI_DEFAULT_N_STEPS			5
#define VORONOI_DEFAULT_N_TESTS			5

// Default parameters in Voronoi Compare test.
#define DEFAULT_N_FILES			20
#define DEFAULT_FILE_LIST		"fileList.txt"

#include "testCompare.h"
#include "testExecution.h"
using namespace std;

/****************************************************************************
// 						TestDelaunayBuild CLASS DEFITNION
****************************************************************************/
class TestVoronoiBuild : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestVoronoiBuild(bool print) : \
			TestExecution(DEFAULT_TEST_VORONOI_NAME, OUT_FOLDER_VORONOI, print) {};
	TestVoronoiBuild(string outFolder, bool print) : \
			TestExecution(DEFAULT_TEST_VORONOI_NAME, outFolder, print) {};
	~TestVoronoiBuild() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string dcelFileName, Dcel &dcel);
	void main();
};

/****************************************************************************
// 						TestDelaunayCompare CLASS DEFITNION
****************************************************************************/
class TestVoronoiCompare : public TestCompare
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestVoronoiCompare(bool print) : \
		TestCompare(DEFAULT_TEST_VORONOI_NAME_COMP, OUT_FOLDER_VORONOI_COMP, print) {};
	TestVoronoiCompare(string outFolder, bool print) : \
		TestCompare(DEFAULT_TEST_VORONOI_NAME_COMP, outFolder, print) {};
	~TestVoronoiCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string dcelFileName, Dcel &dcel);
	void main();
};

#endif /* TEST_TESTVORONOI_H_ */
