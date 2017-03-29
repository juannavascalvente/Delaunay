/*
 *
 *  Created on: Mar 4, 2017
 *      Author: juan
 */

#ifndef TEST_TESTDELAUNAY_H_
#define TEST_TESTDELAUNAY_H_

#define OUT_FOLDER_DELAUNAY					 "./data/tests/log/delaunay/"
#define DEFAULT_TEST_DELAUNAY_NAME			 "Test Delaunay Build"

#define OUT_FOLDER_DELAUNAY_COMP			 "./data/tests/log/delaunayCompare/"
#define DEFAULT_TEST_DELAUNAY_NAME_COMP		 "Test Delaunay Compare"

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
class TestDelaunayBuild : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestDelaunayBuild(bool print) : \
			TestExecution(DEFAULT_TEST_DELAUNAY_NAME, OUT_FOLDER_DELAUNAY, print) {};
	TestDelaunayBuild(string outFolder, bool print) : \
			TestExecution(DEFAULT_TEST_DELAUNAY_NAME, outFolder, print) {};
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
	TestDelaunayCompare(bool print) : \
		TestCompare(DEFAULT_TEST_DELAUNAY_NAME_COMP, OUT_FOLDER_DELAUNAY_COMP, print) {};
	TestDelaunayCompare(string outFolder, bool print) : \
		TestCompare(DEFAULT_TEST_DELAUNAY_NAME_COMP, outFolder, print) {};
	~TestDelaunayCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
	void dump(string dcelFileName, Dcel &dcel);
};

#endif /* TEST_TESTDELAUNAY_H_ */
