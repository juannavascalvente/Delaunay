/*
 * testVoronoi.h
 *
 *  Created on: Feb 8, 2017
 *      Author: juan
 */

#ifndef TEST_TESTVORONOI_H_
#define TEST_TESTVORONOI_H_

// Default parameters in Voronoi Build test.
#define DEFAULT_N_POINTS		3
#define DEFAULT_DELTA_POINTS	2
#define DEFAULT_N_STEPS			5
#define DEFAULT_N_TESTS			5
#define DEFAULT_OUTPUT_FOLDER	"/home/juan/projects/delaunay/code/data/samples/errors/voronoi/"

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
	void dump(string dcelFileName, Dcel &dcel);
	void deallocate();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestVoronoiBuild(string logFile, bool printData) :
									Test(logFile, printData), \
									nPoints(DEFAULT_N_POINTS), \
									deltaPoints(DEFAULT_DELTA_POINTS), \
									nSteps(DEFAULT_N_STEPS), \
									nTests(DEFAULT_N_TESTS), \
									outputFolder(DEFAULT_OUTPUT_FOLDER) {};
	~TestVoronoiBuild() {};

	//------------------------------------------------------------------------
	// Public functions
	//------------------------------------------------------------------------
	void main();

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	int getPoints() const {return nPoints;}
	void setPoints(int points) {nPoints = points;}
	int getDeltaPoints() const {return deltaPoints;}
	void setDeltaPoints(int deltaPoints) {this->deltaPoints = deltaPoints;}
	int getSteps() const {return nSteps;}
	void setSteps(int steps) {nSteps = steps;}
	int getTests() const {return nTests;}
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
	//  Public functions.
	//------------------------------------------------------------------------
	bool prepare();
	void main();
};



#endif /* TEST_TESTVORONOI_H_ */
