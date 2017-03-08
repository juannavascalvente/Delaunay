/*
 * TestPath.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: juan
 */
//#define DEBUG_FIND_DELAUNAY_PATH
//#define DEBUG_FIND_DELAUNAY_PATH_COMPARE
//#define DEBUG_FIND_VORONOI_PATH
//#define DEBUG_FIND_VORONOI_PATH_COMPARE
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
	int			totalTests=0;		// Total # of tests.
	int			testCounter=0;
#ifdef DEBUG_FIND_DELAUNAY_PATH
	int			i=0;				// Loop counter.
#endif

	testCounter = 1;
	totalTests = this->nSteps*this->nTests;

	// Print test parameters.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
#ifdef DEBUG_FIND_DELAUNAY_PATH
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);
#endif

		for (testIndex=0; testIndex<this->nTests ;testIndex++)
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
#ifdef DEBUG_FIND_DELAUNAY_PATH
				Logging::buildText(__FUNCTION__, __FILE__, "Start Delaunay path test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);
#endif
				// Generate random points.
				p1.random();
				p2.random();
				line = Line(p1, p2);

				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				pointsFileName = this->outFolder + "Points_" + convert.str() + ".txt";
				dcelFileName = this->outFolder + "DCEL_" + convert.str() + ".txt";
				this->dump(pointsFileName, dcelFileName, p1, p2, dcel);
				if (delaunay.incremental())
				{
#ifdef DEBUG_FIND_DELAUNAY_PATH
					Logging::buildText(__FUNCTION__, __FILE__, "Start find path");
					Logging::write( true, Info);
#endif
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
#endif
						Logging::write( true, Info);
						Logging::buildText(__FUNCTION__, __FILE__, "Test OK Id: ");
						Logging::buildText(__FUNCTION__, __FILE__, testCounter);
						Logging::buildText(__FUNCTION__, __FILE__, "/");
						Logging::buildText(__FUNCTION__, __FILE__, totalTests);
						Logging::write( true, Info);
					}
					else
					{
						failedTestIndex++;
						Logging::buildText(__FUNCTION__, __FILE__, \
										"Line does not intersect set of points");
						Logging::write( true, Error);
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

		// Update test counter.
		testCounter++;
	}
}

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	compares the Delaunay path read from input files with the
* 				Delaunay path computed using the points in the input files
***************************************************************************/
void TestPathDelaunayCompare::main()
{
	int 	 i=0;				// Loop counter.
	int		 nFailedTests=0;	// # tests failed.
	string 	 dcelFileName;		// DCEL file name.
	string 	 pointsFileName;	// Points file name.
	string 	 facesFileName;		// Faces file name.
	Dcel	 dcel;				// Dcel data.
	Delaunay delaunay;			// Delaunay data.
	Set<int> originalFacesSet;	// Original set of faces.
	Set<int> facesSet;			// Computed set of faces.
	Set<Point<TYPE> > originalPointsSet;	// Original set of faces.
	Point<TYPE> p1, p2;				// Segment points.
	Line line;						// Segment line.
	int testIndex=0;
	int			totalTests=0;		// Total # of tests.
	int			testCounter=0;

	if ((filesList.getNElements() % 3) == 0)
	{
		testCounter = 1;
		totalTests = (filesList.getNElements()/3);

		// Print test parameters.
		this->printParameters();

#ifdef DEBUG_FIND_DELAUNAY_PATH_COMPARE
		Logging::buildText(__FUNCTION__, __FILE__, "Number of files to compare: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
		Logging::write(true, Info);
#endif
		// Read all files.
		for (i=0; i<this->filesList.getNElements() ;i=i+3)
		{
			// Open file.
			dcelFileName = *this->filesList.at(i);
			pointsFileName = *this->filesList.at(i+1);
			facesFileName = *this->filesList.at(i+2);
#ifdef DEBUG_FIND_DELAUNAY_PATH_COMPARE
			Logging::buildText(__FUNCTION__, __FILE__, "Files to check are: ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, pointsFileName);
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, facesFileName);
			Logging::write(true, Info);
#endif
			if (!dcel.readPoints(dcelFileName, false))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading original file: ");
				Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
				Logging::write(true, Error);
			}
			else if (!originalPointsSet.read(pointsFileName))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading set of points: ");
				Logging::buildText(__FUNCTION__, __FILE__, pointsFileName);
				Logging::write(true, Error);
			}
			else if (!originalFacesSet.read(facesFileName))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading set of faces: ");
				Logging::buildText(__FUNCTION__, __FILE__, facesFileName);
				Logging::write(true, Error);
			}
			else
			{
#ifdef DEBUG_FIND_DELAUNAY_PATH_COMPARE
				Logging::buildText(__FUNCTION__, __FILE__, "Start execution");
				Logging::write(true, Info);
#endif
				// Reset dcel to remove all data except point coordinates.
				dcel.clean();

				// Execute current test.
				delaunay.setDCEL(&dcel);
				if (!delaunay.incremental())
				{
					nFailedTests++;
					Logging::buildText(__FUNCTION__, __FILE__, "Cannot create Delaunay. Test id: ");
					Logging::buildText(__FUNCTION__, __FILE__, i+1);
					Logging::write(true, Error);
				}
				else
				{
					p1 = *originalPointsSet.at(0);
					p2 = *originalPointsSet.at(1);
					line = Line(p1, p2);
#ifdef DEBUG_FIND_DELAUNAY_PATH_COMPARE
					Logging::buildText(__FUNCTION__, __FILE__, "Searching path");
					Logging::write(true, Info);
#endif
					// Compute triangles path between two points.
					testIndex++;
					if (delaunay.findPath(line, facesSet))
					{
						// Compare faces sets.
						if (facesSet == originalFacesSet)
						{
							Logging::buildText(__FUNCTION__, __FILE__, "Test OK Id: ");
							Logging::buildText(__FUNCTION__, __FILE__, testCounter);
							Logging::buildText(__FUNCTION__, __FILE__, "/");
							Logging::buildText(__FUNCTION__, __FILE__, totalTests);
							Logging::write( true, Info);
						}
						else
						{
							nFailedTests++;
							Logging::buildText(__FUNCTION__, __FILE__, "Faces sets are not equal. Test id: ");
							Logging::buildText(__FUNCTION__, __FILE__, testCounter);
							Logging::buildText(__FUNCTION__, __FILE__, "/");
							Logging::buildText(__FUNCTION__, __FILE__, totalTests);
							Logging::write(true, Error);
							originalFacesSet.print();
							facesSet.print();
						}
					}
					else
					{
						nFailedTests++;
						Logging::buildText(__FUNCTION__, __FILE__, \
								"Line does not intersect set of points. Test id: ");
						Logging::buildText(__FUNCTION__, __FILE__, testIndex);
						Logging::write( true, Error);
					}
				}
			}

			// Reset data.
			delaunay.reset();
			dcel.reset();
			originalPointsSet.reset();
			originalFacesSet.reset();
			facesSet.reset();
			testCounter++;
		}
		Logging::buildText(__FUNCTION__, __FILE__, "Tests executed successfully ");
		Logging::buildText(__FUNCTION__, __FILE__, testIndex-nFailedTests);
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, testIndex);
		Logging::write(true, Info);
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "# files must be mulitple of three and is ");
		Logging::buildText(__FUNCTION__, __FILE__, filesList.getNElements());
		Logging::write(true, Error);
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
* 				the points and the set of points of the DCEL.
***************************************************************************/
void TestPathVoronoi::main()
{
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
	int			totalTests=0;		// Total # of tests.
	int			testCounter=0;
#ifdef DEBUG_FIND_VORONOI_PATH
	int			i=0;				// Loop counter.
#endif

	testCounter = 1;
	totalTests = this->nSteps*this->nTests;

	// Print test parameters.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
#ifdef DEBUG_FIND_VORONOI_PATH
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);
#endif
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
#ifdef DEBUG_FIND_VORONOI_PATH
				Logging::buildText(__FUNCTION__, __FILE__, "Start Voronoi path test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);
#endif
				// Generate random points.
				p1.random();
				p2.random();
				line = Line(p1, p2);

				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				pointsFileName = this->outFolder + "Points_" + convert.str() + ".txt";
				dcelFileName = this->outFolder + "DCEL_" + convert.str() + ".txt";
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
								Logging::buildText(__FUNCTION__, __FILE__, testCounter);
								Logging::buildText(__FUNCTION__, __FILE__, "/");
								Logging::buildText(__FUNCTION__, __FILE__, totalTests);
								Logging::write( true, Info);
							}
							else
							{
								failedTestIndex++;
								Logging::buildText(__FUNCTION__, __FILE__, "Error computing Voronoi path in iteration ");
								Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
								Logging::write( true, Error);
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

		// Update test counter.
		testCounter++;
	}
}


