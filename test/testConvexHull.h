/*
 * testConvexHull.h
 *
 *  Created on: Mar 17, 2017
 *      Author: juan
 */

#ifndef TEST_TESTCONVEXHULL_H_
#define TEST_TESTCONVEXHULL_H_

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
	TestConvexHullBuild(string fileName, string outFolder, bool print) : \
			TestExecution("Convex hull Build", fileName, outFolder, print) {};
	~TestConvexHullBuild() {};

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
	TestConvexHullCompare(string fileName, string outFolder, bool print) : \
		TestCompare("Convex hull Compare", fileName, outFolder, print) {};
	~TestConvexHullCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
	void dump(string dcelFileName, Set<int> *set);
};

#endif /* TEST_TESTCONVEXHULL_H_ */
