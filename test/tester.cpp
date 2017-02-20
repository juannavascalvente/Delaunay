/*
 * test.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#include "tester.h"
#include "testSet.h"
#include "testPath.h"
#include "testVoronoi.h"


bool Tester::init()
{
	bool read=true;					// Return value.
	bool finished=false; 			// Loop control flag.
	ifstream ifs;					// Input file stream.
	enum TesterTestType testType;	// Test type read from file.
	Test 		*test;

	// Open file.
	ifs.open(this->fileName.c_str(), ios::in);
	if (!ifs.is_open())
	{
		cout << "Error opening input test file " << this->fileName << endl;
		read = false;
	}
	else
	{
		// Execute all tests.
		finished = false;
		while (!finished)
		{
			cout << "Reading test " << (this->tests.getNElements()+1) << endl;

			// Check test type.
			testType = this->readTestType();
			switch(testType)
			{
				// Test Set class.
				case TEST_SET:
				{
					cout << "Testing Set" << endl;
					test = new TestSet();
					finished = true;
					break;
				}
				// Test Path computations.
				case TEST_PATH:
				{
					cout << "Testing Path" << endl;
					test = new TestPath();
					break;
				}
				default:
				{
					cout << "Unknown test type. Value is " << testType << endl;
					break;
				}
			}

			// Initialize test.
			test->init();
			this->tests.add(test);
		}

		// Close input file.
		ifs.close();

		cout << "Finished reading tests." << endl;
	}

	return(read);
}

void Tester::main()
{
	int	 i=0;						// Loop counter.
	Test *currentTest;

	// Initialize tester.
	if (this->init())
	{
		// Execute all tests.
		for(i=0; i<this->tests.getNElements() ;i++)
		{
			cout << "Executing test " << (i+1) << endl;
			currentTest = *this->tests.at(i);
			currentTest->run();
		}

		this->finish();
	}
}

void Tester::finish()
{

}

TesterTestType Tester::readTestType()
{
	static bool firstTime=true;
	if (firstTime) {
		firstTime=false;
		return(TEST_SET);

	} else {
		return(TEST_PATH);
	}

}
