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

#define HALFSIZE		5
#define SIZE			HALFSIZE*2
#define DOUBLESIZE		SIZE*2
#define LONG_SET		10

void TestSet::checkDefaultConstructor()
{
	Set<int>	invalidSet;					// Default constructor set.

	// Check is full and empty functions.
	if (!invalidSet.isFull())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. IsFull failed. Should be full but it is not.");
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. IsFull OK.");
		this->logFile.write(true, Successful);
	}
	if (!invalidSet.isEmpty())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. isEmpty failed. Should be empty but it is not.");
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. isEmpty OK.");
		this->logFile.write(true, Successful);
	}
	// Check attributes initialization in default constructor.
	if (Assert::assertNotEqual(invalidSet.getSize(), 0))
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. Get size in empty set failed. Should be ");
		this->logFile.buildText(__FUNCTION__, __FILE__, 0);
		this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, invalidSet.getSize());
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. Get size in empty set OK.");
		this->logFile.write(true, Successful);
	}
	if (Assert::assertNotEqual(invalidSet.getNElements(), 0))
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. Get # elements in empty set failed. Should be ");
		this->logFile.buildText(__FUNCTION__, __FILE__, 0);
		this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, invalidSet.getSize());
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 1. Get # elements in empty set OK.");
		this->logFile.write(true, Successful);
	}
}

void TestSet::checkCustomConstructor()
{
	Set<int>	newSet=Set<int>(HALFSIZE);	// Custom constructor set.

	// Check is full and empty functions.
	if (newSet.isFull())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. IsFull failed. Should be empty but it is full.");
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. IsFull OK.");
		this->logFile.write(true, Successful);
	}
	if (!newSet.isEmpty())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. isEmpty failed. Should be empty but it is not.");
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. isEmpty OK.");
		this->logFile.write(true, Successful);
	}
	// Check attributes initialization.
	if (Assert::assertNotEqual(newSet.getSize(), HALFSIZE))
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. Get size in constructor set failed. Should be ");
		this->logFile.buildText(__FUNCTION__, __FILE__, HALFSIZE);
		this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, newSet.getSize());
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. Get size in constructor set OK.");
		this->logFile.write(true, Successful);
	}
	if (Assert::assertNotEqual(newSet.getNElements(), 0))
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. Get # elements in constructor set failed. Should be ");
		this->logFile.buildText(__FUNCTION__, __FILE__, 0);
		this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, newSet.getNElements());
		this->logFile.write(true, Error);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST 2. Get # elements in constructor set OK.");
		this->logFile.write(true, Successful);
	}
}

