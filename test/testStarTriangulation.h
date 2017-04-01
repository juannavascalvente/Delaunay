/*
 * testStarTriangulation.h
 *
 *  Created on: Apr 1, 2017
 *      Author: juan
 */

#ifndef TEST_TESTSTARTRIANGULATION_H_
#define TEST_TESTSTARTRIANGULATION_H_

#define OUT_FOLDER_STAR				 "./data/tests/log/star/"
#define DEFAULT_TEST_STAR_NAME		 "Test Star Triangulation Build"

#define OUT_FOLDER_STAR_COMP		 "./data/tests/log/starCompare/"
#define DEFAULT_TEST_STAR_NAME_COMP	 "Test Star Triangulation Compare"

// Default parameters in Delaunay Compare test.
#define DEFAULT_N_FILES			20
#define DEFAULT_FILE_LIST		"fileList.txt"

#include "Dcel.h"
#include "Parameter.h"
#include "testCompare.h"
#include "testExecution.h"

/****************************************************************************
// 						TestDelaunayBuild CLASS DEFITNION
****************************************************************************/
class TestStarTriangulationBuild : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestStarTriangulationBuild(bool print) : \
		TestExecution(DEFAULT_TEST_STAR_NAME, OUT_FOLDER_STAR, print) {};
	TestStarTriangulationBuild(string outFolder, bool print) : \
			TestExecution(DEFAULT_TEST_STAR_NAME, outFolder, print) {};
	~TestStarTriangulationBuild() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string dcelFileName, Dcel &dcel);
	void main();
};

/****************************************************************************
// 						TestStarTriangulationCompare CLASS DEFITNION
****************************************************************************/
class TestStarTriangulationCompare : public TestCompare
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestStarTriangulationCompare(bool print) : \
		TestCompare(DEFAULT_TEST_STAR_NAME_COMP,OUT_FOLDER_STAR_COMP,print){};
	TestStarTriangulationCompare(string outFolder, bool print) : \
		TestCompare(DEFAULT_TEST_STAR_NAME_COMP, outFolder, print) {};
	~TestStarTriangulationCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
	void dump(string dcelFileName, Dcel &dcel);
};

#endif /* TEST_TESTSTARTRIANGULATION_H_ */
