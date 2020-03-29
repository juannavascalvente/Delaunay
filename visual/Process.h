/*
 * Process.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef PROCESS_H_
#define PROCESS_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayManager.h"

#include "Menu.h"
#include "Status.h"
#include "Config.h"
#include "Dcel.h"
#include "Delaunay.h"
#include "Gabriel.h"
#include "Logging.h"
#include "Queue.h"
#include "StarTriangulation.h"
#include "StoreService.h"
#include "Text.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Process
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    DisplayManager      *dispManager;
    StoreService        *storeService;

	Menu 			    m;				// Menu object.
	Status 			    status;			// Current status.
//	Dcel			    dcel;			// Dcel data.
	Delaunay		    delaunay;		// Delaunay data.
	StarTriangulation	triangulation;	// Star triangulation data.
	Voronoi			    voronoi;		// Voronoi diagram data.
	Gabriel			    gabriel;		// Gabriel graph data.
	Logging 		    *log;			// Log file.

    /*******************************************************************************************************************
     * Private methods declaration
     *******************************************************************************************************************/
	void execute();
	static void executeWrapper();
	bool readData(int option);
	void resetData();
	bool buildTriangulation(int option);
	bool buildConvexHull();
	static bool findPath(Delaunay &delaunay, Voronoi &vor, Line &l, Set<int> &faces);
	bool findTwoClosest( int &index1, int &index2);
	bool findFace(Point<TYPE> &point, int &faceId, bool &isImaginary);
	bool findClosest( Point<TYPE> &point, Point<TYPE> &q, double &distance);
	static void getPointToLocate(Point<TYPE> &point);
	static void getLineToLocate(Point<TYPE> &p1, Point<TYPE> &p2);
    static void createDcelPointsInfo(const Dcel &dcelIn, vector<Text> &info);
    static void createDcelEdgeInfo(const Dcel &dcelIn, vector<Text> &info);
    static void createDcelFacesInfo(const Dcel &dcelIn, vector<Text> &info);

protected:
	static Process *instance;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Process(int argc, char **argv, bool printData, StoreService *storeServiceIn);
	~Process();

    /**
     * @fn      start
     * @brief   starts infinite loop
     */
    static void start();

    /**
     * @fn      setInstance
     * @brief   Sets "instance" to the object that is going to be executed
     *          by the main loop process
     * @param   process   instance to be executed by main loop
     */
	static void setInstance(Process *process);
};

#endif /* PROCESS_H_ */
