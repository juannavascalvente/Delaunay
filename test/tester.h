/*
 * testr.h
 *
 *  Created on: Dec 8, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_TESTER_H_
#define CODE_TEST_TESTER_H_

#define DEFAULT_N_TESTS	20

#include "Set.h"
#include "test.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/****************************************************************************
// 							Data DEFITNION
****************************************************************************/
// Types of tests to execute.
enum TesterTestType { TEST_SET,
					TEST_DELAUNAY,
					TEST_VORONOI,
					TEST_PATH};

/****************************************************************************
// 							Test CLASS DEFITNION
****************************************************************************/
class Tester
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	string fileName;	// Input file name.
	Set<Test*> tests;	// Tests array.

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	bool init();
	void finish();
	TesterTestType readTestType();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Tester() : fileName("test.txt") {};
	Tester(string fName) : fileName(fName), tests(DEFAULT_N_TESTS) {};
	~Tester() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void main();
};


#endif /* CODE_TEST_TESTER_H_ */
