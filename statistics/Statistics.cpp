/*
 * Statistics.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: juan
 */
#include "Statistics.h"

#include <sys/time.h>

// Internal static variables.
Statistics *Statistics::instance=NULL;

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
Statistics* Statistics::getInstance()
{
	if (!instance)
	{
		instance = new Statistics();
	}

	return(instance);
}

void Statistics::tic(void)
{
	this->begin = this->getTime();
}

void Statistics::toc(void)
{
	this->end = this->getTime();
}

void Statistics::diff(void)
{
	  // Perform the carry for the later subtraction by updating y.
	  if (Statistics::begin.tv_usec < Statistics::end.tv_usec)
	  {
		  int nsec = (Statistics::end.tv_usec - Statistics::begin.tv_usec) / 1000000 + 1;
		  Statistics::end.tv_usec -= 1000000 * nsec;
		  Statistics::end.tv_sec += nsec;
	  }
	  if (Statistics::begin.tv_usec - Statistics::end.tv_usec > 1000000)
	  {
		  int nsec = (Statistics::begin.tv_usec - Statistics::end.tv_usec) / 1000000;
		  Statistics::end.tv_usec += 1000000 * nsec;
		  Statistics::end.tv_sec -= nsec;
	  }

	  // Compute the time remaining to wait. tv_usec is certainly positive.
	  Statistics::lapse.tv_sec = Statistics::begin.tv_sec - Statistics::end.tv_sec;
	  Statistics::lapse.tv_usec = Statistics::begin.tv_usec - Statistics::end.tv_usec;
}

//------------------------------------------------------------------------
// Private functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	incremental
* IN:		node		node whose area must be computed
* OUT:		NONE
* RETURN:	true		if triangulation built
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	computes the Delaunay triangulation using the incremental
* 				algorithm.
***************************************************************************/
struct timeval Statistics::getTime()
{
    struct timeval time;
    gettimeofday(&time, 0);

    return(time);
}
