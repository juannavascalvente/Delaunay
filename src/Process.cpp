/*
 * Process.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#include <GL/glut.h>

#include "Process.h"
#include "Triangle.h"

//#define DEBUG_PROCESS_FIND_CLOSESTPOINT
//#define DEBUG_PROCESS_FIND_PATH

Process *Process::instance = 0;

typedef Point<TYPE> PointT;

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Process::Process(int argc, char **argv, bool printData)
{
	string 	fileName;			// Configuration file name.

	// Initialize configuration.
	fileName = argv[2];
	this->config = new Config(fileName);

	// Check flag to print data to screen.
	this->log = new Logging("log.txt", printData);

	// Initialize drawer and menu.
	this->drawer = this->drawer->getInstance(argc, argv, &this->dcel,
												&this->delaunay,
												&this->triangulation,
												&this->voronoi,
												&this->gabriel,
												&this->status,
												this->config);
	this->m = Menu(&this->status);

	// Function to execute by GLUT.
	glutDisplayFunc(executeWrapper);
}

Process::~Process()
{
	// Deallocate draw.
	delete this->config;
	delete this->log;
	// PENDING Really necessary to call destructors.
	if (this->status.isDelaunayCreated())
	{
		this->delaunay.~Delaunay();
	}

	if (this->status.isTriangulationCreated())
	{
		this->triangulation.~Triangulation();
	}

	if (this->status.isVoronoiCreated())
	{
		this->gabriel.~Gabriel();
	}
}


//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: start
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: starts infinite loop.
***************************************************************************/
void Process::start(void)
{
	// GLUT main function.
	glutMainLoop();
}


/***************************************************************************
* Name: setInstance
* IN:		process		instance to be executed by main loop.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	"instance" points to "process"
* Description: sets "instance" to the object that is going to be executed
* 				by the main loop process.
***************************************************************************/
void Process::setInstance(Process *process)
{
	instance = process;
}

/***************************************************************************
* Name: executeWrapper
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: wrapper to call "execute" function.
***************************************************************************/
void Process::executeWrapper()
{
	instance->execute();
}


/***************************************************************************
* Name: 	readData
* IN:		option				option selected in menu.
* OUT:		NONE
* RETURN:	true				if successfully read.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	read data from file or generate data set depending on input
* 				option. The data set can be generated:
* 				1.- Randomly uniform
* 				2.- Generated randomly in clusters
* 				3.- Read from flat file.
* 				4.- Read from DCEL.
* 				5.- Read Delaunay (DCEL + graph file).
* 				6.- Read voronoi data.
***************************************************************************/
bool Process::readData(int option)
{
	bool success=true; 		// Return value.

	// Check option to generate/read set.
	switch (option)
	{
		// Generate random set.
		case RANDOMLY:
		{
			success = this->dcel.generateRandom(this->config->getNPoints());
			this->status.set(false, true, false, false, false, false);
			break;
		}
		// Generate clusters set.
		case CLUSTER:
		{
			success = this->dcel.generateClusters(this->config->getNPoints(),
					this->config->getNClusters(), this->config->getRadius());
			this->status.set(false, true, false, false, false, false);
			break;
		}
		// Read set from flat file.
		case READ_POINTS_FILE:
		{
			success = this->dcel.readPoints(this->config->getInFlatFilename());
			this->status.set(false, true, false, false, false, false);
			break;
		}
		// Read dcel file.
		case READ_DCEL:
		{
			// PENDING CHECK IF A DCEL IS CONSISTENT?
			success = this->dcel.read(this->config->getInDCELFilename(), false);
			this->status.set(false, true, !success, false, false, false);
			break;
		}
		// Read Delaunay incremental algorithm files.
		case READ_DELAUNAY:
		{
			// PENDING CHECK IF A DCEL AND GRAPG ARE CONSISTENT?
			this->delaunay.setDCEL(&this->dcel);
			success = this->delaunay.read(this->config->getInDCELFilename(),
					this->config->getInGraphFilename());
			this->status.set(false, true, !success, !success, false, false);
			break;
		}
		// Read Voronoi file.
		case READ_VORONOI:
		{
			// PENDING: What to allow in menu if only voronoi is read.
			success = this->voronoi.read(this->config->getInVoronoiFilename());
			this->status.set(false, true, !success, !success, true, false);
			break;
		}
		// Read Gabriel file.
		default:
		{
			// PENDING: What to allow in menu if only voronoi is read.
			success = this->gabriel.readBinary(this->config->getOutGabrielFilename());
			this->status.setGabrielCreated(true);
			break;
		}
	}

	return(success);
}

