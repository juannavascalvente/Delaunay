/*
 * testVoronoi.h
 *
 *  Created on: Feb 8, 2017
 *      Author: juan
 */

#ifndef TEST_TESTVORONOI_H_
#define TEST_TESTVORONOI_H_

// Default parameters in Voronoi Build test.
#define VORONOI_DEFAULT_N_POINTS		3
#define VORONOI_DEFAULT_DELTA_POINTS	2
#define VORONOI_DEFAULT_N_STEPS			5
#define VORONOI_DEFAULT_N_TESTS			5
#define DEF_VORONOI_OUT_FOLDER	"/home/juan/projects/delaunay/code/data/samples/test/output/voronoi/build/"

// Default parameters in Voronoi Compare test.
#define DEFAULT_N_FILES			20
#define DEFAULT_FILE_LIST		"fileList.txt"
#define DEF_VORONOI_COMPARE_OUT_FOLDER	"/home/juan/projects/delaunay/code/data/samples/test/output/voronoi/compare/"

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
	TestVoronoiBuild(string fileName, string outFolder, bool print) : \
			TestExecution("Voronoi Build", fileName, outFolder, print) {};
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
	TestVoronoiCompare(string fileName, string outFolder, bool print) : \
		TestCompare("Voronoi Compare", fileName, outFolder, print) {};
	~TestVoronoiCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string dcelFileName, Dcel &dcel);
	void main();
};

#endif /* TEST_TESTVORONOI_H_ */
