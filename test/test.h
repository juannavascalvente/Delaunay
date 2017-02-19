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
	void init();
	void finish();

	virtual void prepare() {};
	virtual void setDefault() {};
	virtual bool read() {return(true);};
	virtual void main() {};
	virtual void write() {};
	virtual void print() {};

protected:
	Logging logFile;		// Test log file.

public:
	Test() {};
	Test(const string logFilename, bool printData = false) : logFile(logFilename, printData) {};
	virtual ~Test() {};

	void run();
};

#endif /* CODE_TEST_TEST_H_ */
