#include "Delaunay.h"
#include "Logging.h"
#include "Set.h"
#include "StarTriangulation.h"

#include <iostream>
#include <fstream>
using namespace std;
#include <ctime>
#include <cstdio>

#ifndef INCLUDE_STATISTICS_H_
#define INCLUDE_STATISTICS_H_

#define DEFAULT_N_STATISTICS			100

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
	double getInterval();
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
	int 	nPoints;
	double  execTime;

public:
	StatisticsData() : nPoints(0), execTime(0.0) {};
	virtual ~StatisticsData() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	virtual bool writeResults() {return (true);}

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	double getExecTime() const {return execTime;}
	int getPoints() const {return nPoints;}
	void setExecTime(double execTime) {this->execTime = execTime;}
	void setPoints(int points) {nPoints = points;}
};

/****************************************************************************
//	 				StatisticsConexHull CLASS DEFITNION
****************************************************************************/
class ConexHullStatisticsData : public StatisticsData
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int length;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	ConexHullStatisticsData() : length(0) {};
    ~ConexHullStatisticsData() {};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	int getLength() const {return length;}
	void setLength(int length) {this->length = length;}
};

/****************************************************************************
//	 			StatisticsTriangulation CLASS DEFITNION
****************************************************************************/
class StatisticsTriangulationData : public StatisticsData
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
protected:
	int nEdges;
	int	nConvexhullEdges;
	int nFaces;
	int	nFlips;
	int nCollinear;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsTriangulationData() : nEdges(0), nConvexhullEdges(0), nFaces(0), \
								nFlips(0), nCollinear(0) {}
    virtual ~StatisticsTriangulationData() {}

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
    void analyzeTriangulation(StarTriangulation	&triangulation);

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	int getEdges() const {return nEdges;}
	void setEdges(int edges) {nEdges = edges;}
	int getFaces() const {return nFaces;}
	void setFaces(int faces) {nFaces = faces;}
	int getFlips() const {return nFlips;}
	void setFlips(int flips) {nFlips = flips;}
	int getConvexhullEdges() const {return nConvexhullEdges;}
	int getCollinear() const {return nCollinear;}
};

/****************************************************************************
//	 				StatisticsDelaunay CLASS DEFITNION
****************************************************************************/
class StatisticsDelaunayData : public StatisticsTriangulationData
{
protected:
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int	nLeaves;
	int n2children;
	int	n3children;
	int nImaginaryEdges;
	int nImaginaryFaces;
	int	nNodes;
	int	*nNodesChecked;

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsDelaunayData() : nLeaves(0), n2children(0), n3children(0), \
						   nImaginaryEdges(0), nImaginaryFaces(0), nNodes(0),
						   nNodesChecked(NULL) {}
    ~StatisticsDelaunayData() {delete[] nNodesChecked;}

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
    void analyzeDelaunay(Delaunay &delaunay);
    void analyzeGraph(Graph &graph);

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
    int getN2childrenNodes() const {return n2children;}
	int getN3childrenNodes() const {return n3children;}
	int getImaginaryEdges() const {return nImaginaryEdges;}
	int getImaginaryFaces() const {return nImaginaryFaces;}
	int getNodes() const {return nNodes;}
	int getLeaves() const {return nLeaves;}
	int* getNodesChecked() const {return nNodesChecked;}
	void setNodesChecked(int* nodesChecked) {nNodesChecked = nodesChecked;}
};

/****************************************************************************
//	 				StatisticsRegister CLASS DEFITNION
****************************************************************************/
class StatisticsRegister
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
protected:
	Timer		timer;				// Execution timer.
	string  	fileName;			// Output statistics file.
	ofstream 	ofs;				// Output stream.
	int			nSteps;				// # steps in execution.
	int			nTestsStep;		// # executions per step.

public:
	//------------------------------------------------------------------------
	// Constructor and destructor
	//------------------------------------------------------------------------
	StatisticsRegister(string outFile, int nSteps, int nTestsPerStep) :		\
							fileName(outFile), nSteps(0), nTestsStep(0)  	\
	{
		// Open file.
		ofs.open(fileName.c_str(), ios::out);

		// Check file is opened.
		if (!ofs.is_open())
		{
			// Error opening points file.
			Logging::buildText(__FILE__,__FUNCTION__,
						"Error opening statistics file " + this->fileName);
			Logging::write(true, Error);
		}
	};
    virtual ~StatisticsRegister()
    {
    	// Check file is already opened.
		if (ofs.is_open())
		{
			// Close file.
			ofs.close();
		}
    };

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
    void tic() {this->timer.tic();};
	void toc() {this->timer.toc();};
	double getLapse() {return(this->timer.getInterval());};
	virtual bool writeResults() {return(true);};
	virtual void deallocate() { cout << "PARENT " << endl;};
};

/****************************************************************************
//	 			StatisticsConvexHullRegister CLASS DEFITNION
****************************************************************************/
class StatisticsConvexHullRegister : public StatisticsRegister
{
	Set<ConexHullStatisticsData *>	data;	// Test statistics array.
public:
	StatisticsConvexHullRegister(string outFile, int nSteps, int nTestsStep):\
							StatisticsRegister(outFile, nSteps, nTestsStep), \
							data(DEFAULT_N_STATISTICS) {};
	~StatisticsConvexHullRegister(){this->deallocate();};

    void add(ConexHullStatisticsData *data) {this->data.add(data);};
    bool writeResults();
    void deallocate();
};

/****************************************************************************
//	 			StatisticsStarTriangulationRegister CLASS DEFITNION
****************************************************************************/
class StatisticsStarTriangulationRegister : public StatisticsRegister
{
	Set<StatisticsTriangulationData *>	data;	// Test statistics array.
public:
	StatisticsStarTriangulationRegister(string outFile, int nSteps, int nTestsStep): \
							StatisticsRegister(outFile, nSteps, nTestsStep),\
							data(DEFAULT_N_STATISTICS) {};
	~StatisticsStarTriangulationRegister(){this->deallocate();};

    void add(StatisticsTriangulationData *data) {this->data.add(data);};
    bool writeResults();
    void deallocate();
};

/****************************************************************************
//	 			StatisticsDelaunayRegister CLASS DEFITNION
****************************************************************************/
class StatisticsDelaunayRegister : public StatisticsRegister
{
	Set<StatisticsDelaunayData *>	data;	// Test statistics array.
public:
	StatisticsDelaunayRegister(string outFile, int nSteps, int nTestsStep): \
							StatisticsRegister(outFile, nSteps, nTestsStep),\
							data(DEFAULT_N_STATISTICS) {};
	~StatisticsDelaunayRegister(){this->deallocate();};

    void add(StatisticsDelaunayData *data) {this->data.add(data);};
    bool writeResults();
    void deallocate();
};

#endif /* INCLUDE_STATISTICS_H_ */
