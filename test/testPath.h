/*
 * testPath.h
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */

#ifndef TEST_TESTPATH_H_
#define TEST_TESTPATH_H_

#include "Dcel.h"
#include "Delaunay.h"
#include "Point.h"
#include "testExecution.h"

#define DEF_DELAUNAY_PATH_FOLDER	"/home/juan/projects/delaunay/code/data/samples/test/output/delaunay/path/"
#define DEF_VORONOI_PATH_FOLDER		"/home/juan/projects/delaunay/code/data/samples/test/output/voronoi/path/"

/****************************************************************************
// 							TestPath CLASS DEFITNION
****************************************************************************/
class TestPathDelaunay : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestPathDelaunay(string fileName, string outFolder, bool print) : \
							TestExecution(fileName, outFolder, print) {};
	~TestPathDelaunay() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string pointsFileName, string dcelFileName, Point<TYPE> &p1, \
												Point<TYPE> &p2, Dcel &dcel);
	void main();
};

/****************************************************************************
// 							TestPath CLASS DEFITNION
****************************************************************************/
class TestPathVoronoi : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestPathVoronoi(string fileName, string outFolder, bool print) : \
							TestExecution(fileName, outFolder, print) {};
	~TestPathVoronoi() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string pointsFileName, string dcelFileName, Point<TYPE> &p1, \
												Point<TYPE> &p2, Dcel &dcel);
	void main();
};

#endif /* TEST_TESTPATH_H_ */

