/*
 * testVoronoi.h
 *
 *  Created on: Feb 8, 2017
 *      Author: juan
 */

#ifndef TEST_TESTVORONOI_H_
#define TEST_TESTVORONOI_H_

using namespace std;

/****************************************************************************
// 							TestVoronoi CLASS DEFITNION
****************************************************************************/
class TestVoronoi
{
	Logging log;			// Log file.
	void dump(string fileName, Dcel &dcel);

public:
	TestVoronoi() : log("logTestVoronoi.txt", false) {};
	~TestVoronoi() {};

	void init();
	void main();
	void finish();
};



#endif /* TEST_TESTVORONOI_H_ */