/***************************************************************************
* Name: 	resetData
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	resets the data of the process class.
***************************************************************************/
void Process::resetData()
{
	// Check if voronoi must be reset.
	if (this->status.isVoronoiCreated())
	{
		this->voronoi.reset();
	}
}

/***************************************************************************
* Name: 	buildTriangulation
* IN:		option				option selected in menu.
* OUT:		NONE
* RETURN:	true				if successfully built.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	build a triangulation depending on input option:
* 				1.- STAR triangulation
* 				2.- Delaunay triangulation. If this option is selected, the
* 				triangulation can be computed from scratch or computed using
* 				and initial triangulation.
***************************************************************************/
bool Process::buildTriangulation(int option)
{
	bool built=true;		// Return value.

	// Check triangulation type.
	if (option == STAR_TRIANGULATION)
	{
		// Check if star triangulation already computed.
		if (!this->status.isTriangulationCreated())
		{
			this->triangulation.setDCEL(&this->dcel);
			built = this->triangulation.build();
			if (built)
			{
				status.set(false, true, true, false, false, false);
			}
		}
	}
	else
	{
		// Get reference to current DCEL.
		this->delaunay.setDCEL(&this->dcel);

		// Build Delaunay from Star triangulation.
		if (this->status.isTriangulationCreated())
		{
			built = this->triangulation.delaunay();
			this->delaunay.setAlgorithm(FROM_STAR);
		}
		else
		{
			// Build Delaunay from DCEL.
			if (!this->status.isDelaunayCreated())
			{
				built = this->delaunay.incremental();
			}
		}

		if (built)
		{
			status.set(false, true, true, true, false, false);
		}
	}

	return(built);
}

/***************************************************************************
* Name: 	buildConvexHull
* IN:		NONE
* OUT:		NONE
* RETURN:	true				if successfully built.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	build the convex hull of a set of points. The triangulation
* 				can be any triangulation or a Delaunay triangulation.
***************************************************************************/
bool Process::buildConvexHull()
{
	bool built=true;		// Return value.

	// Computing convex hull.
	if (this->status.isDelaunayCreated())
	{
		built = this->delaunay.convexHull();
	}
	else
	{
		built = this->triangulation.convexHull();
	}

	return(built);
}

/***************************************************************************
* Name: 	findPath
* IN:		delaunay			Delaunay triangulation data.
* 			voronoi				Voronoi data.
* 			line				line whose path must be searched.
* OUT:		faces				queue where points are inserted.
* RETURN:	true				if path found.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	finds the list of faces in the "dcel"between "line"
* 				extreme points.
***************************************************************************/
bool Process::findPath(Delaunay &delaunay, Voronoi &voronoi, Line &line, Set<int> &pathFaces)
{
	bool found=false;			// Return value.
	int	 initialFace=0;			// Initial face in the path.
	int	 finalFace=0;			// Final face in the path.
	Point<TYPE> origin, dest;	// Line extreme points.
	Point<TYPE> closest;		// Closest point.
	double distance=0.0;		// Distance between points.
	Set<int> extremeFaces(2);	// First and last faces in the path.

	// Get origin and destination points.
	origin = line.getOrigin();
	dest = line.getDest();

#ifdef DEBUG_PROCESS_FIND_PATH
	Logging::buildText(__FUNCTION__, __FILE__, "Line origin point is ");
	Logging::buildText(__FUNCTION__, __FILE__, &origin);
	Logging::buildText(__FUNCTION__, __FILE__, " and line destination point is ");
	Logging::buildText(__FUNCTION__, __FILE__, &dest);
	Logging::write(true, Info);
#endif

	// Get extreme point faces.
	if (delaunay.findClosestPoint(origin, voronoi, closest, initialFace, distance) &&
		delaunay.findClosestPoint(dest, voronoi, closest, finalFace, distance))
	{
		// Add faces to set.
		extremeFaces.add(initialFace+1);
		extremeFaces.add(finalFace+1);

#ifdef DEBUG_PROCESS_FIND_PATH
		Logging::buildText(__FUNCTION__, __FILE__, "Origin face is ");
		Logging::buildText(__FUNCTION__, __FILE__, *extremeFaces.at(0));
		Logging::buildText(__FUNCTION__, __FILE__, " and destination ");
		Logging::buildText(__FUNCTION__, __FILE__, *extremeFaces.at(1));
		Logging::write(true, Info);
#endif

		// Find path.
		found = voronoi.getRefDcel()->findPath(extremeFaces, line, pathFaces);
	}
#ifdef DEBUG_PROCESS_FIND_PATH
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error computing initial faces");
		Logging::write(true, Error);
	}
