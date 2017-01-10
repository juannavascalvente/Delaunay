/*
 * testSet.h
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_TESTSET_H_
#define CODE_TEST_TESTSET_H_

#include "Set.h"
#include "test.h"

class TestSet: public Test
{
	Set<int> set;

	void checkDefaultConstructor();
	void checkCustomConstructor();

public:
	TestSet() : Test("testSet.txt") {};
	TestSet(const string fileName) : Test(fileName) {};
	void run();
};

#endif /* CODE_TEST_TESTSET_H_ */
