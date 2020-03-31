
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Process.h"
#include "Command.h"
#include "CommandFactory.h"
#include "DcelGenerator.h"
#include "DcelReader.h"
#include "DcelWriter.h"
#include "DelaunayIO.h"
#include "DisplayableFactory.h"
#include "GabrielIO.h"
#include "LineFactory.h"
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

	this->m = Menu(storeServiceIn->getStatus());

	// Function to execute by GLUT.
	glutDisplayFunc(executeWrapper);
}


Process::~Process()
{
	// Deallocate resources
	delete this->log;
	delete this->dispManager;
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
//            status->set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Generate clusters set.
//		case CLUSTER:
//		{
//            isSuccess = DcelGenerator::generateClusters(Config::getNPoints(), Config::getNClusters(), Config::getRadius(), this->dcel);
//			status->set(false, isSuccess, false, false, false, false);
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
//			status->set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Read dcel file.
//		case READ_DCEL:
//		{
//			// PENDING CHECK IF A DCEL IS CONSISTENT?
//			success = DcelReader::read(Config::getInDCELFilename(), false, this->dcel);
//			this->delaunay.setDCEL(&this->dcel);
//			status->set(false, true, true, false, false, false);
//			break;
//		}
//		// Read Delaunay incremental algorithm files.
//		case READ_DELAUNAY:
//		{
//			// PENDING CHECK IF A DCEL AND GRAPG ARE CONSISTENT?
//			this->delaunay.setDCEL(&this->dcel);
//			success = DelaunayIO::read(Config::getInDCELFilename(),
//                                       Config::getInGraphFilename(), this->delaunay);
//			status->set(false, success, success, success, false, false);
//			this->delaunay.setAlgorithm(INCREMENTAL);
//			break;
//		}
//		// Read Voronoi file.
//		case READ_VORONOI:
//		{
//			// PENDING: What to allow in menu if only voronoi is read.
//			//success = this->voronoi.read(Config::getInVoronoiFilename());
//			//status->set(false, true, !success, !success, true, false);
//			cout << "NOT IMPLEMENTED YET" << endl;
//			break;
//		}
//		// Read Gabriel file.
//		default:
//		{
//			// PENDING: What to allow in menu if only voronoi is read.
//			success = GabrielIO::readBinary(Config::getOutGabrielFilename(), this->gabriel);
//			status->setGabrielCreated(true);
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
    Command *cmd=nullptr;           // Command to execute
    CommandResult *result;

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
        case RANDOMLY:
        case CLUSTER:
		case STAR_TRIANGULATION:
		case DELAUNAY:
		case CONVEX_HULL:
        case VORONOI:
        case GABRIEL:
        case TRIANGULATION_PATH:
        case VORONOI_PATH:
        case CLOSEST_POINT:
        case FIND_FACE:
        case TWO_CLOSEST:
        case FILTER_EDGES:
        case CIRCUMCENTRES:
        case EDGE_CIRCLES:
        case DCEL_INFO:
        case VORONOI_INFO:
        case CLEAR:
		{
            // Create command
            cmd = CommandFactory::create(option, storeService);

            // Run command
            cmd->run();

            // Process results
            result = cmd->getResult();
            if (result->wasSuccess())
            {
                // Update menu status
                result->updateStatus();

                // Get displaybale elements
                vector<Displayable*> vDisplayable(0);
                result->createDisplayables(vDisplayable);
                dispManager->add(vDisplayable);

                dispManager->process();
            }

            // Update menu entries.
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
			// Check option to generate/read set.
			if (this->readData(option))
		    {
//				if (status->isVoronoiCreated())
//				{
//					// Clear screen.
//					this->drawer->drawFigures(VORONOI_DRAW);
//				}
//				if (status->isDelaunayCreated() ||
//					status->isTriangulationCreated())
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
            Delaunay *delaunay = storeService->getDelaunay();
            DelaunayIO::write(Config::getOutDCELFilename(), Config::getOutGraphFilename(), *delaunay);
			break;
		}
		// Write voronoi DCEL file.
		case WRITE_VORONOI:
		{
			VoronoiIO::write(Config::getOutVoronoiFilename(), *storeService->getVoronoi());
			break;
		}
		// Write Gabriel graph data.
		case WRITE_GABRIEL:
		{
			GabrielIO::writeBinary(Config::getOutGabrielFilename(), *storeService->getGabriel());
			break;
		}
		// Quit application.
		case QUIT:
		{
			// Quit application.
			quit = true;
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

	// Delete iteration resources
    delete cmd;

	// Exit application.
	if (quit)
	{
		// PENDING. Anything to deallocate.
		delete this->log;
		exit(0);
	}
	else
	{
		// Reset menu option.
		m.resetMenuOption();
	}
}
