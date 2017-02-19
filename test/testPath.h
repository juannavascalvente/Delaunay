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
#include "test.h"

/****************************************************************************
// 							Data DEFITNION
****************************************************************************/
// Types of tests to execute.
enum enumTypeTest { TESTPATH_DELAUNAY_COMPARE_TO_GOLD,
					TESTPATH_VORONOI_COMPARE_TO_GOLD,
					TESTPATH_DELAUNAY_RANDOM_INCREMENTAL_STEP,
					TESTPATH_VORONOI_RANDOM_INCREMENTAL_STEP
};

/****************************************************************************
// 							TestPath CLASS DEFITNION
****************************************************************************/
class TestPath : public Test
{
	// Input/output files.
	string baseFileName;
	string fileName;

	// Tests parameters.
	enum enumTypeTest testType;
	int	nTests;
	int nSteps;
	int	step;
	int nPoints;

	// Data attributes.
	Dcel			dcel;		// Dcel data.
	Delaunay		delaunay;	// Delaunay data.
	Voronoi			voronoi;	// Voronoi data.

	// Initialization functions.
	bool createSet();
	bool buildDelaunay();
	bool buildVoronoi();

	bool initializeDelaunay();
	bool initializeVoronoi();

	void dump(string fileName, string dcelFileName, Point<TYPE> &p1, Point<TYPE> &p2, Dcel &dcel);
	void DelaunayPath();
	void VoronoiPath();

	void print();
	void setDefault();

public:
	TestPath() : testType(TESTPATH_DELAUNAY_COMPARE_TO_GOLD), \
				 nTests(0), nSteps(0), step(0), nPoints(0) {};
	~TestPath() {};

	void prepare();
	void main();
	bool read();
};

#endif /* TEST_TESTPATH_H_ */
/*dcel.generateRandom(this->config->getNPoints()); */
