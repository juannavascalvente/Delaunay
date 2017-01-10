/*
 * test.h
 *
 *  Created on: Dec 7, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_TEST_H_
#define CODE_TEST_TEST_H_

#include <string>
using namespace std;

#include "Logging.h"

/****************************************************************************
// 							Test CLASS DEFITNION
****************************************************************************/
class Test
{
protected:
	Logging logFile;		// Test log file.

public:
	Test() {};
	Test(const string logFilename) : logFile(logFilename, false) {};
	virtual ~Test() {};

	virtual void init() {};
	virtual void main() {};
	virtual void finish() {};
};

#endif /* CODE_TEST_TEST_H_ */
