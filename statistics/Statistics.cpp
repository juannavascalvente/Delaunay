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
			this->ofs << "nEdges <- c(" << current->getLength();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getLength();
			}
			this->ofs << ")" << endl;

			// Write execution times.
			current = *this->data.at(0);
			this->ofs << "times <- c(" << current->getExecTime();
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

/***************************************************************************
* NAME: 	deallocate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	delete statistics data vector elements.
***************************************************************************/
void StatisticsConvexHullRegister::deallocate()
{
	int	 i=0;				// Loop counter.

	// Delete statistics registers.
	for (i=0; i<this->data.getNElements() ;i++)
	{
		delete *this->data.at(i);
	}
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
bool StatisticsDelaunayRegister::writeResults()
{
	bool written=false;		// Return value.
	int	 i=0;				// Loop counter.
	StatisticsDelaunayData *current=NULL;

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

			// Write # edges in test data.
			current = *this->data.at(0);
			this->ofs << "nEdges <- c(" << current->getEdges();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getEdges();
			}
			this->ofs << ")" << endl;

			// Write # imaginary edges in test data.
			current = *this->data.at(0);
			this->ofs << "nImaginaryEdges <- c(" << current->getImaginaryEdges();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getImaginaryEdges();
			}
			this->ofs << ")" << endl;

			// Write # convex hull edges in test data.
			current = *this->data.at(0);
			this->ofs << "nConvexHullEdges <- c(" << current->getConvexhullEdges();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getConvexhullEdges();
			}
			this->ofs << ")" << endl;

			// Write # faces in test data.
			current = *this->data.at(0);
			this->ofs << "nFaces <- c(" << current->getFaces();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getFaces();
			}
			this->ofs << ")" << endl;

			// Write # imaginary faces in test data.
			current = *this->data.at(0);
			this->ofs << "nImaginaryFaces <- c(" << current->getImaginaryFaces();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getImaginaryFaces();
			}
			this->ofs << ")" << endl;

			// Write # nodes in test data.
			current = *this->data.at(0);
			this->ofs << "nNodes <- c(" << current->getNodes();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getNodes();
			}
			this->ofs << ")" << endl;

			// Write # nodes with 3 children in test data.
			current = *this->data.at(0);
			this->ofs << "n3childrenNodes <- c(" << current->getN3childrenNodes();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getN3childrenNodes();
			}
			this->ofs << ")" << endl;

			// Write # nodes with 2 children in test data.
			current = *this->data.at(0);
			this->ofs << "n2childrenNodes <- c(" << current->getN2childrenNodes();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getN2childrenNodes();
			}
			this->ofs << ")" << endl;

			// Write # flipped edges in test data.
			current = *this->data.at(0);
			this->ofs << "nFlipped <- c(" << current->getFlips();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getFlips();
			}
			this->ofs << ")" << endl;

			// Write # collinear points in test data.
#ifdef INCREMENTAL_DELAUNAY_STATISTICS
			current = *this->data.at(0);
			this->ofs << "nCollinearPoints <- c(" << current->getCollinear();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				current = *this->data.at(i);
				this->ofs << "," << current->getCollinear();
			}
			this->ofs << ")" << endl;
#endif
			// Write # leaves in graph.
			current = *this->data.at(0);
			this->ofs << "nLeaves <- c(" << current->getLeaves();
			for (i=1; i<this->data.getNElements() ;i++)
			{
				// Write # points in test data.
				current = *this->data.at(i);
				this->ofs << "," << current->getLeaves();
			}
			this->ofs << ")" << endl;

			// Write execution times.
			current = *this->data.at(0);
			this->ofs << "times <- c(" << current->getExecTime();
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

/***************************************************************************
* NAME: 	deallocate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	delete statistics data vector elements.
***************************************************************************/
void StatisticsDelaunayRegister::deallocate()
{
	int	 i=0;				// Loop counter.

	// Delete statistics registers.
	for (i=0; i<this->data.getNElements() ;i++)
	{
		delete *this->data.at(i);
	}
}

/***************************************************************************
* NAME: 	analyzeDelaunay
* IN:		delaunay		delaunay data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	analyzes a DCEL built using the incremental Delaunay
* 				algorithm.
***************************************************************************/
void StatisticsDelaunayData::analyzeDelaunay(Delaunay &delaunay)
{
	int		i=0;			// Loop counter.

	// Get # points.
	this->setPoints(delaunay.getRefDcel()->getNVertex());

	// Analyze edges.
	for (i=0; i<delaunay.getRefDcel()->getNEdges() ;i++)
	{
		if (delaunay.getRefDcel()->hasNegativeVertex(i+1))
		{
			//cout << "IMAGINARY ";
			this->nImaginaryEdges++;
		}
		else
		{
			//cout << "REAL ";
			this->nEdges++;
		}
		//delaunay.getRefDcel()->getRefEdge(i)->print(std::cout);
	}

	// Divide by 2 because some edges are twin.
	this->nEdges = this->nEdges / 2;
	this->nImaginaryEdges = this->nImaginaryEdges / 2;

	// Compute # edges the convex hull.
	if (delaunay.convexHull())
	{
		this->nConvexhullEdges = delaunay.getConvexHullEdges()->getNElements();
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
			this->nImaginaryFaces++;
		}
		else
		{
			this->nFaces++;
		}
	}

	// Get # collinear points found in algorithm execution.
#ifdef INCREMENTAL_DELAUNAY_STATISTICS
	this->nCollinear = delaunay.getCollinear();
#endif
	// Analyze graph.
	this->analyzeGraph(*delaunay.getGraph());
}

/***************************************************************************
* NAME: 	analyzeGraph
* IN:		graph		graph data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	analyzes a graph built using the incremental Delaunay
* 				algorithm.
***************************************************************************/
void StatisticsDelaunayData::analyzeGraph(Graph &graph)
{
	int i=0;		// Loop counter.

	// Get # nodes.
	this->nNodes = graph.getNElements();

	// Check all nodes.
	for (i=0; i<graph.getNElements() ;i++)
	{
		// Check # children in current node.
		if (graph.getNChildren(i) == 3)
		{
			this->n3children++;
		}
		else if (graph.getNChildren(i) == 2)
		{
			this->n2children++;
		}
		else
		{
			this->nLeaves++;
		}
	}

	// Compute # flipped edges.
	this->setFlips(this->n2children / 2);
}

