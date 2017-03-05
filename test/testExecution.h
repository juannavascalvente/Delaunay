/*
 * testExecution.h
 *
 *  Created on: Mar 5, 2017
 *      Author: juan
 */

#ifndef TEST_TESTEXECUTION_H_
#define TEST_TESTEXECUTION_H_

#include "test.h"

/****************************************************************************
// 							TestExecution CLASS DEFITNION
****************************************************************************/
class TestExecution : public Test
{
protected:
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	int nTests;
	int nSteps;
	int deltaPoints;
	int nPoints;

	//------------------------------------------------------------------------
	// Private functions
	//------------------------------------------------------------------------
	void initParameters();
	void applyParameter(Parameter *parameter, string value);
	void printParameters();
	void deallocate();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestExecution(string fileName, string outFolder, bool print) : \
						Test(fileName, outFolder, print),
						nTests(0), nSteps(0), deltaPoints(0), nPoints(0) {};
	~TestExecution() {};

	//------------------------------------------------------------------------
	//  Get/Set functions.
	//------------------------------------------------------------------------
	void setDeltaPoints(int deltaPoints) {this->deltaPoints = deltaPoints;};
	void setPoints(int points) {this->nPoints = points;};
	void setSteps(int steps) {this->nSteps = steps;};
	void setTests(int tests) {this->nTests = tests;};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	virtual void main() {};
};


#endif /* TEST_TESTEXECUTION_H_ */
