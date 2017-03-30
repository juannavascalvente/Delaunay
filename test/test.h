/*
 * test.h
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_TEST_H_
#define CODE_TEST_TEST_H_

#define DEFAULT_TEST_N_PARAMETERS		20
#define	DEFAULT_OUTPUT_FOLDER			"."
#define	DEFAULT_LOG_FILENAME			"log.txt"
#define	DEFAULT_STAT_FILENAME			"statistics.txt"

#include <string>
#include <unistd.h>
using namespace std;

#include "Dcel.h"
#include "Delaunay.h"
#include "Logging.h"
#include "Parameter.h"
#include "Set.h"
#include "Statistics.h"
#include "Voronoi.h"

/****************************************************************************
// 							Data DEFITNION
****************************************************************************/
// Types of tests to execute.
enum TestType { UNKNOWN_TEST,
				TEST_SET,
				TEST_DCEL,
				TEST_DELAUNAY,
				TEST_DELAUNAY_COMPARE,
				TEST_CONVEXHULL,
				TEST_CONVEXHULL_COMPARE,
				TEST_VORONOI_BUILD,
				TEST_VORONOI_COMPARE,
				TEST_PATH_DELAUNAY,
				TEST_PATH_DELAUNAY_COMPARE,
				TEST_PATH_VORONOI,
				TEST_PATH_VORONOI_COMPARE};

// Test types in string format.
#define TEST_SET_NAME						"TEST_SET"
#define TEST_DCEL_NAME						"TEST_DCEL"
#define TEST_CONVEXHULL_NAME				"TEST_CONVEXHULL"
#define TEST_CONVEXHULL_COMPARE_NAME		"TEST_CONVEXHULL_COMPARE"
#define TEST_DELAUNAY_NAME					"TEST_DELAUNAY"
#define TEST_DELAUNAY_COMPARE_NAME			"TEST_DELAUNAY_COMPARE"
#define TEST_VORONOI_NAME					"TEST_VORONOI"
#define TEST_VORONOI_COMPARE_NAME			"TEST_VORONOI_COMPARE"
#define TEST_PATH_DELAUNAY_NAME				"TEST_PATH_DELAUNAY"
#define TEST_PATH_DELAUNAY_COMPARE_NAME		"TEST_PATH_DELAUNAY_COMPARE"
#define TEST_PATH_VORONOI_NAME				"TEST_PATH_VORONOI"
#define TEST_PATH_VORONOI_COMPARE_NAME		"TEST_PATH_VORONOI_COMPARE"

/****************************************************************************
// 							Test CLASS DEFITNION
****************************************************************************/
class Test
{
	//------------------------------------------------------------------------
	// Private virtual functions.
	//------------------------------------------------------------------------
	virtual void applyParameter(Parameter *parameter, string value) \
										{cout << "DEFAULT APPLY" << endl;};
	virtual void printParameters() {cout << "DEFAULT PRINT" << endl;};
	virtual bool prepare() {cout << "DEFAULT PREPARE" << endl; return(true);};
	virtual void main() {cout << "DEFAULT MAIN" << endl;};
	void write();

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	bool getParameterIndex(Label &label, int &index);
	void parseParameters(Set<Label> &labels);
	int  removeExistingFiles();
	static TestType getTypeTest(string testName);
protected:
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	string testName;				// Test name.
	string outFolder;				// Test output folder.
	string logFileName;				// Log output file name.
	string statFileName;			// Statistics output file name.
	int	totalTests;					// Total # of tests.
	int	testCounter;				// Current test.
	int	nTestFailed;				// # tests failed.
	Set<Parameter*> parameters;		// Test parameters.
	bool print;						// Print log data.
	StatisticsRegister *stat;		// Pointer to statistics data.

	virtual void initParameters();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Test() : testName(""), outFolder(DEFAULT_OUTPUT_FOLDER), \
			 logFileName(DEFAULT_LOG_FILENAME), \
			 statFileName(DEFAULT_STAT_FILENAME), totalTests(0), \
			 testCounter(0), nTestFailed(0), \
			 parameters(DEFAULT_TEST_N_PARAMETERS), print(true), \
			 stat(NULL) {};
	Test(string name, string folder, bool print) : testName(name), \
			outFolder(folder), logFileName(DEFAULT_LOG_FILENAME), \
			statFileName(DEFAULT_STAT_FILENAME), totalTests(0), \
			testCounter(0), nTestFailed(0), \
			parameters(DEFAULT_TEST_N_PARAMETERS), print(print), \
			stat(NULL) {};
	virtual ~Test();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	static bool read(ifstream &ifs, Set<Label> &labels, TestType &testType);
	void init(Set<Label> &labels);
	void run();
	void finish(){};

	// Common functions to execute tests.
	bool readDelaunay(string filName, Dcel &dcel, Delaunay &delaunay);
	bool buildRandomDelaunay(int nPoints, Dcel &dcel, Delaunay &delaunay);
	bool readVoronoi(string filName, Dcel& dcel, Delaunay &delaunay, Voronoi &voronoi);
	bool buildVoronoi(int nPoints, Dcel& dcel, Delaunay &delaunay, Voronoi &voronoi);

	const string& getOutFolder() const {return outFolder;}
	void setOutFolder(const string& outFolder) {this->outFolder = outFolder;}
	const string& getTestName() const {return testName;}
	int getTestFailed() const {return nTestFailed;}
	int getTotalTests() const {return totalTests;}
};

#endif /* CODE_TEST_TEST_H_ */

