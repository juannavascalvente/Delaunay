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
				"TEST 1. IsFull \t\t\t\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 1. isEmpty \t\t\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 1. Get size in empty set\t\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 1. Get # elements in empty set\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 2. IsFull \t\t\t\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 2. isEmpty \t\t\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 2. Get size in constructor set\t\t\t\tOK.");
		this->logFile.write(true, Info);
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
				"TEST 2. Get # elements in constructor set\t\t\tOK.");
		this->logFile.write(true, Info);
	}
}

void TestSet::run()
{
	int	i=0;								// Loop counter.
	int highest=0;
	int highestIndex=0;
	int updatedIndex=0;
	int *element;

	// Check is full and empty functions.
	checkDefaultConstructor();

	// Check is full and empty functions.
	checkCustomConstructor();

	cout << "PROBANDO" << endl;

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
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get size after adding element\t\t\t\t\t\t\tOK.");
			this->logFile.write( true, Info);
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
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get # elements after adding element\t\t\t\t\t\tOK.");
			this->logFile.write( true, Info);
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
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get size after adding element\t\t\t\t\t\t\tOK.");
			this->logFile.write( true, Info);
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
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 3. Get # elements after adding element\t\t\t\t\t\tOK.");
			this->logFile.write( true, Info);
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
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 4. At element \t\t\t\t\t\t\t\t\tOK.");
			this->logFile.write( true, Info);
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
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 5. isEmpty \t\t\t\t\t\t\t\t\tOK.");
		this->logFile.write( true, Info);
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
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 5. isFull \t\t\t\t\t\t\t\t\t\tOK.");
		this->logFile.write( true, Info);
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
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 6. isEmpty \t\t\t\t\t\t\t\t\tOK.");
		this->logFile.write( true, Info);
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
		this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 6. isFull \t\t\t\t\t\t\t\t\t\tOK.");
		this->logFile.write( true, Info);
	}

	// Check get highest element.
	for (i=0; i<DOUBLESIZE ;i++)
	{
		this->set.add(i+1);
	}

	highest = this->set.highest();
	highestIndex = this->set.highestIndex();
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
		highest = this->set.highest();
		highestIndex = this->set.highestIndex();
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
			this->logFile.buildText(__FUNCTION__, __FILE__, "TEST 7. highest \t\t\t\t\t\t\t\t\t\tOK.");
			this->logFile.write( true, Info);
		}
	}
}
