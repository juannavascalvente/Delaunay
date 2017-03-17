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

#define MILLION	1000000
#define BILLION 1000000000L;

//****************************************************************************
//                           	TIMER CLASS
//****************************************************************************
class Timer
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	struct timespec begin;
	struct timespec end;
	double diff;
	double total;
	double avg;
public:
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Timer() : diff(0.0), total(0.0), avg(0.0) {};
	~Timer(){};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	inline void tic() {clock_gettime(CLOCK_REALTIME, &this->begin);};
	inline void toc() {clock_gettime(CLOCK_REALTIME, &this->end);};
	void getInterval();
	inline void average(int n) {this->avg = this->total / (double) n;};
	inline void reset() {this->total = 0.0;};
	inline void updateTotal() {this->total = this->total + this->diff;};

	// Getters/setters.
	double getTotal() const { return this->total; }
	double getAvg() const { return this->avg; }
};

/****************************************************************************
//	 						StatisticsData CLASS DEFITNION
****************************************************************************/
class StatisticsData
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int nPoints;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsData() : nPoints(0) {}
    ~StatisticsData() {}

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	int getPoints() const {return nPoints;}
	void setPoints(int points) {nPoints = points;}
};

/****************************************************************************
//	 				StatisticsConexHull CLASS DEFITNION
****************************************************************************/
class StatisticsConexHull : public StatisticsData
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int length;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsConexHull() : StatisticsData(), length(0) {}
    ~StatisticsConexHull() {}

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	int getLength() const {return length;}
	void setLength(int length) {this->length = length;}
};

/****************************************************************************
//	 						StatisticsTriangulation CLASS DEFITNION
****************************************************************************/
class StatisticsTriangulation : public StatisticsData
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int nEdges;
	int nFaces;
	int	nFlips;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsTriangulation() : StatisticsData(), nEdges(0), nFaces(0), \
								nFlips(0) {}
    ~StatisticsTriangulation() {}

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	int getEdges() const {return nEdges;}
	void setEdges(int edges) {nEdges = edges;}
	int getFaces() const {return nFaces;}
	void setFaces(int faces) {nFaces = faces;}
	int getFlips() const {return nFlips;}
	void setFlips(int flips) {nFlips = flips;}
};

/****************************************************************************
//	 						StatisticsDelaunay CLASS DEFITNION
****************************************************************************/
class StatisticsDelaunay : public StatisticsTriangulation
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int nCollinear;
	int n2children;
	int	n3children;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsDelaunay() : StatisticsTriangulation(), nCollinear(0), \
							n2children(0), n3children(0)  {}
    ~StatisticsDelaunay() {}

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	int getN2childrenNodes() const {return n2children;}
	void setN2childrenNodes(int n2children) {this->n2children = n2children;}
	int getN3childrenNodes() const {return n3children;}
	void setN3childrenNodes(int n3children) {this->n3children = n3children;}
	int getCollinear() const {return nCollinear;}
	void setCollinear(int collinear) {nCollinear = collinear;}
};


#endif /* INCLUDE_STATISTICS_H_ */
