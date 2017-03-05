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
#define DEFAULT_OUTPUT_FOLDER	"/home/juan/projects/delaunay/code/data/samples/errors/delaunay/"

// Default parameters in Delaunay Compare test.
#define DEFAULT_N_FILES			20
#define DEFAULT_FILE_LIST		"fileList.txt"

#include "Dcel.h"
#include "Parameter.h"
#include "test.h"
#include <string>
using namespace std;

/****************************************************************************
// 						TestDelaunayBuild CLASS DEFITNION
****************************************************************************/
class TestDelaunayBuild : public Test
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
	TestDelaunayBuild(string logFile, bool printData) :
									Test(logFile, printData), \
									nPoints(DEFAULT_N_POINTS), \
									deltaPoints(DEFAULT_DELTA_POINTS), \
									nSteps(DEFAULT_N_STEPS), \
									nTests(DEFAULT_N_TESTS), \
									outputFolder(DEFAULT_OUTPUT_FOLDER) {};
	~TestDelaunayBuild() {};

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
// 						TestDelaunayCompare CLASS DEFITNION
****************************************************************************/
class TestDelaunayCompare : public Test
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
	TestDelaunayCompare(string logFile, bool printData) :
							Test(logFile, printData), \
							filesNamesFile(DEFAULT_FILE_LIST), \
							filesList(DEFAULT_N_FILES) \
							{};
	~TestDelaunayCompare() {};

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


#endif /* TEST_TESTDELAUNAY_H_ */
