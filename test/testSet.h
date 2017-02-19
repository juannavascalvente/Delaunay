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

	void prepare() {};
	void setDefault() {};
	bool read(){return(true);};
	void main(){};
	void write(){};
	void print(){};

public:
	TestSet() : Test("testSet.txt", true) {};
	void run();
};

#endif /* CODE_TEST_TESTSET_H_ */