void TestSet::main()
{
	int	i=0;								// Loop counter.
	int highest=0;
	int highestIndex=0;
	int updatedIndex=0;

	// Check is full and empty functions.
	checkDefaultConstructor();

	// Check is full and empty functions.
	checkCustomConstructor();

	// Add elements.
	for (i=0; i<HALFSIZE ;i++)
	{
		this->set.add(i);
		if (Assert::assertNotEqual( this->set.getSize(), DEFAULT_SIZE))
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get size after adding element failed.Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, DEFAULT_SIZE);
			this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getSize());
			this->logFile.write( true, Error);
		}
		else
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get size after adding element OK.");
			this->logFile.write( true, Successful);
		}
		if (Assert::assertNotEqual( this->set.getNElements(), i+1))
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get # elements after adding element failed.Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, i+1);
			this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getNElements());
			this->logFile.write( true, Error);

		}
		else
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get # elements after adding element OK.");
			this->logFile.write( true, Successful);
		}
	}

	// Add elements.
	for (i=HALFSIZE; i<DEFAULT_SIZE ;i++)
	{
		this->set.add(i);
		if (Assert::assertNotEqual( this->set.getSize(), DEFAULT_SIZE))
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get size after adding element failed.Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, DEFAULT_SIZE);
			this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getSize());
			this->logFile.write( true, Error);
		}
		else
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get size after adding element OK.");
			this->logFile.write( true, Successful);
		}
		if (Assert::assertNotEqual( this->set.getNElements(), i+1))
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get # elements after adding element failed.Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, i+1);
			this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getNElements());
			this->logFile.write( true, Error);

		}
		else
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get # elements after adding element OK.");
			this->logFile.write( true, Successful);
		}
	}

	// Check element values.
	for (i=0; i<SIZE ;i++)
	{
		if (Assert::assertNotEqual( *this->set.at(i), i))
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 4. At element failed. Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, i);
			this->logFile.buildText(__FUNCTION__, __FILE__, " and it is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, *this->set.at(i));
			this->logFile.write( true, Error);

		}
		else
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 4. At element OK.");
			this->logFile.write( true, Successful);
		}
	}

	if (this->set.isEmpty())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 5. Checking if set is empty failed. Set should be empty but is has ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getNElements());
		this->logFile.buildText(__FUNCTION__, __FILE__, " and size is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getSize());
		this->logFile.write( true, Error);

	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 5. isEmpty OK.");
		this->logFile.write( true, Successful);
	}
	if (!this->set.isFull())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 5. Checking if set is not full failed. Set should be full but is has ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getNElements());
		this->logFile.buildText(__FUNCTION__, __FILE__, " and size is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getSize());
		this->logFile.write( true, Error);

	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 5. isFull OK.");
		this->logFile.write( true, Successful);
	}

	// Check reset.
	this->set.reset();
	if (this->set.isFull())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 6. Checking if set is empty failed. Set should be empty but is has ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getNElements());
		this->logFile.buildText(__FUNCTION__, __FILE__, " and size is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getSize());
		this->logFile.write( true, Error);

	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 6. isEmpty OK.");
		this->logFile.write( true, Successful);
	}
	if (!this->set.isEmpty())
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 6. Checking if set is not full failed. Set should be full but is has ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getNElements());
		this->logFile.buildText(__FUNCTION__, __FILE__, " and size is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, this->set.getSize());
		this->logFile.write( true, Error);

	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 6. isFull OK.");
		this->logFile.write( true, Successful);
	}

	// Check get highest element.
	for (i=0; i<DOUBLESIZE ;i++)
	{
		this->set.add(i+1);
	}

	highest = this->set.getHighest();
	highestIndex = this->set.getIndexHighest();
	if (highest != DOUBLESIZE)
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 7. Checking highest element. Should be ");
		this->logFile.buildText(__FUNCTION__, __FILE__, DOUBLESIZE);
		this->logFile.buildText(__FUNCTION__, __FILE__, " and is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, highest);
		this->logFile.write( true, Error);
	}
	else if (highestIndex != (i-1))
	{
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 7. Checking highest index element. Should be ");
		this->logFile.buildText(__FUNCTION__, __FILE__, (i-1));
		this->logFile.buildText(__FUNCTION__, __FILE__, " and is ");
		this->logFile.buildText(__FUNCTION__, __FILE__, highestIndex);
		this->logFile.write( true, Error);
	}
	else
	{
		updatedIndex = 3;
		this->set.update(updatedIndex, DOUBLESIZE*2);
		highest = this->set.getHighest();
		highestIndex = this->set.getIndexHighest();
		if (highest != DOUBLESIZE*2)
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 7. Checking highest element after updated. Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, DOUBLESIZE*2);
			this->logFile.buildText(__FUNCTION__, __FILE__, " and is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, highest);
			this->logFile.write( true, Error);
			this->set.print();
		}
		else if (highestIndex != updatedIndex)
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 7. Checking highest index element after updated. Should be ");
			this->logFile.buildText(__FUNCTION__, __FILE__, (updatedIndex-1));
			this->logFile.buildText(__FUNCTION__, __FILE__, " and is ");
			this->logFile.buildText(__FUNCTION__, __FILE__, highestIndex);
			this->logFile.write( true, Error);
			this->set.print();
		}
		{
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 7. highest OK.");
			this->logFile.write( true, Successful);
		}
	}

	// Test post == operator using openMP.
	this->testEqualOpenMP();

	// Test post increment opeartor using openMP.
	this->testAddOpenMP();
}

bool TestSet::testEqualOpenMP()
{
	bool success=false;		// Return value.
	int i=0;				// Loop counter.
	Set<int>	currentSet(10);
	Set<int>	final(10);

	// Initialize set.
	for (i=0; i<LONG_SET ;i++)
	{
		currentSet.add(i);
		final.add(i+1);
	}

	// Check both sets are different
	if (currentSet == final)
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST testEqualOpenMP FAILED");
		this->logFile.write(true, Error);
	}
	else
	{
		success = true;
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST testEqualOpenMP OK");
		this->logFile.write(true, Successful);
	}

	return(success);
}

bool TestSet::testAddOpenMP()
{
	bool success=false;		// Return value.
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
	if (currentSet == final)
	{
		success = true;
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST testAddOpenMP OK");
		this->logFile.write(true, Successful);
	}
	else
	{
		this->logFile.buildText(__FUNCTION__, __FILE__,
				"TEST testAddOpenMP FAILED");
		this->logFile.write(true, Error);
	}

	return(success);
}
