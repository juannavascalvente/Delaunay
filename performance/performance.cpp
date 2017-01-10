/*
 * performance.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: juan
 */

#include "defines.h"
#include "Dcel.h"
#include "Delaunay.h"
#include "performance.h"
#include "Triangulation.h"

#define MILLION	1000000
#define BILLION 1000000000L;

//#define DEBUG_DELAUNAYTEST_READ
//#define DEBUG_DELAUNAYTEST_WRITE

#define TEST_TYPE_SPECIFIC_NPOINTS			1
#define TEST_TYPE_DELAUNAY_INCREMENTAL		2
#define TEST_TYPE_DELAUNAY_FROM_STAR		3
#define TEST_TYPE_DELAUNAY_BOTH				4

/***************************************************************************
* NAME: 	read
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	read test configuration parameters.
***************************************************************************/
bool DelaunayTest::read()
{
	bool read=true;		// Return value.
	ifstream ifs;		// Input file.

#ifdef DEBUG_DELAUNAYTEST_READ
	Logging::buildText(__FUNCTION__, __FILE__, "Opening file: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getInputFile());
	Logging::write(true, Info);
#endif

	// Open file.
	ifs.open(this->getInputFile().c_str(), ios::in);
	if (ifs.is_open())
	{
		// Get test parameters.
		ifs >> this->nExecutions;
		ifs >> this->nPoints;
		ifs >> this->buildType;
		ifs >> this->testType;
		ifs >> this->nSteps;
		ifs >> this->step;

		// Check test type.
		switch(this->testType)
		{
			case TEST_TYPE_SPECIFIC_NPOINTS:
			{
				// Compute average time.
				this->timers = new Timer();
				break;
			}
			case TEST_TYPE_DELAUNAY_INCREMENTAL:
			case TEST_TYPE_DELAUNAY_FROM_STAR:
			case TEST_TYPE_DELAUNAY_BOTH:
			{
				this->timers = new Timer[this->nSteps];
				break;
			}
			default:
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Unknown type of test.");
				Logging::write(true, Error);
				break;
			}
		}

		// Close file.
		ifs.close();
	}
	// Error opening file.
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->getInputFile());
		Logging::write(true, Error);
		read = false;
	}

#ifdef DEBUG_DELAUNAYTEST_READ
	Logging::buildText(__FUNCTION__, __FILE__, "File read successfully.");
	Logging::write(true, Info);
#endif

	return(read);
}

/***************************************************************************
* NAME: 	write
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	write test results.
***************************************************************************/
bool DelaunayTest::write()
{
	bool written=true;	// Return value.
	ofstream ofs;		// Output file.
	int	i=0;			// Loop counter.
	int	newNPoints=0;	// # points in triangulation.

#ifdef DEBUG_DELAUNAYTEST_WRITE
	Logging::buildText(__FUNCTION__, __FILE__, "Opening file: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getOuputFile());
	Logging::write(true, Info);
#endif
	// Open file.
	ofs.open(this->getOuputFile().c_str(), ios::out);
	if (ofs.is_open())
	{
		switch(this->testType)
		{
			case TEST_TYPE_SPECIFIC_NPOINTS:
			{
				// Compute average time.
				this->timers[0].average(this->nExecutions);

				ofs << "Number of tests " << this->nExecutions << endl;
				ofs << "Average time " << this->timers[0].getAvg() << " seconds" << endl;
				break;
			}
			case TEST_TYPE_DELAUNAY_INCREMENTAL:
			case TEST_TYPE_DELAUNAY_FROM_STAR:
			{
				newNPoints = this->nPoints;

				ofs << "----------------------------------------" << endl;
				ofs << "General information" << endl;
				ofs << "Number of tests " << this->nExecutions << endl;
				ofs << "----------------------------------------" << endl;

				// Run input # of tests.
				for (i=0; i<this->nSteps ;i++)
				{
					// Compute average time.
					this->timers[i].average(this->nExecutions);

					// Build Delunay triangulation.
					ofs << "TEST " << (i+1);
					ofs << ".\tNumber of points: " << newNPoints;
					ofs << ".\tAvg time: " ;
					ofs << this->timers[i].getAvg() << endl;
					newNPoints = newNPoints*this->step;
				}
				ofs << "----------------------------------------" << endl;
				ofs << "R data" << endl;
				ofs << "----------------------------------------" << endl;
				ofs << "# points array:" << endl;
				newNPoints = this->nPoints;
				ofs << "nPointsArray <- c(";
				ofs << newNPoints;
				for (i=1; i<this->nSteps ;i++)
				{
					newNPoints = newNPoints*this->step;
					ofs << "," << newNPoints;
				}
				ofs << ")" << endl;
				ofs << "timesArray <- c(";
				ofs << this->timers[0].getAvg();
				for (i=1; i<this->nSteps ;i++)
				{
					ofs << "," << this->timers[i].getAvg();
				}
				ofs << ")" << endl;
				break;
			}
			case TEST_TYPE_DELAUNAY_BOTH:
			{
				break;
			}
			default:
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Unkonwn type of test.");
				Logging::write(true, Error);
				break;
			}
		}

		// Close file.
		ofs.close();
	}
	// Error opening file.
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->getOuputFile());
		Logging::write(true, Error);
		written = false;
	}

	return(written);
}

