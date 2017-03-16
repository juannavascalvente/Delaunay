/*
 * testSet.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#include "assert.h"
#include "defines.h"
#include "testSet.h"
#include "test.h"
#include "testFunction.h"

#define HALFSIZE		50
#define SIZE			HALFSIZE*2
#define DOUBLESIZE		SIZE*2

#define LONG_SET		10

class TestAdd : public testFunction<Set<int> >
{
public:
	TestAdd() : testFunction("Add elements", 1) {};
	~TestAdd() {};
    bool executeTest(Set<int> &set);
};

bool TestAdd::executeTest(Set<int> &set)
{
	int 	i=0;			// Loop counter.
	bool 	success=true;	// Return value.
	int		size=0;

	// Add elements.
	i = 0;
	size = set.getSize();
	while ((i<size) && (success))
	{
		set.add(i);
		if (Assert::assertNotEqual(set.getSize(), size))
		{
			ostringstream convert;
			convert << set.getSize();
			ostringstream convert2;
			convert2 << size;
			string msg = "";
			msg = msg + "Wrong size. Should be " + convert2.str() + " and is " + convert.str();
			this->setErrorMessage(msg);
			success = false;
		}
		if (Assert::assertNotEqual(set.getNElements(), i+1))
		{
			ostringstream convert;
			convert << i+1;
			ostringstream convert2;
			convert2 << set.getNElements();
			string msg = "";
			msg = msg + "Wrong # elements. Should be " + convert.str() + " and is " + convert2.str();
			this->setErrorMessage(msg);
			success = false;
		}
		i++;
	}

	return(success);
}

class TestResize : public testFunction<Set<int> >
{
public:
	TestResize() : testFunction("Resize", 1) {};
	~TestResize() {};
    bool executeTest(Set<int> &set);
};

bool TestResize::executeTest(Set<int> &set)
{
	int 	i=0;			// Loop counter.
	bool 	success=true;	// Return value.
	int		size=0;

	// Add elements.
	size = set.getSize()*2;
	for (i=0; i<size ;i++)
	{
		set.add(i);
	}

	if (Assert::assertNotEqual(set.getSize(), size))
	{
		ostringstream convert;
		convert << set.getSize();
		ostringstream convert2;
		convert2 << size;
		string msg = "";
		msg = msg + "Wrong size. Should be " + convert2.str() + " and is " + convert.str();
		this->setErrorMessage(msg);
		success = false;
	}

	return(success);
}

class TestEmptyFull : public testFunction<Set<int> >
{
public:
	TestEmptyFull() : testFunction("Empty and Full", 1) {};
	~TestEmptyFull() {};
    bool executeTest(Set<int> &set);
};

bool TestEmptyFull::executeTest(Set<int> &set)
{
	int 	i=0;			// Loop counter.
	bool 	success=true;	// Return value.
	int		size=0;

	if (set.isFull())
	{
		ostringstream convert;
		convert << set.getNElements();
		string msg = "";
		msg = msg + "Set must not be full but it has " + convert.str() + " elements.";
		this->setErrorMessage(msg);
		success = false;
	}
	else if (!set.isEmpty())
	{
		ostringstream convert;
		convert << set.getNElements();
		string msg = "";
		msg = msg + "Set must be empty but it has " + convert.str() + " elements.";
		this->setErrorMessage(msg);
		success = false;
	}
	else
	{
		// Add elements.
		size = set.getSize()/2;
		for (i=0; i<size ;i++)
		{
			set.add(i);
		}

		if (set.isFull() || set.isEmpty())
		{
			ostringstream convert;
			convert << set.getNElements();
			string msg = "";
			msg = msg + "Set not be empty nor full but it has " + convert.str() + " elements.";
			this->setErrorMessage(msg);
			success = false;
		}
		else
		{
			// Add elements.
			for (i=size; i<size*2 ;i++)
			{
				set.add(i);
			}

			if (!set.isFull())
			{
				ostringstream convert;
				convert << set.getNElements();
				string msg = "";
				msg = msg + "Set must be full but it has " + convert.str() + " elements.";
				this->setErrorMessage(msg);
				success = false;
			}
			else if (set.isEmpty())
			{
				ostringstream convert;
				convert << set.getNElements();
				string msg = "";
				msg = msg + "Set must not be empty but it has " + convert.str() + " elements.";
				this->setErrorMessage(msg);
				success = false;
			}
		}
	}

	return(success);
}

class TestAddOpenMP : public testFunction<Set<int> >
{
public:
	TestAddOpenMP() : testFunction("Test add openMP", 1) {};
	~TestAddOpenMP() {};
    bool executeTest(Set<int> &set);
};

bool TestAddOpenMP::executeTest(Set<int> &set)
{
	bool success=true;		// Return value.
	int i=0;				// Loop counter.
	Set<int>	currentSet(10);
	Set<int>	final(10);

	// Initialize set.
	for (i=0; i<LONG_SET ;i++)
	{
		currentSet.add(i);
		final.add(i+1);
	}

	currentSet++;
	if (!(currentSet == final))
	{
		string msg = "";
		msg = msg + "Post increment test failed. Sets are not equal.";
		this->setErrorMessage(msg);
		success = false;
	}

	return(success);
}

void TestSet::main()
{
	ClassTests<Set<int> > testSetClass;
	TestAdd			testAdding;
	TestResize		testResize;
	TestEmptyFull	testEmptyFull;
	TestAddOpenMP	testAddOpenMP;

	testSetClass.add(&testAdding);
	testSetClass.add(&testResize);
	testSetClass.add(&testEmptyFull);
	testSetClass.add(&testAddOpenMP);
	testSetClass.execute(this->totalTests, this->nTestFailed);
}

