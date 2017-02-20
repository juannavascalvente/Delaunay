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

	void prepare() { cout << "PREPARE TEST SET" << endl; };
	void setDefault() {cout << "SET DEFAULT TEST SET" << endl;};
	bool read(){cout << "READ TEST SET" << endl; return(true);};
	void write(){cout << "WRITE TEST SET" << endl;};
	void print(){};

public:
	TestSet() : Test("testSet.txt", true) {};
	void main();
};

#endif /* CODE_TEST_TESTSET_H_ */
