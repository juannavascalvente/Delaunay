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

#include <string>
using namespace std;

#include "Logging.h"
#include "Parameter.h"
#include "Set.h"

/****************************************************************************
// 							Data DEFITNION
****************************************************************************/
// Types of tests to execute.
enum TestType { UNKNOWN_TEST,
				TEST_SET,
				TEST_DELAUNAY,
				TEST_DELAUNAY_COMPARE,
				TEST_VORONOI_BUILD,
				TEST_VORONOI_COMPARE,
				TEST_PATH_DELAUNAY,
				TEST_PATH_DELAUNAY_COMPARE,
				TEST_PATH_VORONOI,
				TEST_PATH_VORONOI_COMPARE};

// Test types in string format.
#define TEST_SET_NAME						"TEST_SET"
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
	virtual void initParameters() {};
	virtual void applyParameter(Parameter *parameter, string value) {cout << "DEFAULT APPLY" << endl;};
	virtual void printParameters() {};
	virtual bool prepare() {return(true);};
	virtual void main() {};
	virtual void write() {};
	virtual void deallocate() {};

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	bool getParameterIndex(Label &label, int &index);
	void parseParameters(Set<Label> &labels);
	static TestType getTypeTest(string testName);
protected:
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	Logging logFile;		// Test log file.
	Set<Parameter*> parameters;
	string outFolder;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Test() : logFile("logTest.txt", false),
			 parameters(DEFAULT_TEST_N_PARAMETERS), \
			 outFolder(DEFAULT_OUTPUT_FOLDER) {};
	Test(const string fileName, string folder, bool printData = false) : \
			logFile(fileName, printData), \
			parameters(DEFAULT_TEST_N_PARAMETERS), \
			outFolder(folder) {};
	virtual ~Test() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	static bool read(ifstream &ifs, Set<Label> &labels, TestType &testType);
	void init(Set<Label> &labels);
	void run();
	void finish();

	const string& getOutFolder() const {return outFolder;}
	void setOutFolder(const string& outFolder) {this->outFolder = outFolder;}
};

#endif /* CODE_TEST_TEST_H_ */
