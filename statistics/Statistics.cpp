/*
 * Performance.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: juan
 */
#include "Statistics.h"
#include <sys/time.h>

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/*void Timer::diff(void)
{
	  // Perform the carry for the later subtraction by updating y.
	  if (Performance::begin.tv_usec < Performance::end.tv_usec)
	  {
		  int nsec = (Performance::end.tv_usec - Performance::begin.tv_usec) / 1000000 + 1;
		  Performance::end.tv_usec -= 1000000 * nsec;
		  Performance::end.tv_sec += nsec;
	  }
	  if (Performance::begin.tv_usec - Performance::end.tv_usec > 1000000)
	  {
		  int nsec = (Performance::begin.tv_usec - Performance::end.tv_usec) / 1000000;
		  Performance::end.tv_usec += 1000000 * nsec;
		  Performance::end.tv_sec -= nsec;
	  }

	  // Compute the time remaining to wait. tv_usec is certainly positive.
	  Performance::lapse.tv_sec = Performance::begin.tv_sec - Performance::end.tv_sec;
	  Performance::lapse.tv_usec = Performance::begin.tv_usec - Performance::end.tv_usec;
}
struct timeval Timer::getTime()
{
    struct timeval time;
    gettimeofday(&time, 0);

    return(time);
}
*/
/***************************************************************************
* NAME: 	getInterval
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: computes the time lapse between tic and toc functions.
***************************************************************************/
void Timer::getInterval()
{
	this->diff = (this->end.tv_sec - this->begin.tv_sec) +
				 (this->end.tv_nsec - this->begin.tv_nsec)/(double) BILLION;
}
