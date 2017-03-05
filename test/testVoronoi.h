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
#define VORONOI_DEFAULT_OUTPUT_FOLDER	"/home/juan/projects/delaunay/code/data/samples/errors/voronoi/"

// Default parameters in Voronoi Compare test.
#define DEFAULT_N_FILES			20
#define DEFAULT_FILE_LIST		"fileList.txt"

#include "test.h"
using namespace std;

/****************************************************************************
// 						TestVoronoiBuild CLASS DEFITNION
****************************************************************************/
class TestVoronoiBuild : public Test
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	int nPoints;
	int deltaPoints;
	int nSteps;
	int nTests;
	string outputFolder;

	//------------------------------------------------------------------------
	// Private functions
	//------------------------------------------------------------------------
	void initParameters();
	void applyParameter(Parameter *parameter, string value);
	void printParameters();
	static void dump(string dcelFileName, Dcel &dcel);
	void deallocate();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestVoronoiBuild(string logFile, bool printData) :
							Test(logFile, printData), \
							nPoints(VORONOI_DEFAULT_N_POINTS), \
							deltaPoints(VORONOI_DEFAULT_DELTA_POINTS), \
							nSteps(VORONOI_DEFAULT_N_STEPS), \
							nTests(VORONOI_DEFAULT_N_TESTS), \
							outputFolder(VORONOI_DEFAULT_OUTPUT_FOLDER) {};
	~TestVoronoiBuild() {};

	//------------------------------------------------------------------------
	// Public functions
	//------------------------------------------------------------------------
	void main();

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	void setPoints(int points) {nPoints = points;}
	void setDeltaPoints(int deltaPoints) {this->deltaPoints = deltaPoints;}
	void setSteps(int steps) {nSteps = steps;}
	void setTests(int tests) {nTests = tests;}
};

/****************************************************************************
// 						TestVoronoiCompare CLASS DEFITNION
****************************************************************************/
class TestVoronoiCompare : public Test
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	string filesNamesFile;
	Set<string> filesList;

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	void initParameters();
	void printParameters();
	void applyParameter(Parameter *parameter, string value);
	void deallocate();
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestVoronoiCompare(string logFile, bool printData) :
							Test(logFile, printData), \
							filesNamesFile(DEFAULT_FILE_LIST), \
							filesList(DEFAULT_N_FILES) \
							{};
	~TestVoronoiCompare() {};

	//------------------------------------------------------------------------
	//  Get/Set functions.
	//------------------------------------------------------------------------
	void setFileNamesFile(string value) {this->filesNamesFile = value;};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	bool prepare();
	void main();
};



#endif /* TEST_TESTVORONOI_H_ */
