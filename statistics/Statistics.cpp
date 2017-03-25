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
double Timer::getInterval()
{
	this->diff = (this->end.tv_sec - this->begin.tv_sec) +
				 (this->end.tv_nsec - this->begin.tv_nsec)/(double) BILLION;
	return(this->diff);
}

/***************************************************************************
* NAME: 	writeResults
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	write two arrays: the number of points in test and the number
* 				of edges of the convex hull per test.
***************************************************************************/
bool StatisticsConvexHullRegister::writeResults()
{
	bool written=false;		// Return value.
	int	 i=0;				// Loop counter.
	ConexHullStatisticsData *current=NULL;

	// Check file is opened.
	if (this->ofs.is_open())
	{
		if (this->data.getNElements() > 0)
		{
			// Write # points in test data.
			current = *this->data.at(0);
			this->ofs << "nPoints <- c(" << current->getPoints();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getPoints();
			}
			this->ofs << ")" << endl;

			// Write # edges in convex hull.
			current = *this->data.at(0);
			this->ofs << "nPoints <- c(" << current->getLength();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getLength();
			}
			this->ofs << ")" << endl;

			// Write execution times.
			current = *this->data.at(0);
			this->ofs << "nPoints <- c(" << current->getExecTime();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				// Write # points in test data.
				current = *this->data.at(i);
				this->ofs << "," << current->getExecTime();
			}
			this->ofs << ")" << endl;

			// Update return value.
			written = true;
		}
		else
		{
			this->ofs << "No statistics computed." << endl;
		}
	}

	return(written);
}

void StatisticsDelaunay::analyzeDelaunay(Delaunay &delaunay, int index)
{
	int		i=0;			// Loop counter.
/*
	// Get # points.
	this->nPoints[index] = delaunay.getRefDcel()->getNVertex();

	// Analyze edges.
	for (i=0; i<delaunay.getRefDcel()->getNEdges() ;i++)
	{
		if (delaunay.getRefDcel()->hasNegativeVertex(i+1))
		{
			this->nEdges++;
		}
		else
		{
			this->nImaginaryEdges++;
		}
	}

	// Divide by 2 because some edges are twin.
	this->nEdges = this->nEdges / 2;
	this->nImaginaryEdges = this->nImaginaryEdges / 2;

	// Compute # edges the convex hull.
	if (delaunay.convexHull())
	{
		this->nConvexhullEdges = delaunay.getConvexHullEdges()->getNElements();
		this->nConvexhullEdges = this->nConvexhullEdges*2;
	}
	else
	{
		cout << "Error computing convex hull" << endl;
	}

	// Analyze faces.
	for (i=0; i<delaunay.getRefDcel()->getNFaces() ;i++)
	{
		if (delaunay.getRefDcel()->imaginaryFace(i))
		{
			this->nFaces++;
		}
		else
		{
			this->nImaginaryFaces++;
		}
	}*/

	// Analyze graph.
	//this->analyzeGtaph(delaunay.getGraph());
}



