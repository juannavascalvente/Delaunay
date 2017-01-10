/*
 * performance.h
 *
 *  Created on: Dec 19, 2016
 *      Author: juan
 */

#ifndef CODE_PERFORMANCE_PERFORMANCE_H_
#define CODE_PERFORMANCE_PERFORMANCE_H_

#include <string>
using namespace std;
#include <sys/time.h>
#include <time.h>

#include "Logging.h"

//****************************************************************************
//                          PERFORMANCETEST CLASS
//****************************************************************************
class PerformanceTest
{
	string inFile;
	string outFile;
	Logging logFile;
public:
	PerformanceTest(string in, string out, string logFile, bool printData) :
		inFile(in),	outFile(out), logFile(logFile, printData) {};
	~PerformanceTest() {};

	bool read() { return(true); };
	bool write() { return(true); };
	void run() {};

	const string& getInputFile() const {return inFile;}
	void setInputFile(const string& in) {this->inFile = in;}
	const string& getOuputFile() const {return outFile;}
	void setOuputFile(const string& out) {this->outFile = out;}
};


//****************************************************************************
//                           	PERFORMANCE CLASS
//****************************************************************************
class Performance
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	int testDelaunay(int nPoints, int nTests);

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Performance(){};
	~Performance(){};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void init();
	void run();
	void finish();
};

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
	inline void tic() {clock_gettime( CLOCK_REALTIME, &this->begin);};
	inline void toc() {clock_gettime( CLOCK_REALTIME, &this->end);};
	void getInterval();
	inline void average(int n) {this->avg = this->total / (double) n;};
	inline void reset() {this->total = 0.0;};
	inline void updateTotal() {this->total = this->total + this->diff;};

	// Getters/setters.
	double getTotal() const { return this->total; }
	double getAvg() const { return this->avg; }
};

/**************************************************************************************/
class DelaunayTest : public PerformanceTest
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int 	nExecutions;	// # tests to execute.
	int 	nPoints;		// # points in in set.
	int 	buildType;		// Type of algorithm to use building Delaunay.
	int 	testType;		// Type of tes to execute.
	Timer 	*timers;		// Performance timer.

	int nSteps;				// # steps between incremental number of points tests.
	int step;				// Step factor between number of points tests.

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	// Specific class functions.
	int testDelaunay();
	int testIncrementalDelaunay();
	int testFromStarDelaunay();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	DelaunayTest(string in, string out, string logFile, bool printData) :
						PerformanceTest(in, out, logFile, printData)
	{
		this->nExecutions = INVALID;
		this->nPoints = INVALID;
		this->buildType = INVALID;
		this->testType = INVALID;
		this->nSteps = INVALID;
		this->step = INVALID;
		this->timers = NULL;
	}
	~DelaunayTest(){};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool read();
	bool write();
	void run();
	void finish();
};


#endif /* CODE_PERFORMANCE_PERFORMANCE_H_ */
