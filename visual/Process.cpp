/*
 * Process.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Process.h"
#include "Command.h"
#include "CommandFactory.h"
#include "DcelFigureBuilder.h"
#include "DcelGenerator.h"
#include "DcelReader.h"
#include "DcelWriter.h"
#include "DelaunayIO.h"
#include "DisplayableFactory.h"
#include "GabrielIO.h"
#include "MenuOption.h"
#include "VoronoiIO.h"

#include <GL/glut.h>


/***********************************************************************************************************************
* Static members
***********************************************************************************************************************/
Process *Process::instance = nullptr;
typedef Point<TYPE> PointT;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Process::Process(int argc, char **argv, bool printData, StoreService *storeServiceIn)
{
	string 	fileName;			// Configuration file name.

	// Initialize configuration.
	fileName = argv[2];
	Config::readConfig(fileName);

	// Check flag to print data to screen.
	this->log = new Logging("log.txt", printData);

    storeService = storeServiceIn;
    dispManager = new DisplayManager(argc, argv);

	this->m = Menu(&this->status);

	// Function to execute by GLUT.
	glutDisplayFunc(executeWrapper);
}


Process::~Process()
{
	// Deallocate draw.
	delete this->log;
	delete this->dispManager;
	
	// PENDING Really necessary to call destructors.
	if (this->status.isDelaunayCreated())
	{
		this->delaunay.~Delaunay();
	}

	if (this->status.isTriangulationCreated())
	{
		this->triangulation.~StarTriangulation();
	}

	if (this->status.isVoronoiCreated())
	{
		this->gabriel.~Gabriel();
	}

	delete storeService;
}


void Process::start()
{
	// GLUT main function.
	glutMainLoop();
}


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
	bool isSuccess; 		// Return value.

