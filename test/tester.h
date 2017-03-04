/*
 * testr.h
 *
 *  Created on: Dec 8, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_TESTER_H_
#define CODE_TEST_TESTER_H_

#define DEFAULT_N_TESTS_TO_EXEC		20
#define DEFAULT_N_LABELS			20

#include "Set.h"
#include "test.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/****************************************************************************
// 							Test CLASS DEFITNION
****************************************************************************/
class Tester
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	string fileName;	// Input file name.
	ifstream ifs;		// Input file stream.
	Set<Test*> tests;	// Tests array.
	Set<Label> labels;	// Set of labels.
	bool 	printData;

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	bool 	readTests();
	Test* 	createTestInstance(TestType type);
	void 	finish();
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Tester() : fileName("test.txt"), printData(false) {};
	Tester(string fName, bool forcePrintData) : fileName(fName),
												tests(DEFAULT_N_TESTS_TO_EXEC),
												labels(DEFAULT_N_LABELS),
												printData(forcePrintData) {};
	~Tester() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void main();
};


#endif /* CODE_TEST_TESTER_H_ */
