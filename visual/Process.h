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

#include "Draw.h"
#include "Menu.h"
#include "Status.h"
#include "Config.h"
#include "Dcel.h"
#include "Delaunay.h"
#include "Gabriel.h"
#include "Logging.h"
#include "Queue.h"
#include "StarTriangulation.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Process
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    DisplayManager dispManager;

	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	Menu 			m;				// Menu object.
	Status 			status;			// Current status.
	Draw			*drawer;		// Pointer to drawer instance.

	Dcel			dcel;			// Dcel data.
	Delaunay		delaunay;		// Delaunay data.
	StarTriangulation	triangulation;	// Star triangulation data.
	Voronoi			voronoi;		// Voronoi diagram data.
	Gabriel			gabriel;		// Gabriel graph data.

	Logging 		*log;			// Log file.

	/*------------------------------------------------------------------------
	  Private functions.
	------------------------------------------------------------------------*/
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
	void getLineToLocate(Point<TYPE> &p1, Point<TYPE> &p2);

protected:
	static Process *instance;

public:
	/*------------------------------------------------------------------------
	  Constructor/Destructor.
	------------------------------------------------------------------------*/
	Process(int argc, char **argv, bool printData);
	~Process();

	/*------------------------------------------------------------------------
	  Public functions.
	------------------------------------------------------------------------*/
	// Get functions.
	inline Status* getStatus() {return(&this->status); };
	inline Dcel* getDcel() {return(&this->dcel); };
	inline Delaunay* getDelaunay() {return(&this->delaunay); };
	inline StarTriangulation* getTriangulation() {return(&this->triangulation); };
	inline Voronoi* getVoronoi() {return(&this->voronoi); };
	inline Gabriel* getGabriel() {return(&this->gabriel); };

	// Main functions.
    static void start();
	static void setInstance(Process *process);
};

#endif /* PROCESS_H_ */