/***************************************************************************
* NAME: 	run
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	run tests
***************************************************************************/
void DelaunayTest::run()
{
	switch(this->testType)
	{
		case TEST_TYPE_SPECIFIC_NPOINTS:
		{
			this->testDelaunay();
			break;
		}
		// Incremental Delaunay test.
		case TEST_TYPE_DELAUNAY_INCREMENTAL:
		{
			this->testIncrementalDelaunay();
			break;
		}
		// Delaunay from start triangulation test.
		case TEST_TYPE_DELAUNAY_FROM_STAR:
		{
			this->testFromStarDelaunay();
			break;
		}
		default:
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Unknown type of test.");
			Logging::write(true, Error);
			break;
		}
	}
}

/***************************************************************************
* NAME: 	finish
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	deallocate test resources.
***************************************************************************/
void DelaunayTest::finish()
{
	// Check test type.
	switch(this->testType)
	{
		case TEST_TYPE_SPECIFIC_NPOINTS:
		{
			// Compute average time.
			delete this->timers;
			break;
		}
		case TEST_TYPE_DELAUNAY_INCREMENTAL:
		case TEST_TYPE_DELAUNAY_FROM_STAR:
		case TEST_TYPE_DELAUNAY_BOTH:
		{
			delete[] this->timers;
			break;
		}
		default:
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Unknown type of test.");
			Logging::write(true, Error);
			break;
		}
	}
}

/***************************************************************************
* NAME: 	testDelaunay
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	builds the "nTests" Delaunay triangulations using nPoints and
* 				computes the average execution time.
***************************************************************************/
int DelaunayTest::testDelaunay()
{
	int ret=SUCCESS;		// Return value.
	int	i=0;				// Loop counter.
	Dcel		dcel;		// Triangulation data.
	Delaunay	delaunay;	// Delaunay triangulation.

	// Initialize data.
	dcel.resize( this->nPoints, false);
	delaunay.setDCEL( &dcel);

	// Run input # of tests.
	for (i=0; i<this->nExecutions ;i++)
	{
		cout << "Test " << (i+1) << endl;

		// Generate random set of points.
		dcel.generateRandom( this->nPoints);

		// Build Delunay triangulation.
		this->timers[0].tic();
		delaunay.incremental();
		this->timers[0].toc();
		this->timers[0].getInterval();
		this->timers[0].updateTotal();
	}

	return(ret);
}

/***************************************************************************
* NAME: 	testDelaunay
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	builds the "nTests" Delaunay triangulations using nPoints and
* 				computes the average execution time.
***************************************************************************/
int DelaunayTest::testIncrementalDelaunay()
{
	int ret=SUCCESS;		// Return value.
	int	i=0,j=0;			// Loop counters.
	Dcel		dcel;		// Triangulation data.
	Delaunay	delaunay;	// Delaunay triangulation.
	int	newNPoints=0;

	// Initialize data.
	newNPoints = this->nPoints;
	delaunay.setDCEL( &dcel);

	// Run input # of tests.
	for (i=0; i<this->nSteps ;i++)
	{
		cout << "STEP " << (i+1) << "/" << this->nSteps << " with number of points " << newNPoints << endl;
		for (j=0; j<this->nExecutions ;j++)
		{
			cout << "Test " << (j+1) << "/" << this->nExecutions << endl;

			// Generate random set of points.
			dcel.generateRandom( newNPoints);

			// Build Delunay triangulation.
			this->timers[i].tic();
			delaunay.incremental();
			this->timers[i].toc();
			this->timers[i].getInterval();
			this->timers[i].updateTotal();
		}
		this->timers[i].average(this->nExecutions);
		newNPoints = newNPoints*this->step;
		dcel.resize( newNPoints, false);
	}

	return(ret);
}


/***************************************************************************
* NAME: 	testFromStarDelaunay
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	builds the "nTests" Delaunay triangulations using a star
* 				triangulation using nPoints and computes the average
* 				execution time.
***************************************************************************/
int DelaunayTest::testFromStarDelaunay()
{
	int ret=SUCCESS;				// Return value.
	int	i=0,j=0;					// Loop counters.
	Dcel		dcel;				// Triangulation data.
	Triangulation	triangulation;	// Star triangulation data.
	int	newNPoints=0;

	cout << "Delaunay from start triangulation tests" << endl;

	// Initialize data.
	newNPoints = this->nPoints;
	triangulation.setDCEL(&dcel);

	// Run input # of tests.
	for (i=0; i<this->nSteps ;i++)
	{
		cout << "STEP " << (i+1) << "/" << this->nSteps << " with number of points " << newNPoints << endl;
		for (j=0; j<this->nExecutions ;j++)
		{
			cout << "Test " << (j+1) << "/" << this->nExecutions << endl;

			// Generate random set of points.
			dcel.generateRandom( newNPoints);

			// Build Delunay triangulation.
			this->timers[i].tic();
			triangulation.build();
			triangulation.delaunay();
			this->timers[i].toc();
			this->timers[i].getInterval();
			this->timers[i].updateTotal();
		}
		this->timers[i].average(this->nExecutions);
		newNPoints = newNPoints*this->step;
		dcel.resize( newNPoints, false);
	}

	return(ret);
}


/******************************************************************************/
void Performance::init()
{

}

void Performance::run()
{
	DelaunayTest delaunayTest("delaunayIn.txt", "delaunayOut.txt", "logDelaunay.txt,", true);

	delaunayTest.read();
	delaunayTest.run();
	delaunayTest.write();
	delaunayTest.finish();
}

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