//	// Check option to generate/read set.
//	switch (option)
//	{
//		// Generate random set.
//		case RANDOMLY:
//		{
//            isSuccess = DcelGenerator::generateRandom(Config::getNPoints(), this->dcel);
//            this->status.set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Generate clusters set.
//		case CLUSTER:
//		{
//            isSuccess = DcelGenerator::generateClusters(Config::getNPoints(), Config::getNClusters(), Config::getRadius(), this->dcel);
//			this->status.set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Read set from flat file.
//		case READ_POINTS_FLAT_FILE:
//		case READ_POINTS_DCEL_FILE:
//		{
//			// Read points from flat file.
//			if(option == READ_POINTS_FLAT_FILE)
//			{
//                isSuccess = DcelReader::readPoints(Config::getInFlatFilename(), true, this->dcel);
//			}
//			// Read points from DCEL file.
//			else
//			{
//                isSuccess = DcelReader::readPoints(Config::getInDCELFilename(), false, this->dcel);
//			}
//			this->status.set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Read dcel file.
//		case READ_DCEL:
//		{
//			// PENDING CHECK IF A DCEL IS CONSISTENT?
//			success = DcelReader::read(Config::getInDCELFilename(), false, this->dcel);
//			this->delaunay.setDCEL(&this->dcel);
//			this->status.set(false, true, true, false, false, false);
//			break;
//		}
//		// Read Delaunay incremental algorithm files.
//		case READ_DELAUNAY:
//		{
//			// PENDING CHECK IF A DCEL AND GRAPG ARE CONSISTENT?
//			this->delaunay.setDCEL(&this->dcel);
//			success = DelaunayIO::read(Config::getInDCELFilename(),
//                                       Config::getInGraphFilename(), this->delaunay);
//			this->status.set(false, success, success, success, false, false);
//			this->delaunay.setAlgorithm(INCREMENTAL);
//			break;
//		}
//		// Read Voronoi file.
//		case READ_VORONOI:
//		{
//			// PENDING: What to allow in menu if only voronoi is read.
//			//success = this->voronoi.read(Config::getInVoronoiFilename());
//			//this->status.set(false, true, !success, !success, true, false);
//			cout << "NOT IMPLEMENTED YET" << endl;
//			break;
//		}
//		// Read Gabriel file.
//		default:
//		{
//			// PENDING: What to allow in menu if only voronoi is read.
//			success = GabrielIO::readBinary(Config::getOutGabrielFilename(), this->gabriel);
//			this->status.setGabrielCreated(true);
//			break;
//		}
//        default:
//        {
//            // PENDING: What to allow in menu if only voronoi is read.
//            isSuccess = false;
//            break;
//        }
//	}
//
//	// Add figure display.
//    vector<Point<TYPE>> vPoints;
//    for (size_t i=0; i< Config::getNPoints(); i++)
//    {
//        vPoints.push_back(*this->dcel.getRefPoint(i));
//    }
//    dispManager->add(DisplayableFactory::createPointsSet(vPoints));

	return isSuccess;
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
	// Check if Delaunay must be reset.
	if (this->status.isDelaunayCreated())
	{
		this->delaunay.reset();
	}
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
			built = this->triangulation.build(storeService->getDcel());
			if (built)
			{
				status.set(false, true, true, false, false, false);
			}
		}
	}
	else
	{
		// Get reference to current DCEL.
		this->delaunay.setDCEL(storeService->getDcel());

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
	bool built;		// Return value.

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
	bool found;		// Return value.

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


/**
 * @fn      findFace
 * @brief   Find the face where the input point falls into
 *
 * @param   point       (IN)    Point to find
 * @param   faceId      (OUT)   Face where point falls into
 * @param   isImaginary (OUT)   Flag for imaginary faces (incremental algorithm)
 *
 * @return  true if point found
 *          false otherwise
 */
bool Process::findFace(Point<TYPE> &point, int &faceId, bool &isImaginary)
{
	bool found;		// Return value.

	// Check if Delaunay triangulation computed.
	// PENDING ALSO EXECUTES THIS IF THE DELAUNAY WAS BUILD FROM STAR? IF
	// SO THERE SHOULD BE NOT GRAPH AND IT IS NOT POSSIBLE TO USE THE GRAPH.
	if (this->status.isDelaunayCreated())
	{
		found = this->delaunay.findFace(point, faceId, isImaginary);
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
	bool found=false;		    // Return value.
	int	pointIndex=0;	// Index of the closest point.

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
			//found = this->delaunay.findClosestPoint(p, Config::getNAnchors(), q, distance);
		}
	}
	else
	{
		// Find closest using brute force.
		found = this->triangulation.findClosestPoint(p, q, distance);
	}

	return found;
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
	point = Config::getClosestPoint();

	// Check if input point parameter provided by user.
	if (point.getX() == INVALID)
	{
		// Get min and max coordiantes.
		Config::getScreenCoordinates(minX, minY, maxX, maxY);

		// Generate seed.
		srand(time(nullptr));

		// Create a random point.
		point.setX(rand() % (int) maxX);
		point.setY(rand() % (int) maxY);
	}
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
void Process::getLineToLocate(Point<TYPE> &p1, Point<TYPE> &p2)
{
	int minX, minY, maxX, maxY;

	// Get point from configuration.
	p1 = Config::getOriginPoint();
	p2 = Config::getDestinationPoint();

	// Check if input point parameter provided by user.
	if ((p1.getX() == INVALID) || (p2.getX() == INVALID))
	{
		// Get min and max coordiantes.
		Config::getScreenCoordinates(minX, minY, maxX, maxY);

		// Generate seed.
		srand(time(nullptr));

		// Create a random points.
		p1.setX(rand() % (int) maxX);
		p1.setY(rand() % (int) maxY);
		p2.setX(rand() % (int) maxX);
		p2.setY(rand() % (int) maxY);
	}
}


void Process::createDcelPointsInfo(const Dcel &dcelIn, vector<Text> &info)
{
    char	text_Info[50];

    // Draw all points of the set.
    for (size_t i=0; i < dcelIn.getNVertex() ; i++)
    {
        // Get and draw i-point.
        Point<TYPE> *point = dcelIn.getRefPoint(i);
        string strText = std::to_string(i+1);

        Text text(point->getX(), point->getY(), strText);
        info.push_back(text);
    }
}


void Process::createDcelEdgeInfo(const Dcel &dcelIn, vector<Text> &info)
{
    // Loop all edges.
    for (size_t edgeIndex=0; edgeIndex<dcelIn.getNEdges() ;edgeIndex++)
    {
        // Check if twin edge already visited.
        if ((edgeIndex+1) < dcelIn.getTwin(edgeIndex))
        {
            // Check edge is real.
            if (!dcelIn.hasNegativeVertex((int) edgeIndex+1))
            {
                // Get edge extreme points.
                Point<TYPE> origin, dest;	// Extreme points of edges.
                dcelIn.getEdgePoints(edgeIndex, origin, dest);

                // Compute middle point of edge.
                Point<TYPE> middle;         // Middle point of the edge.
                Point<TYPE>::middlePoint(&origin, &dest, &middle);

                // Print information.
                string strText = std::to_string(edgeIndex+1) + " - " + std::to_string(dcelIn.getTwin(edgeIndex));

                Text text(middle.getX(), middle.getY(), strText);
                info.push_back(text);
            }
        }
    }
}


void Process::createDcelFacesInfo(const Dcel &dcelIn, vector<Text> &info)
{
    // Loop all faces (skip external face).
    for (size_t faceId=0; faceId<dcelIn.getNFaces() ;faceId++)
    {
        // If any vertex is imaginary then face is not drawn.
        if (!dcelIn.imaginaryFace(faceId))
        {
            Polygon polygon;

            // Get edge in current face.
            size_t firstEdgeIndex = dcelIn.getFaceEdge(faceId)-1;
            size_t edgeIndex = firstEdgeIndex;
            do
            {
                // Add origin point to polygon.
                Point<TYPE> origin;			// Edge origin point.
                origin = *dcelIn.getRefPoint(dcelIn.getOrigin(edgeIndex)-1);
                polygon.add(origin);

                // Next edge in face.
                edgeIndex = dcelIn.getNext(edgeIndex)-1;
            } while(edgeIndex != firstEdgeIndex);

            // Compute face centroid.
            Point<TYPE> center;			// Middle point of the edge.
            polygon.centroid(center);
            polygon.reset();

            // Print information.
            string strText = std::to_string(faceId);

            Text text(center.getX(), center.getY(), strText);
            info.push_back(text);
        }
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
void Process::execute()
{
    Command *cmd;           // Command to execute
    bool isSuccess;

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
			cmd = CommandFactory::create(option, storeService->getDcel());
            cmd->run();
            dispManager->process();
			break;
		}
		// New set of points (generated or read).
		case RANDOMLY:
		case CLUSTER:
        {
            this->resetData();

            // Read configuration file.
            cmd = CommandFactory::create(option, storeService->getDcel());
            isSuccess = cmd->run();

            //	// Add figure display.
            vector<Point<TYPE>> vPoints;
            for (size_t i=0; i< Config::getNPoints(); i++)
            {
                vPoints.push_back(*storeService->getDcel()->getRefPoint(i));
            }
            dispManager->add(DisplayableFactory::createPointsSet(vPoints));
            dispManager->process();

            // Update menu entries.
            this->status.set(false, isSuccess, false, false, false, false);
            m.updateMenu();

            break;
        }
		case READ_POINTS_FLAT_FILE:
		case READ_POINTS_DCEL_FILE:
		case READ_DCEL:
		case READ_DELAUNAY:
		case READ_VORONOI:
		case READ_GABRIEL:
		{
			this->resetData();

			// Check option to generate/read set.
			if (this->readData(option))
		    {
//				if (this->status.isVoronoiCreated())
//				{
//					// Clear screen.
//					this->drawer->drawFigures(VORONOI_DRAW);
//				}
//				if (this->status.isDelaunayCreated() ||
//					this->status.isTriangulationCreated())
//				{
//					// Draw triangulation.
//					this->drawer->drawFigures(TRIANGULATION_DRAW);
//				}
//				else
//				{
//					// Draw set of points.
//                    dispManager->process();
//					//this->drawer->drawFigures(SET_DRAW);
//				}

                dispManager->process();

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
                // Draw Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);
                dispManager->process();

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
				error = !this->voronoi.init(storeService->getDcel());
				if (!error)
				{
					// Compute Voronoi diagram.
					error = !this->voronoi.build(true);
				}

				// Check error and update status.
				if (!error)
				{
					// Draw Voronoi graph and the triangulation.
					Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                    dispManager->add(dispDelaunay);

                    Displayable *dispVoronoi = DisplayableFactory::createDcel(this->voronoi.getRefDcel());
                    dispManager->add(dispVoronoi);
                    dispManager->process();

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
				this->gabriel.init(this->delaunay.getRefDcel(), &this->voronoi);
				error = !this->gabriel.build();
				if (!error)
				{
					// Draw Voronoi graph and the triangulation.
                    Point<TYPE> *vertex1;	    // First vertex.
                    Point<TYPE> *vertex2;	    // Second vertex.
                    Dcel	*dcelRef;

                    // Get reference to gabriel dcel.
                    dcelRef = gabriel.getDcel();

                    // Draw Gabriel edges.
                    vector<Line> vLines;
                    for (size_t edgeIndex=0; edgeIndex<gabriel.getSize() ;edgeIndex++)
                    {
                        // Check if current edge mamtches Gabriel restriction.s
                        if (gabriel.isSet(edgeIndex))
                        {
                            // Get origin vertex of edge.
                            vertex1 = dcelRef->getRefPoint(dcelRef->getOrigin(edgeIndex)-1);

                            // Get destination vertex of edge.
                            vertex2 = dcelRef->getRefPoint(dcelRef->getOrigin(dcelRef->getTwin(edgeIndex)-1)-1);

                            Line line(*vertex1, *vertex2);
                            vLines.push_back(line);
                        }
                    }

                    // Draw Delaunay
                    Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                    dispManager->add(dispDelaunay);

                    // Add lines
                    dispManager->add(DisplayableFactory::createPolyLine(vLines));

                    dispManager->process();

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
				// Get points.
				this->getLineToLocate(p1, p2);
				line = Line(p1, p2);

				// Check incremental triangulation computed.
				if (this->delaunay.getAlgorithm() == INCREMENTAL)
				{
					// Compute triangles path between two points.
					error = !this->delaunay.findPath(line, faces);
				}
				else
				{
					// PENDING https://github.com/juannavascalvente/Delaunay/issues/10
					Logging::buildText(__FUNCTION__, __FILE__,
							"StarTriangulation path not implemented in normal triangulation");
					Logging::write(true, Error);
				}

				// Print error message.
				if (error)
				{
                    Logging::buildText(__FUNCTION__, __FILE__, "Error computing Delaunay path");
                    Logging::write(true, Error);
                }
				else
                {
                    // Add Delaunay triangulation
                    Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                    dispManager->add(dispDelaunay);

                    // Add points whose path is drawn
                    vector<Point<TYPE>> vPoints;
                    vPoints.push_back(p1);
                    vPoints.push_back(p2);
                    dispManager->add(DisplayableFactory::createPolygon(vPoints));

                    // Add path faces
                    vector<Polygon> vPolygons;
                    for (size_t i=0; i<faces.getNElements() ;i++)
                    {
                        vector<Point<TYPE>> vFacesPoints;
                        DcelFigureBuilder::getFacePoints(*faces.at(i), *storeService->getDcel(), vFacesPoints);

                        Polygon polygon;
                        for (auto point : vFacesPoints)
                        {
                            polygon.add(point);
                        }
                        vPolygons.push_back(polygon);
                    }
                    dispManager->add(DisplayableFactory::createPolygonSet(vPolygons));
                    dispManager->process();
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
				// Get points.
				this->getLineToLocate(p1, p2);
				line = Line(p1, p2);

				// Check incremental triangulation computed.
				if (this->delaunay.getAlgorithm() == INCREMENTAL)
				{
					// Compute triangles path between two points.
					error = !this->findPath(this->delaunay, this->voronoi, line, faces);
				}
				else
				{
					// TODO https://github.com/juannavascalvente/Delaunay/issues/10
					Logging::buildText(__FUNCTION__, __FILE__,
							"Voronoi path not implemented in normal triangulation");
					Logging::write(true, Error);
					error = true;
				}

				// Check if an error must be printed.
				if (error)
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Error computing Voronoi path");
					Logging::write(true, Error);
				}
				else
                {
                    // Add Delaunay triangulation
                    Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                    dispManager->add(dispDelaunay);

                    // Add Voronoi
                    Displayable *dispVoronoi = DisplayableFactory::createDcel(this->voronoi.getRefDcel());
                    dispManager->add(dispVoronoi);

                    // Add points whose path is drawn
                    vector<Point<TYPE>> vPoints;
                    vPoints.push_back(p1);
                    vPoints.push_back(p2);
                    dispManager->add(DisplayableFactory::createPolygon(vPoints));

                    // Add path faces
                    vector<Polygon> vPolygons;
                    for (size_t i=0; i<faces.getNElements() ;i++)
                    {
                        vector<Point<TYPE>> vFacesPoints;
                        DcelFigureBuilder::getFacePoints(*faces.at(i), *this->voronoi.getRefDcel(), vFacesPoints);

                        Polygon polygon;
                        for (auto point : vFacesPoints)
                        {
                            polygon.add(point);
                        }
                        vPolygons.push_back(polygon);
                    }
                    dispManager->add(DisplayableFactory::createPolygonSet(vPolygons));
                    dispManager->process();
                }
			}
			break;
		}
		// Compute convex hull.
		case CONVEX_HULL:
		{
			// Computing convex hull.
			if (this->buildConvexHull())
			{
                Polygon *hull;

                // Computing convex hull.
                if (this->status.isDelaunayCreated())
                {
                    hull = this->delaunay.getConvexHull();
                }
                else
                {
                    hull = this->triangulation.getConvexHull();
                }

                // Add points to display manager.
                vector<Point<TYPE>> vPoints;
                hull->getPoints(vPoints);
                dispManager->add(DisplayableFactory::createPolygon(vPoints));
                dispManager->process();
            }
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Convex hull not computed");
				Logging::write(true, Error);
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
			if (this->findClosest(point, closest, distance))
			{
				// Draw the triangulation, the point and the closest point.
                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                vector<Point<TYPE>> vPoints;
                vPoints.push_back(point);
                vPoints.push_back(closest);
                Displayable *closestPoints = DisplayableFactory::createPointsSet(vPoints);
                closestPoints->setPointSize(3.0);
                dispManager->add(closestPoints);

                dispManager->process();
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

			// Check if input point parameter provided by user.
			this->getPointToLocate(point);

			// Find face.
			bool isImaginaryFace=false;
			if (this->findFace(point, faceId, isImaginaryFace))
			{
				// Draw the triangulation, the point and its face.
                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add face
                if (!isImaginaryFace)
                {
                    vector<Point<TYPE>> vFacesPoints;
                    DcelFigureBuilder::getFacePoints(faceId, *storeService->getDcel(), vFacesPoints);
                    dispManager->add(DisplayableFactory::createPolygon(vFacesPoints));
                }

                // Add point to draw
                vector<Point<TYPE>> vPoints;
                vPoints.push_back(point);
                Displayable *closestPoints = DisplayableFactory::createPointsSet(vPoints);
                closestPoints->setPointSize(3.0);
                dispManager->add(closestPoints);

                dispManager->process();
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error face not found");
				Logging::write(true, Error);
			}
			break;
		}
		// Find the two closest point in the set.
		case TWO_CLOSEST:
		{
			Set<PointT> points(2);	// List of points.

			// Compute the two closest point in the set of points.
			if (this->findTwoClosest(index1, index2))
			{
                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                vector<Point<TYPE>> vPoints;
                vPoints.push_back(*storeService->getDcel()->getRefPoint(index1));
                vPoints.push_back(*storeService->getDcel()->getRefPoint(index2));
                Displayable *closestPoints = DisplayableFactory::createPointsSet(vPoints);
                dispManager->add(closestPoints);

                dispManager->process();
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "The two closest points not found");
				Logging::write(true, Error);
			}
			break;
		}
		// Filter edges whose length is lower than a threshold.
		case FILTER_EDGES:
		{
			// Check if Delaunay triangulation already created.
			if (status.isTriangulationCreated())
			{
                // Add Delaunay triangulation filtering edges
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel(), Config::getMinLengthEdge());
                dispManager->add(dispDelaunay);

                dispManager->process();
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
                // Add circles
                vector<Circle> vCircles;
                for (int faceID=1; faceID<storeService->getDcel()->getNFaces() ;faceID++)
                {
                    // Skip imaginary faces.
                    if (!storeService->getDcel()->imaginaryFace(faceID))
                    {
                        // Get points of the triangle.
                        int		points[NPOINTS_TRIANGLE];	// Triangle points.
                        storeService->getDcel()->getFaceVertices(faceID, points);

                        // Build circle.
                        vector<Point<TYPE>> vPoints;
                        vPoints.push_back(*storeService->getDcel()->getRefPoint(points[0]-1));
                        vPoints.push_back(*storeService->getDcel()->getRefPoint(points[1]-1));
                        vPoints.push_back(*storeService->getDcel()->getRefPoint(points[2]-1));
                        Circle circle = Circle(vPoints);
                        vCircles.push_back(circle);
                    }
                }

                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                Displayable *circles = DisplayableFactory::createCircleSet(vCircles);
                dispManager->add(circles);

                dispManager->process();
			}
			break;
		}
		case EDGE_CIRCLES:
		{
			// Check if triangulation created.
			if (this->status.isTriangulationCreated())
			{
                int		edgeIndex=0;        // Edge index.
                int		nEdges=0;			// # edges in the storeService->getDcel()->
                TYPE  	radius; 			// Circle radius.
                Line	line;				// Edge line.
                Point<TYPE> origin, dest;  	// Origin and destination points.
                Point<TYPE> middle;	    	// Edge middle point.

                // Add circles
                vector<Circle> vCircles;

                // Loop all faces (but external).
                nEdges = storeService->getDcel()->getNEdges();
                for (edgeIndex=0; edgeIndex<nEdges ;edgeIndex++)
                {
                    // Skip imaginary edges.
                    if (!storeService->getDcel()->hasNegativeVertex(edgeIndex+1))
                    {
                        // Create line.
                        storeService->getDcel()->getEdgePoints(edgeIndex, origin, dest);
                        line = Line(origin, dest);

                        // Compute middle point of edge.
                        line.getMiddle(middle);

                        // Create circle
                        radius = origin.distance(middle);
                        Circle circle = Circle(&middle, radius);

                        // Add circles
                        vCircles.push_back(circle);
                    }
                }

                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                Displayable *circles = DisplayableFactory::createCircleSet(vCircles);
                dispManager->add(circles);

                dispManager->process();
			}
			break;
		}
		// Print DCEL data.
		case DCEL_INFO:
		{
            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
            dispManager->add(dispDelaunay);

            vector<Text> vPointsInfo;
            this->createDcelPointsInfo(*storeService->getDcel(), vPointsInfo);
            Displayable *dispPointsInfo = DisplayableFactory::createTextSet(vPointsInfo);
            dispManager->add(dispPointsInfo);

            vector<Text> vEdgesInfo;
            this->createDcelEdgeInfo(*storeService->getDcel(), vEdgesInfo);
            Displayable *dispEdgesInfo = DisplayableFactory::createTextSet(vEdgesInfo);
            dispManager->add(dispEdgesInfo);

            vector<Text> vFacesInfo;
            this->createDcelFacesInfo(*storeService->getDcel(), vFacesInfo);
            Displayable *dispFacesInfo = DisplayableFactory::createTextSet(vFacesInfo);
            dispManager->add(dispFacesInfo);

            dispManager->process();
			break;
		}
		// Print Voronoi data.
		case VORONOI_INFO:
		{
            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(this->voronoi.getRefDcel());
            dispManager->add(dispDelaunay);

            vector<Text> vPointsInfo;
            this->createDcelPointsInfo(*this->voronoi.getRefDcel(), vPointsInfo);
            Displayable *dispPointsInfo = DisplayableFactory::createTextSet(vPointsInfo);
            dispManager->add(dispPointsInfo);

            vector<Text> vEdgesInfo;
            this->createDcelEdgeInfo(*this->voronoi.getRefDcel(), vEdgesInfo);
            Displayable *dispEdgesInfo = DisplayableFactory::createTextSet(vEdgesInfo);
            dispManager->add(dispEdgesInfo);

            vector<Text> vFacesInfo;
            this->createDcelFacesInfo(*this->voronoi.getRefDcel(), vFacesInfo);
            Displayable *dispFacesInfo = DisplayableFactory::createTextSet(vFacesInfo);
            dispManager->add(dispFacesInfo);

            dispManager->process();
			break;
		}
		// Write points to a flat file.
		case WRITE_POINTS:
		{
			DcelWriter::writePoints(Config::getOutFlatFilename(), INVALID, *storeService->getDcel());
			break;
		}
		// Write points to a DCEL file.
		case WRITE_DCEL:
		{
			DcelWriter::write(Config::getOutDCELFilename(), false, *storeService->getDcel());
			break;
		}
		// Write DCEL and graph files.
		case WRITE_DELAUNAY:
		{
            DelaunayIO::write(Config::getOutDCELFilename(), Config::getOutGraphFilename(), this->delaunay);
			break;
		}
		// Write voronoi DCEL file.
		case WRITE_VORONOI:
		{
			VoronoiIO::write(Config::getOutVoronoiFilename(), this->voronoi);
			break;
		}
		// Write Gabriel graph data.
		case WRITE_GABRIEL:
		{
			GabrielIO::writeBinary(Config::getOutGabrielFilename(), this->gabriel);
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
            dispManager->process();

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
                dispManager->process();
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
