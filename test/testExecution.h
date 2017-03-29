/*
 * testExecution.h
 *
 *  Created on: Mar 5, 2017
 *      Author: juan
 */

#ifndef TEST_TESTEXECUTION_H_
#define TEST_TESTEXECUTION_H_

#include "Statistics.h"
#include "test.h"

// Default parameters in Delaunay Build test.
#define DEFAULT_N_POINTS		3
#define DEFAULT_DELTA_POINTS	2
#define DEFAULT_N_STEPS			5
#define DEFAULT_N_TESTS			5

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
	bool prepare();
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestExecution(string name, string outFolder, bool print) : \
						Test(name, outFolder, print), \
						nTests(DEFAULT_N_TESTS), nSteps(DEFAULT_N_STEPS), \
						deltaPoints(DEFAULT_DELTA_POINTS), \
						nPoints(DEFAULT_N_POINTS) {};
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
