/*
 * testFunction.h
 *
 *  Created on: Mar 10, 2017
 *      Author: juan
 */

#ifndef TEST_TESTFUNCTION_H_
#define TEST_TESTFUNCTION_H_

#include "Set.h"
#include "test.h"

#define DEFAULT_N_TESTS		10

//****************************************************************************
//                           testFunction CLASS
//****************************************************************************
template<class C>
class testFunction
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------

	string 	testName;
	int 	testId;
	string  msg;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	testFunction(string name, int id) : testName(name), testId(id), msg("") {};
	virtual ~testFunction() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
    virtual bool executeTest(C &c) = 0;
    void writeResult() {};

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	int getTestId() const {return testId;}
	const string& getTestName() const {return testName;}
	const string& getErrorMessage() const {return msg;}
	void setErrorMessage(const string& msg) {this->msg = msg;}
};

//****************************************************************************
//                           ClassTests CLASS
//****************************************************************************
template<class C>
class ClassTests
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	Set<testFunction<C> *> testSet;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	ClassTests() : testSet(DEFAULT_N_TESTS) {};
	~ClassTests() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void add(testFunction<C> *func) {this->testSet.add(func);};
	void execute();
};

/***************************************************************************
* Name: 	execute
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	executes all the tests.
***************************************************************************/
template<class C> void ClassTests<C>::execute()
{
	int i=0;	// Loop counter.
	testFunction<C> *funcTest;

	// Execute all the tests.
	for (i=0; i<this->testSet.getNElements() ;i++)
	{
		C c;

		// Get test and execute it.
		funcTest = *this->testSet.at(i);
		if (funcTest->executeTest(c))
		{
			string message = "Test " + funcTest->getTestName();
			Logging::buildText(__FUNCTION__, __FILE__, message);
			Logging::buildText(__FUNCTION__, __FILE__, "......TEST OK");
			Logging::write(true, Successful);
		}
		else
		{
			string message = "Test " + funcTest->getTestName();
			Logging::buildText(__FUNCTION__, __FILE__, message);
			Logging::buildText(__FUNCTION__, __FILE__, "......TEST FAILED");
			Logging::write(true, Error);
			Logging::buildText(__FUNCTION__, __FILE__, funcTest->getErrorMessage());
			Logging::write(true, Error);
		}
	}
}

#endif /* TEST_TESTFUNCTION_H_ */
