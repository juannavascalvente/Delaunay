/*
 * TestPath.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */
#define DEBUG_FIND_DELAUNAY_PATH
#define DEBUG_FIND_VORONOI_PATH
#define DEFAULT_QUEUE_SIZE			20

#include "Line.h"
#include "Set.h"
#include "testPath.h"

#include <unistd.h>


/***************************************************************************
* Name: 	dump
* IN:		pointsFileName		points file name
* 			dcelFileName		dcel file name
* 			p1					first point
* 			p2					second point
* 			dcel				dcel data to write
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Writes to output files the data required to reproduce the fail
***************************************************************************/
void TestPathDelaunay::dump(string pointsFileName, string dcelFileName, \
								Point<TYPE> &p1, Point<TYPE> &p2, Dcel &dcel)
{
	ofstream ofs;			// Output file.

	// Open file.
	ofs.open(pointsFileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Points loop.
		ofs << p1 << endl;
		ofs << p2 << endl;

		// Close file.
		ofs.close();

		// Write DCEL data.
		if (!dcel.writePoints(dcelFileName, dcel.getNVertex()))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Cannot write to file: ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::write(true, Error);
		}
	}
	// Error opening points file.
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Cannot open file: ");
		Logging::buildText(__FUNCTION__, __FILE__, pointsFileName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	builds a Delaunay triangulation, create two random points
* 				and finds the set of faces of the triangulation from one
* 				point to the other. If the path is not found then it writes
* 				the points and the set of points of the triangulation.
***************************************************************************/
void TestPathDelaunay::main()
{
	int			i=0;				// Loop counter.
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	int			failedTestIndex=1;	// Index of last failed test.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	int			currentNPoints=0;
	Point<TYPE> p1, p2;				// Segment points.
	Line line;						// Segment line.
	Set<int> faces(DEFAULT_QUEUE_SIZE);		// Set of faces.
	string pointsFileName;			// Points file name.
	string dcelFileName;			// DCEL file name.

	// Print test parameters.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);

		for (testIndex=0; testIndex< this->nTests ;testIndex++)
		{
			// Execute current test.
			delaunay.setDCEL(&dcel);
			if (!dcel.generateRandom(currentNPoints))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Start Delaunay path test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);

				// Generate random points.
				p1.random();
				p2.random();
				line = Line(p1, p2);

				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				pointsFileName = this->outputFolder + "Points_" + convert.str() + ".txt";
				dcelFileName = this->outputFolder + "DCEL_" + convert.str() + ".txt";
				this->dump(pointsFileName, dcelFileName, p1, p2, dcel);
				if (delaunay.incremental())
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Start find path");
					Logging::write( true, Info);

					// Compute triangles path between two points.
					if (delaunay.findPath(line, faces))
					{
						// Remove files because test did not fail.
						remove(pointsFileName.c_str());
						remove(dcelFileName.c_str());

						// Print test results
#ifdef DEBUG_FIND_DELAUNAY_PATH
						Logging::buildText(__FUNCTION__, __FILE__, "Faces in the path:");
						for (i=0; i<faces.getNElements() ;i++)
						{
							Logging::buildText(__FUNCTION__, __FILE__, " ");
							Logging::buildText(__FUNCTION__, __FILE__, *faces.at(i));
						}
						Logging::write( true, Info);
#endif
						Logging::buildText(__FUNCTION__, __FILE__, "Test OK Id: ");
						Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
						Logging::write( true, Info);
					}
					else
					{
						failedTestIndex++;
						Logging::buildText(__FUNCTION__, __FILE__, \
										"Line does not intersect set of points");
						Logging::write( true, Info);
					}
				}
				else
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay");
					Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
					Logging::write(true, Error);
				}

				// Reset Delaunay data.
				sleep(1);
				delaunay.reset();
			}
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}
}

