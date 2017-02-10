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

}

void Tester::main()
{
	/*
	string testSetFilename = "testSet2.txt";

	TestSet testSet;

	// Execute Set class tests.
	testSet.run();
	*/

	TestPath testPath;

	testPath.init();
	testPath.main();
	testPath.finish();

	TestVoronoi testVoronoi;

	testVoronoi.init();
	testVoronoi.main();
	testVoronoi.finish();
}

void Tester::finish()
{

}
