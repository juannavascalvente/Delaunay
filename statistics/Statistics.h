/*
 * statistics.h
 *
 *  Created on: Nov 17, 2016
 *      Author: juan
 */

#include <iostream>
#include <fstream>
using namespace std;
#include <sys/time.h>
#include <stdio.h>

#ifndef INCLUDE_STATISTICS_H_
#define INCLUDE_STATISTICS_H_

#define STATISCTICS_FINDCLOSEST

/****************************************************************************
//	 						STATISTICS CLASS DEFITNION
****************************************************************************/
class Statistics
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	static Statistics *instance;

	struct timeval begin;
	struct timeval end;
	struct timeval lapse;

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
    // Constructor and destructor
	Statistics() {}
    ~Statistics() {}
	struct timeval getTime(void);

public:
	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
    static Statistics* getInstance();

	void tic(void);
	void toc(void);
	void diff(void);
};


#endif /* INCLUDE_STATISTICS_H_ */