/***************************************************************************
* Name: 	dump
* IN:		pointsFileName		points file name
* 			dcelFileName		dcel file name
* 			p1					first point
* 			p2					second point
* 			dcel				dcel data to write
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Writes to output files the data required to reproduce the fail
***************************************************************************/
void TestPathVoronoi::dump(string pointsFileName, string dcelFileName, Point<TYPE> &p1, Point<TYPE> &p2, Dcel &dcel)
{
	ofstream ofs;			// Output file.

	// Open file.
	ofs.open(pointsFileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Points loop.
		ofs << p1 << endl;
		ofs << p2 << endl;

		// Close file.
		ofs.close();

		// Write DCEL data.
		if (!dcel.writePoints(dcelFileName, dcel.getNVertex()))
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Cannot write to file: ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::write(true, Error);
		}
	}
	// Error opening points file.
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Cannot open file: ");
		Logging::buildText(__FUNCTION__, __FILE__, pointsFileName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	builds a Voronoi diagram, create two random points
* 				and finds the set of faces of the diagram from one
* 				point to the other. If the path is not found then it writes
* 				the points and the set of points of the ------------------------------------------------------------------------.
***************************************************************************/
void TestPathVoronoi::main()
{
	int			i=0;				// Loop counter.
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	Voronoi		voronoi;			// Voronoi data.
	int			failedTestIndex=1;	// Index of last failed test.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	int			currentNPoints=0;
	Point<TYPE> p1, p2;				// Segment points.
	Line line;						// Segment line.
	Point<TYPE> closest;		// Closest point.
	double distance=0.0;		// Distance between points.
	int	 initialFace=0;			// Initial face in the path.
	int	 finalFace=0;			// Final face in the path.
	Set<int> faces(DEFAULT_QUEUE_SIZE);		// Set of faces.
	Set<int> extremeFaces(DEFAULT_QUEUE_SIZE);		// Set of faces.
	Set<int> pathFaces;
	string pointsFileName;			// Points file name.
	string dcelFileName;			// DCEL file name.

	// Print test parameters.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);

		for (testIndex=0; testIndex< this->nTests ;testIndex++)
		{
			// Execute current test.
			delaunay.setDCEL(&dcel);
			if (!dcel.generateRandom(currentNPoints))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Start Voronoi path test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);

				// Generate random points.
				p1.random();
				p2.random();
				line = Line(p1, p2);

				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				pointsFileName = this->outputFolder + "Points_" + convert.str() + ".txt";
				dcelFileName = this->outputFolder + "DCEL_" + convert.str() + ".txt";
				this->dump(pointsFileName, dcelFileName, p1, p2, dcel);

				// Create Delaunay triangulation.
				if (delaunay.incremental())
				{
					// Initialize Voronoi.
					if (!voronoi.init(&dcel))
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Error initializing Voronoi.");
						Logging::write(true, Error);
					}
					// Build Voronoi.
					else if (!voronoi.build())
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Error building Voronoi.");
						Logging::write(true, Error);
					}
					else
					{
#ifdef DEBUG_FIND_VORONOI_PATH
						Logging::buildText(__FUNCTION__, __FILE__, "Locating initial faces");
						Logging::write(true, Info);
#endif
						extremeFaces.reset();
						if (delaunay.findClosestPoint(p1, voronoi, closest, initialFace, distance) &&
							delaunay.findClosestPoint(p2, voronoi, closest, finalFace, distance))
						{
							// Add faces to set.
							extremeFaces.add(initialFace+1);
							extremeFaces.add(finalFace+1);
#ifdef DEBUG_FIND_VORONOI_PATH
							Logging::buildText(__FUNCTION__, __FILE__, "Computing Voronoi path");
							Logging::write(true, Info);
#endif
							// Compute triangles path between two points.
							pathFaces.reset();
							if (voronoi.getRefDcel()->findPath(extremeFaces, line, pathFaces))
							{
								// Remove files because test did not fail.
								remove(pointsFileName.c_str());
								remove(dcelFileName.c_str());

								// Print test results
#ifdef DEBUG_FIND_VORONOI_PATH
								Logging::buildText(__FUNCTION__, __FILE__, "Faces in the path:");
								for (i=0; i<pathFaces.getNElements() ;i++)
								{
									Logging::buildText(__FUNCTION__, __FILE__, " ");
									Logging::buildText(__FUNCTION__, __FILE__, *pathFaces.at(i));
								}
								Logging::write( true, Info);
#endif
								Logging::buildText(__FUNCTION__, __FILE__, "Test OK Id: ");
								Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
								Logging::write( true, Info);
							}
							else
							{
								failedTestIndex++;
								Logging::buildText(__FUNCTION__, __FILE__, "Error computing Voronoi path in iteration ");
								Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
								Logging::write( true, Info);
							}
						}
					}
				}
				else
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay");
					Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
					Logging::write(true, Error);
				}

				// Reset Delaunay data.
				sleep(1);
				delaunay.reset();
				voronoi.reset();
			}
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}
}

