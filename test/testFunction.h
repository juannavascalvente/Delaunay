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
	void execute(int &nTests, int &nFailed);
	const Set<testFunction<C> *>& getTestSet() const {return testSet;}
};

/***************************************************************************
* Name: 	execute
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	executes all the tests.
***************************************************************************/
template<class C> void ClassTests<C>::execute(int &nTests, int &nFailed)
{
	int i=0;	// Loop counter.
	testFunction<C> *funcTest;

	// Execute all the tests.
	nTests = this->testSet.getNElements();
	for (i=0; i<nTests ;i++)
	{
		C c;

		// Get test and execute it.
		funcTest = *this->testSet.at(i);
		Logging::buildText(funcTest->getTestName());
		if (funcTest->executeTest(c))
		{
			Logging::buildText("\t\t\t\t\tTEST OK");
			Logging::write(true, Successful);
		}
		else
		{
			nFailed++;
			Logging::buildText("\t\t\t\t\tTEST FAILED");
			Logging::write(true, Error);
		}
	}
}

#endif /* TEST_TESTFUNCTION_H_ */
