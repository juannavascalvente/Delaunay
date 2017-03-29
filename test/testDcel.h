/*
 * testDcel.h
 *
 *  Created on: Mar 15, 2017
 *      Author: juan
 */

#ifndef TEST_TESTDCEL_H_
#define TEST_TESTDCEL_H_

#define OUT_FOLDER_DCEL					"./data/tests/log/dcelTest/"
#define DEFAULT_TEST_DCEL_NAME			"Test Dcel"

#include "test.h"

//****************************************************************************
// 							DCEL TEST CLASS DEFITNION
//****************************************************************************
class TestDcel : public Test
{
public:
	//------------------------------------------------------------------------
	//  Constructor/Destructor.
	//------------------------------------------------------------------------
	TestDcel(bool print) : \
		Test(DEFAULT_TEST_DCEL_NAME, OUT_FOLDER_DCEL, print) {};
	TestDcel(string fileName, string outFolder, bool print) : \
		Test(DEFAULT_TEST_DCEL_NAME, outFolder, print) {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void main();
};

#endif /* TEST_TESTDCEL_H_ */
