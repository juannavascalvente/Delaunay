/*
 * Assert.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: juan
 */

#include "assert.h"


// Check two values are equal.
bool Assert::assertEqual(int value1, int value2)
{
	return (value1 == value2);
}

// Check two values are not equal.
bool Assert::assertNotEqual(int value1, int value2)
{
	return (value1 != value2);
}
