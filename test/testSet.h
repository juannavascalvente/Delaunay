/*
 * testSet.h
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_TESTSET_H_
#define CODE_TEST_TESTSET_H_

#define OUT_FOLDER_SET					"./data/tests/log/setTest/"
#define DEFAULT_TEST_NAME				"Test Set"

#include "test.h"

/****************************************************************************
// 						TestSet CLASS DEFITNION
****************************************************************************/
class TestSet : public Test
{
	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	void checkDefaultConstructor();
	void checkCustomConstructor();
	bool testEqualOpenMP();
	bool testAddOpenMP();

	bool prepare() {return(true);};
	void setDefault() {};
	bool parseParameters(Set<Label> &labels) {return(true);};
	void write() {};
	void print() {};
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestSet(bool print) : Test(DEFAULT_TEST_NAME, OUT_FOLDER_SET, print) {};
	TestSet(string outFolder,bool print) : Test("Test Set",outFolder,print){};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void main();
};

#endif /* CODE_TEST_TESTSET_H_ */
