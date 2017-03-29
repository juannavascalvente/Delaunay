/*
 * testPath.h
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */

#ifndef TEST_TESTPATH_H_
#define TEST_TESTPATH_H_

#define OUT_FOLDER_DELAUNAY_PATH					"./data/tests/log/delaunayPath/"
#define DEFAULT_TEST_DELAUNAY_PATH_NAME			 	"Test Delaunay Path"

#define OUT_FOLDER_DELAUNAY_PATH_COMP 				"./data/tests/log/delaunayPathCompare/"
#define DEFAULT_TEST_DELAUNAY_PATH_COMP_NAME		"Test Delaunay Path Compare"

#define OUT_FOLDER_VORONOI_PATH						"./data/tests/log/voronoiPath/"
#define DEFAULT_TEST_VORONOI_PATH_NAME			 	"Test Voronoi Path"

#define OUT_FOLDER_VORONOI_PATH_COMP				"./data/tests/log/voronoiPathCompare/"
#define DEFAULT_TEST_VORONOI_PATH_COMP_NAME			"Test Voronoi Path Compare"

#include "Dcel.h"
#include "Delaunay.h"
#include "Point.h"
#include "testCompare.h"
#include "testExecution.h"

/****************************************************************************
// 							TestPathDelaunay CLASS DEFITNION
****************************************************************************/
class TestPathDelaunay : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestPathDelaunay(bool print) : \
					TestExecution(DEFAULT_TEST_DELAUNAY_PATH_NAME, \
					OUT_FOLDER_DELAUNAY_PATH, print) {};
	TestPathDelaunay(string outFolder, bool print) : \
			TestExecution(DEFAULT_TEST_DELAUNAY_PATH_NAME, outFolder, print) {};
	~TestPathDelaunay() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string pointsFileName, string dcelFileName, Point<TYPE> &p1, \
												Point<TYPE> &p2, Dcel &dcel);
	void main();
};

/****************************************************************************
//					TestPathVoronoiCompare CLASS DEFITNION
****************************************************************************/
class TestPathDelaunayCompare : public TestCompare
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestPathDelaunayCompare(bool print) : \
		TestCompare(DEFAULT_TEST_DELAUNAY_PATH_COMP_NAME, \
				OUT_FOLDER_DELAUNAY_PATH_COMP, print) {};
	TestPathDelaunayCompare(string outFolder, bool print) : \
		TestCompare(DEFAULT_TEST_DELAUNAY_PATH_COMP_NAME, outFolder, print) {};
	~TestPathDelaunayCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
};

/****************************************************************************
// 							TestPathVoronoi CLASS DEFITNION
****************************************************************************/
class TestPathVoronoi : public TestExecution
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestPathVoronoi(bool print) : TestExecution(DEFAULT_TEST_VORONOI_PATH_NAME, \
											OUT_FOLDER_VORONOI_PATH, print) {};
	TestPathVoronoi(string outFolder, bool print) : \
			TestExecution(DEFAULT_TEST_VORONOI_PATH_NAME, outFolder, print) {};
	~TestPathVoronoi() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void dump(string pointsFileName, string dcelFileName, Point<TYPE> &p1, \
												Point<TYPE> &p2, Dcel &dcel);
	void main();
};

/****************************************************************************
//					TestPathVoronoiCompare CLASS DEFITNION
****************************************************************************/
class TestPathVoronoiCompare : public TestCompare
{
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestPathVoronoiCompare(bool print) : \
			TestCompare(DEFAULT_TEST_VORONOI_PATH_COMP_NAME, \
					OUT_FOLDER_VORONOI_PATH_COMP, print) {};
	TestPathVoronoiCompare(string outFolder, bool print) : \
			TestCompare(DEFAULT_TEST_VORONOI_PATH_COMP_NAME, outFolder, print) {};
	~TestPathVoronoiCompare() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
};

#endif /* TEST_TESTPATH_H_ */

