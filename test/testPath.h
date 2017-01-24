/*
 * testPath.h
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */

#ifndef TEST_TESTPATH_H_
#define TEST_TESTPATH_H_

#include "Point.h"
#include "Dcel.h"

/****************************************************************************
// 							TestPath CLASS DEFITNION
****************************************************************************/
class TestPath
{
	Logging log;			// Log file.

	void dump(string fileName, Point<TYPE> &p1, Point<TYPE> &p2, Dcel &dcel);

public:
	TestPath() : log("logTestPath.txt", false) {};
	~TestPath() {};

	void init();
	void main();
	void finish();
};

#endif /* TEST_TESTPATH_H_ */
/*dcel.generateRandom(this->config->getNPoints()); */
