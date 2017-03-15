/*
 * testDcel.h
 *
 *  Created on: Mar 15, 2017
 *      Author: juan
 */

#ifndef TEST_TESTDCEL_H_
#define TEST_TESTDCEL_H_

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
	TestDcel(string fileName, string outFolder, bool print) : \
						Test("Test Dcel", fileName, outFolder, print) {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void main();
};

#endif /* TEST_TESTDCEL_H_ */