#endif

	return(found);
}

/***************************************************************************
* Name: 	findTwoClosest
* IN:		NONE
* OUT:		index1				index of the first point
* 			index2				index of the second point
* RETURN:	true				if found.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	finds the two closest points in the set of points. If the
* 				Delaunay triangulation exists the uses the incremental
* 				algorithm to locate the points. Otherwise uses a brute force
* 				algorithm.
***************************************************************************/
bool Process::findTwoClosest(int &index1, int &index2)
{
	bool found=true;		// Return value.

	// Check if Delaunay triangulation computed.
	// PENDING ALSO EXECUTES THIS IF THE DELAUNAY WAS BUILD FROM STAR? IF
	// SO THERE SHOULD BE NOT GRAPH AND IT IS NOT POSSIBLE TO USE THE GRAPH.
	if (this->status.isDelaunayCreated())
	{
		found = this->delaunay.findTwoClosest(index1, index2);
	}
	else
	{
		found = this->triangulation.findTwoClosest(index1, index2);
	}

	return(found);
}


/***************************************************************************
* Name: 	findTwoClosest
* IN:		index		index of the point whose face must be located.
* OUT:		faceId		face id that surrounds input index point.
* RETURN:	true		if found.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	finds the two closest points in the set of points. If the
* 				Delaunay triangulation exists the uses the incremental
* 				algorithm to locate the points. Otherwise uses a brute force
* 				algorithm.
***************************************************************************/
bool Process::findFace(Point<TYPE> &point, int &faceId)
{
	bool found=true;		// Return value.

	// Check if Delaunay triangulation computed.
	// PENDING ALSO EXECUTES THIS IF THE DELAUNAY WAS BUILD FROM STAR? IF
	// SO THERE SHOULD BE NOT GRAPH AND IT IS NOT POSSIBLE TO USE THE GRAPH.
	if (this->status.isDelaunayCreated())
	{
		found = this->delaunay.findFace(point, faceId);
	}
	else
	{
		found = this->triangulation.findFace(point, faceId);
	}

	return(found);
}


/***************************************************************************
* Name: 	findTwoClosest
* IN:		index		index of the point whose face must be located.
* OUT:		faceId		face id that surrounds input index point.
* RETURN:	true		if found.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	finds the two closest points in the set of points. If the
* 				Delaunay triangulation exists the uses the incremental
* 				algorithm to locate the points. Otherwise uses a brute force
* 				algorithm.
***************************************************************************/
bool Process::findClosest(Point<TYPE> &p, Point<TYPE> &q, double &distance)
{
	bool found=true;		// Return value.
	int	pointIndex=0;		// Index of the closest point.

	// Check if Delaunay triangulation computed.
	if (this->status.isDelaunayCreated())
	{
		// Find node that surrounds input point p.
		if (this->status.isVoronoiCreated())
		{
			found = this->delaunay.findClosestPoint(p, this->voronoi, q, pointIndex, distance);
		}
		else
		{
			printf("PENDING TO IMPLEMENT.\n");
			found = this->delaunay.findClosestPoint(p, this->config->getNAnchors(), q, distance);
		}
	}
	else
	{
		// Find closest using brute force.
		found = this->triangulation.findClosestPoint(p, q, distance);
	}

	return(found);
}




