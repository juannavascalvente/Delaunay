/*
 * testConvexHull.h
 *
 *  Created on: Mar 17, 2017
 *      Author: juan
 */

#ifndef TEST_TESTCONVEXHULL_H_
#define TEST_TESTCONVEXHULL_H_

#define OUT_FOLDER_CONVEXHULL				"./data/tests/log/convexHull/"
#define DEFAULT_TEST_CONVEX_NAME			"Test Convex hull Build"

#define OUT_FOLDER_CONVEXHULL_COMP			"./data/tests/log/convexHullCompare/"
#define DEFAULT_TEST_CONVEX_NAME_COMP		"Test Convex hull Compare"

#include "testCompare.h"
#include "testExecution.h"

/****************************************************************************
// 						TestConvexHullBuild CLASS DEFITNION
****************************************************************************/
class TestConvexHullBuild : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestConvexHullBuild(bool print) : \
			TestExecution(DEFAULT_TEST_CONVEX_NAME, OUT_FOLDER_CONVEXHULL, print){};
	TestConvexHullBuild(string outFolder, bool print) : \
			TestExecution(DEFAULT_TEST_CONVEX_NAME, outFolder, print){};
	~TestConvexHullBuild(){};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string dcelFileName, Dcel &dcel);
	void main();
};

/****************************************************************************
// 						TestConvexHullCompare CLASS DEFITNION
****************************************************************************/
class TestConvexHullCompare : public TestCompare
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestConvexHullCompare(bool print) : \
		TestCompare(DEFAULT_TEST_CONVEX_NAME_COMP, OUT_FOLDER_CONVEXHULL_COMP, print) {};
	TestConvexHullCompare(string outFolder, bool print) : \
		TestCompare(DEFAULT_TEST_CONVEX_NAME_COMP, outFolder, print) {};
	~TestConvexHullCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
	void dump(string dcelFileName, Set<int> *set);
};

#endif /* TEST_TESTCONVEXHULL_H_ */
