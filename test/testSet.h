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

class TestSet : public Test
{
	Set<int> set;

	void checkDefaultConstructor();
	void checkCustomConstructor();

	bool prepare() {return(true);};
	void setDefault() {};
	bool parseParameters(Set<Label> &labels) {return(true);};
	void write() {};
	void print() {};

public:
	TestSet() : Test("logTestSet.txt", false) {};
	void main();
};

#endif /* CODE_TEST_TESTSET_H_ */
