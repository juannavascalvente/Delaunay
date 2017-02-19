/*
 * test.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#include "tester.h"
#include "test.h"
#include "testSet.h"
#include "testPath.h"
#include "testVoronoi.h"

void Tester::init()
{
	//this->read();
}

void Tester::main()
{
	/*
	string testSetFilename = "testSet2.txt";
*/
	Test *test;
	TestSet testSet;
	TestPath testPath;

	test = &testPath;

	//test->run();

	// Execute Set class tests.
	testSet.run();
/*
	TestVoronoi testVoronoi;

	testVoronoi.init();
	testVoronoi.main();
	testVoronoi.finish();*/
}

void Tester::finish()
{

}