/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	compares the Delaunay path read from input files with the
* 				Delaunay path computed using the points in the input files
***************************************************************************/
void TestPathVoronoiCompare::main()
{
	int 	 i=0;				// Loop counter.
	int		 nFailedTests=0;	// # tests failed.
	string 	 dcelFileName;		// DCEL file name.
	string 	 pointsFileName;	// Points file name.
	string 	 facesFileName;		// Faces file name.
	Dcel	 dcel;				// Dcel data.
	Delaunay delaunay;			// Delaunay data.
	Voronoi  voronoi;			// Voronoi data.
	Set<int> originalFacesSet;	// Original set of faces.
	Set<int> facesSet;			// Computed set of faces.
	Set<Point<TYPE> > originalPointsSet;	// Original set of faces.
	Point<TYPE> p1, p2;				// Segment points.
	Line line;						// Segment line.
	Point<TYPE> closest;		// Closest point.
	int	 initialFace=0;			// Initial face in the path.
	int	 finalFace=0;			// Final face in the path.
	double distance=0.0;		// Distance between points.
	Set<int> pathFaces;
	int			totalTests=0;		// Total # of tests.
	int			testCounter=0;

	testCounter = 1;
	totalTests = filesList.getNElements() / 3;

	// Print test parameters.
	this->printParameters();

	if ((filesList.getNElements() % 3) == 0)
	{
#ifdef DEBUG_FIND_VORONOI_PATH_COMPARE
		Logging::buildText(__FUNCTION__, __FILE__, "Number of files to compare: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
		Logging::write(true, Info);
#endif
		// Read all files.
		for (i=0; i<this->filesList.getNElements() ;i=i+3)
		{
			// Open file.
			dcelFileName = *this->filesList.at(i);
			pointsFileName = *this->filesList.at(i+1);
			facesFileName = *this->filesList.at(i+2);
#ifdef DEBUG_FIND_VORONOI_PATH_COMPARE
			Logging::buildText(__FUNCTION__, __FILE__, "Files to check are: ");
			Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, pointsFileName);
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, facesFileName);
			Logging::write(true, Info);
#endif
			if (!dcel.readPoints(dcelFileName, false))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading original file: ");
				Logging::buildText(__FUNCTION__, __FILE__, dcelFileName);
				Logging::write(true, Error);
			}
			else if (!originalPointsSet.read(pointsFileName))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading set of points: ");
				Logging::buildText(__FUNCTION__, __FILE__, pointsFileName);
				Logging::write(true, Error);
			}
			else if (!originalFacesSet.read(facesFileName))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error reading set of faces: ");
				Logging::buildText(__FUNCTION__, __FILE__, facesFileName);
				Logging::write(true, Error);
			}
			else
			{
#ifdef DEBUG_FIND_VORONOI_PATH_COMPARE
				Logging::buildText(__FUNCTION__, __FILE__, "Start execution");
				Logging::write(true, Info);
#endif
				// Reset dcel to remove all data except point coordinates.
				dcel.clean();

				// Execute current test.
				delaunay.setDCEL(&dcel);
				if (!delaunay.incremental())
				{
					nFailedTests++;
					Logging::buildText(__FUNCTION__, __FILE__, "Cannot create Delaunay. Test id: ");
					Logging::buildText(__FUNCTION__, __FILE__, i+1);
					Logging::write(true, Error);
				}
				// Initialize Voronoi.
				else if (!voronoi.init(&dcel))
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
					// Get points from file.
					p1 = *originalPointsSet.at(0);
					p2 = *originalPointsSet.at(1);
					line = Line(p1, p2);

#ifdef DEBUG_FIND_VORONOI_PATH_COMPARE
					Logging::buildText(__FUNCTION__, __FILE__, "Locating initial faces");
					Logging::write(true, Info);
#endif
					if (delaunay.findClosestPoint(p1, voronoi, closest, initialFace, distance) &&
						delaunay.findClosestPoint(p2, voronoi, closest, finalFace, distance))
					{
						// Add faces to set.
						facesSet.add(initialFace+1);
						facesSet.add(finalFace+1);
#ifdef DEBUG_FIND_VORONOI_PATH_COMPARE
						Logging::buildText(__FUNCTION__, __FILE__, "Computing Voronoi path");
						Logging::write(true, Info);
#endif
						// Compute triangles path between two points.
						if (voronoi.getRefDcel()->findPath(facesSet, line, pathFaces))
						{
							if (pathFaces == originalFacesSet)
							{
								Logging::buildText(__FUNCTION__, __FILE__, "Test OK Id: ");
								Logging::buildText(__FUNCTION__, __FILE__, testCounter);
								Logging::buildText(__FUNCTION__, __FILE__, "/");
								Logging::buildText(__FUNCTION__, __FILE__, totalTests);
								Logging::write( true, Info);
							}
							else
							{
								nFailedTests++;
								Logging::buildText(__FUNCTION__, __FILE__, "Faces sets are not equal. Test id: ");
								Logging::buildText(__FUNCTION__, __FILE__, testCounter);
								Logging::buildText(__FUNCTION__, __FILE__, "/");
								Logging::buildText(__FUNCTION__, __FILE__, totalTests);
								Logging::write(true, Error);
								originalFacesSet.print();
								pathFaces.print();
							}
						}
						else
						{
							nFailedTests++;
							Logging::buildText(__FUNCTION__, __FILE__, "Error computing Voronoi path in test ");
							Logging::buildText(__FUNCTION__, __FILE__, testCounter);
							Logging::buildText(__FUNCTION__, __FILE__, "/");
							Logging::buildText(__FUNCTION__, __FILE__, totalTests);
							Logging::write( true, Error);
						}
					}
				}

				// Reset data.
				delaunay.reset();
				dcel.reset();
				voronoi.reset();
				originalPointsSet.reset();
				originalFacesSet.reset();
				facesSet.reset();
				pathFaces.reset();
				testCounter++;
			}
		}
#ifdef DEBUG_FIND_VORONOI_PATH_COMPARE
		Logging::buildText(__FUNCTION__, __FILE__, "Tests executed successfully ");
		Logging::buildText(__FUNCTION__, __FILE__, totalTests-nFailedTests);
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, totalTests);
		Logging::write(true, Info);
#endif
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "# files must be mulitple of three and is ");
		Logging::buildText(__FUNCTION__, __FILE__, filesList.getNElements());
		Logging::write(true, Error);
	}
}

