/*
 *
 *  Created on: Mar 4, 2017
 *      Author: juan
 */

#ifndef TEST_TESTDELAUNAY_H_
#define TEST_TESTDELAUNAY_H_

// Default parameters in Delaunay Build test.
#define DEFAULT_N_POINTS		3
#define DEFAULT_DELTA_POINTS	2
#define DEFAULT_N_STEPS			5
#define DEFAULT_N_TESTS			5
#define DEF_OUT_FOLDER_DELAUNAY	"/home/juan/projects/delaunay/code/data/samples/test/output/delaunay/build/"

// Default parameters in Delaunay Compare test.
#define DEFAULT_N_FILES			20
#define DEFAULT_FILE_LIST		"fileList.txt"
#define DEF_DELAUNAY_COMPARE_FOLDER	"/home/juan/projects/delaunay/code/data/samples/test/output/delaunay/compare/"

#include "Dcel.h"
#include "Parameter.h"
#include "testCompare.h"
#include "testExecution.h"

/****************************************************************************
// 						TestDelaunayBuild CLASS DEFITNION
****************************************************************************/
class TestDelaunayBuild : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestDelaunayBuild(string fileName, string outFolder, bool print) : \
			TestExecution("Delaunay Build", fileName, outFolder, print) {};
	~TestDelaunayBuild() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string dcelFileName, Dcel &dcel);
	void main();
};

/****************************************************************************
// 						TestDelaunayCompare CLASS DEFITNION
****************************************************************************/
class TestDelaunayCompare : public TestCompare
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestDelaunayCompare(string fileName, string outFolder, bool print) : \
		TestCompare("Delaunay Compare", fileName, outFolder, print) {};
	~TestDelaunayCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
	void dump(string dcelFileName, Dcel &dcel);
};

#endif /* TEST_TESTDELAUNAY_H_ */
