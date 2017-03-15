/*
 * testDcel.cpp
 *
 *  Created on: Mar 15, 2017
 *      Author: juan
 */

#include "Dcel.h"
#include "testDcel.h"
#include "testFunction.h"

/****************************************************************************
// 								DCEL CLASS DEFITNION
****************************************************************************/
class TestSetHighestFirst : public testFunction<Dcel>
{
public:
	TestSetHighestFirst() : testFunction("Test setHighestFirst openMP", 1) {};
	~TestSetHighestFirst() {};
    bool executeTest(Dcel &dcel);
};

bool TestSetHighestFirst::executeTest(Dcel &dcel)
{
	bool success=true;		// Return value.
	int  nPoints=10;		// # points in set.
	int  index=0;			// Index to modify.
	int	 computedIndex=0;	// Index computed in test.
	int	 i=0;				// Loop counter.

	// Initialize set.
	dcel.generateRandom(nPoints);

	// Generate random index.
	index = random() % dcel.getNVertex();
	for (i=0;i<dcel.getNVertex() ;i++)
	{
		if (i != index)
		{
			dcel.getRefPoint(i)->setOrigin();
		}
	}
	computedIndex = dcel.getIndexHighest(&Point<TYPE>::lexicographicHigher);
	if (index != computedIndex)
	{
		ostringstream convert;
		convert << index;
		ostringstream convert2;
		convert2 << computedIndex;
		string msg = "";
		msg = msg + "Wrong index. Should be " + convert.str() + " and is " + convert2.str();
		this->setErrorMessage(msg);
		success = false;
	}

	return(success);
}

void TestDcel::main()
{
	ClassTests<Dcel> test;
	TestSetHighestFirst		setHighestFirstTest;

	test.add(&setHighestFirstTest);
	test.execute();
}