/***************************************************************************
* Name: 	getPointToLocate
* IN:		NONE
* OUT:		point		point to locate.
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	if a point was set in the configuration file then this
* 				method sets in the output parameter that point. Otherwise
* 				it is generated randomly.
***************************************************************************/
void Process::getPointToLocate(Point<TYPE> &point)
{
	int minX, minY, maxX, maxY;

	// Get point from configuration.
	point = config->getClosestPoint();

	// Check if input point parameter provided by user.
	if (point.getX() == INVALID)
	{
		// Get min and max coordiantes.
		config->getScreenCoordinates(minX, minY, maxX, maxY);

		// Generate seed.
		srand(time(NULL));

		// Create a random point.
		point.setX(rand() % (int) maxX);
		point.setY(rand() % (int) maxY);
	}
}



/***************************************************************************
* Name: execute
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	main loop that processes events from menu and executes the
* 				selected option and updates menu and internal status.
***************************************************************************/
void Process::execute(void)
{
	static bool firstTime=true;
	int		option=0;			// Option to be executed.
	bool	error=false;		// Error executing any function.
	bool	quit=false;			// Quit application flag.
	int		index1, index2;		// Index of the two closest points.

	// Get option to be executed.
	option = m.getMenuOption();

	// Execute option.
	switch(option)
	{
		// Read parameters from configuration file.
		case PARAMETERS:
		{
			// Read configuration file.
			this->config->readConfig();
			break;
		}
		// New set of points (generated or read).
		case RANDOMLY:
		case CLUSTER:
		case READ_POINTS_FILE:
		case READ_DCEL:
		case READ_DELAUNAY:
		case READ_VORONOI:
		case READ_GABRIEL:
		{
			this->resetData();

			// Check option to generate/read set.
			error = !this->readData(option);
			if (!error)
		    {
				if (this->status.isVoronoiCreated())
				{
					// Clear screen.
					this->drawer->drawFigures(VORONOI_DRAW);
				}
				if (this->status.isDelaunayCreated() ||
					this->status.isTriangulationCreated())
				{
					// Draw triangulation.
					this->drawer->drawFigures(TRIANGULATION_DRAW);
				}
				else
				{
					// Draw set of points.
					this->drawer->drawFigures(SET_DRAW);
				}

				// Update menu entries.
				m.updateMenu();
		    }
			break;
		}
		// Build STAR or DELAUNAY triangulation
		case STAR_TRIANGULATION:
		case DELAUNAY:
		{
			// Check if Delaunay already created.
			if (!this->status.isDelaunayCreated())
			{
				// Build triangulation.
				error = !this->buildTriangulation(option);
			}

			if (!error)
			{
                // Draw triangulation.
				this->drawer->drawFigures(TRIANGULATION_DRAW);

				// Update menu entries.
				m.updateMenu();
			}
			break;
		}
		// Build Voronoi diagram.
		case VORONOI:
		{
			// PENDING CALL VORONOI
			if (this->status.isDelaunayCreated())
			{
				// Initialize voronoi data.
				error = this->voronoi.init(&this->dcel);
				if (!error)
				{
					// Compute Voronoi diagram.
					error = this->voronoi.build();
				}

				// Check error and update status.
				if (!error)
				{
					//int minX, minY, maxX, maxY;
					//this->config->getScreenCoordinates(minX, minY, maxX, maxY);
					//this->voronoi.correctBorderPoints(minX, minY, maxX, maxY);

					// Draw Voronoi graph and the triangulation.
					this->drawer->drawFigures(VORONOI_DRAW);

					// Update execution status flags.
					status.set(false, true, true, true, true, false);

					// Update menu entries.
					m.updateMenu();
				}
			}
			break;
		}
		case GABRIEL:
		{
			if (this->status.isDelaunayCreated() &&
				this->status.isVoronoiCreated())
			{
				// Build Gabriel graph.
				this->gabriel.init(this->delaunay.getDCEL(), &this->voronoi);
				error = !this->gabriel.build();
				if (!error)
				{
					// Draw Voronoi graph and the triangulation.
					this->drawer->drawFigures(GABRIEL_DRAW);

					// Update execution status flags.
					status.set(false, true, true, true, true, true);

					// Update menu entries.
					m.updateMenu();
				}
			}
			break;
		}
		// Compute path between two points.
		case TRIANGULATION_PATH:
		{
			Point<TYPE> p1, p2;						// Segment points.
			Line line;								// Segment line.
			Set<PointT> points(1);				// List of points.
			Set<int> faces(DEFAULT_QUEUE_SIZE);		// Set of faces.

			// Check Delaunay triangulation already created.
			if (this->status.isDelaunayCreated())
			{
				// Generate random points.
				p1.random();
				p2.random();
				line = Line(p1, p2);

				// Compute triangles path between two points.
				error = !this->delaunay.findPath(line, faces);

				// Draw triangulation, segment and if no error, the path.
				points.add(p1);
				points.add(p2);
				this->drawer->setPointsSet(&points);
				this->drawer->setFacesSet(&faces);
				this->drawer->drawFigures(TRIANGULATION_PATH_DRAW, error);

				// Print error message.
				if (error)
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Error computing Delaunay path");
					Logging::write(true, Error);
				}
			}
			break;
		}
		// Compute Voronoi path between two points.
		case VORONOI_PATH:
		{
			Point<TYPE> p1, p2;						// Segment points.
			Line line;								// Segment line.
			Set<PointT> points(1);				// List of points.
			Set<int> faces(DEFAULT_QUEUE_SIZE);		// Set of faces.

			// Check Delaunay triangulation already created.
			if (this->status.isVoronoiCreated())
			{
				// Generate random points.
				p1.random();
				p2.random();
				line = Line(p1, p2);

				// Compute triangles path between two points.
				error = !this->findPath(this->delaunay, this->voronoi, line, faces);

				// Draw triangulation, Voronoi, segment and the path.
				points.add(p1);
				points.add(p2);
				this->drawer->setPointsSet(&points);
				this->drawer->setFacesSet(&faces);
				this->drawer->drawFigures(VORONOI_PATH_DRAW, error);

				// Check if an error must be printed.
				if (error)
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Error computing Voronoi path");
					Logging::write(true, Error);
				}
			}
			break;
		}
		// Compute convex hull.
		case CONVEX_HULL:
		{
			// Computing convex hull.
			error = !this->buildConvexHull();
			if (!error)
			{
                // Convex hull.
				this->drawer->drawFigures(CONVEXHULL_DRAW);
			}
			break;
		}
		// Find closest point to a given point.
		case CLOSEST_POINT:
		{
			Point<TYPE> point;			// Point to locate.
			Point<TYPE> closest;		// Closest point.
			double distance=0.0;		// Distance between point and closest.
			Set<PointT> points(2);	// List of points.

			// Check if input point parameter provided by user.
			this->getPointToLocate(point);

			// Find closest point to point.
			error = !this->findClosest(point, closest, distance);
			if (!error)
			{
				// Draw the triangulation, the point and the closest point.
				points.add(point);
				points.add(closest);
				this->drawer->setPointsSet(&points);
				this->drawer->drawFigures(CLOSESTPOINT_DRAW);
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error closest point to a given point");
				Logging::write(true, Error);
			}
			break;
		}
		// Locate face that surrounds input point.
		case FIND_FACE:
		{
			int faceId=0;				// Face that surrounds point.
			PointT point;				// Point to locate.
			Set<int> 	faces(1);		// List of faces.
			Set<PointT> points(1);	// List of points.

			// Check if input point parameter provided by user.
			this->getPointToLocate(point);

			// Find face.
			error = !this->findFace(point, faceId);
			if (!error)
			{
				// Draw the triangulation, the point and its face.
				points.add(point);
				this->drawer->setPointsSet(&points);
				faces.add(faceId);
				this->drawer->setFacesSet(&faces);
				this->drawer->drawFigures(FINDFACE_DRAW);
			}
			break;
		}
		// Find the two closest point in the set.
		case TWO_CLOSEST:
		{
			Set<PointT> points(2);	// List of points.

			// Compute the two closest point in the set of points.
			error = !this->findTwoClosest(index1, index2);
			if (!error)
			{
				// Draw the triangulation and the two closest points.
				points.add(*this->dcel.getRefPoint(index1));
				points.add(*this->dcel.getRefPoint(index2));
				this->drawer->setPointsSet(&points);
				this->drawer->drawFigures(TWOCLOSEST_DRAW);
			}
			break;
		}
		// Filter edges whose length is lower than a threshold.
		case FILTER_EDGES:
		{
			// Check if Delaunay triangulation already created.
			if (status.isTriangulationCreated())
			{
				// Draw triangulation filtering edges.
				this->drawer->drawFigures(FILTEREDGES_DRAW);
            }
			break;
		}
		// Draw triangles circumcentres.
		case CIRCUMCENTRES:
		{
			// Check if triangulation created.
			if (this->status.isDelaunayCreated() ||
				this->status.isTriangulationCreated())
			{
				this->drawer->drawFigures(CIRCUMCENTRES_DRAW);
			}
			break;
		}
		case EDGE_CIRCLES:
		{
			// Check if triangulation created.
			if (this->status.isTriangulationCreated())
			{
				this->drawer->drawFigures(EDGESCIRCLES_DRAW);
			}
			break;
		}
		// Print DCEL data.
		case DCEL_INFO:
		{
			this->drawer->drawFigures(DCEL_INFO_DRAW);
			break;
		}
		// Print Voronoi data.
		case VORONOI_INFO:
		{
			this->drawer->drawFigures(VORONOI_INFO_DRAW);
			break;
		}
		// Shake points in DCEL.
		case SHAKE_POINTS:
		{
			// Shake set of points.
			this->dcel.shake();

			// Draw set of points.
			this->drawer->drawFigures(SET_DRAW);

			break;
		}
		// Write points to a flat file.
		case WRITE_POINTS:
		{
			this->dcel.writePoints(this->config->getOutFlatFilename(), INVALID);
			break;
		}
		// Write points to a DCEL file.
		case WRITE_DCEL:
		{
			this->dcel.write(this->config->getOutDCELFilename(), false);
			break;
		}
		// Write DCEL and graph files.
		case WRITE_DELAUNAY:
		{
			this->delaunay.write(this->config->getOutDCELFilename(), this->config->getOutGraphFilename());
			break;
		}
		// Write voronoi DCEL file.
		case WRITE_VORONOI:
		{
			this->voronoi.write(this->config->getOutVoronoiFilename());
			break;
		}
		// Write Gabriel graph data.
		case WRITE_GABRIEL:
		{
			this->gabriel.writeBinary(this->config->getOutGabrielFilename());
			break;
		}
		// Clear data.
		case CLEAR:
		{
			// Update execution status flags.
			this->status.reset();

			// Update menu entries.
			this->m.updateMenu();

			// Clear screen.
			this->drawer->drawFigures(CLEAR_SCREEN);

			// Reset data.
			break;
		}
		// Quit application.
		case QUIT:
		{
			// Quit application.
			quit = true;
			break;
		}
		// Not implemented functionality.
		case CHECK_DCEL:
		case ZOOM:
		{
			std::cout << "Not implemented." << std::endl;
			break;
		}
		default:
		{
			if (firstTime)
			{
				// Clear screen.
				firstTime = false;
				this->drawer->drawFigures(CLEAR_SCREEN);
			}
			break;
		}
	}

	// Exit application.
	if (quit)
	{
		// PENDING. Anything to deallocate.
		delete this->log;
		if (this->status.isDelaunayCreated())
		{
			this->delaunay.~Delaunay();
		}
		exit(0);
	}
	else
	{
		// Reset menu option.
		m.resetMenuOption();
	}
}
