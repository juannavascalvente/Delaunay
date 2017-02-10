/*
 * Process.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include "Config.h"
#include "Dcel.h"
#include "Delaunay.h"
#include "Draw.h"
#include "Gabriel.h"
#include "Logging.h"
#include "Menu.h"
#include "Queue.h"
#include "Status.h"
#include "Triangulation.h"
#include "Voronoi.h"


/****************************************************************************
// 						PROCESS CLASS DEFITNION
****************************************************************************/
class Process
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	Config 			*config;		// Configuration file.
	Menu 			m;				// Menu object.
	Status 			status;			// Current status.
	Draw			*drawer;		// Pointer to drawer instance.

	Dcel			dcel;			// Dcel data.
	Delaunay		delaunay;		// Delaunay data.
	Triangulation	triangulation;	// Star triangulation data.
	Voronoi			voronoi;		// Voronoi diagram data.
	Gabriel			gabriel;		// Gabriel graph data.

	Logging 		*log;			// Log file.

	/*------------------------------------------------------------------------
	  Private functions.
	------------------------------------------------------------------------*/
	void execute(void);
	static void executeWrapper();
	bool readData(int option);
	void resetData();
	bool buildTriangulation(int option);
	bool buildConvexHull();
	bool findPath(Delaunay &delaunay, Voronoi &vor, Line &l, Set<int> &faces);
	bool findTwoClosest( int &index1, int &index2);
	bool findFace( Point<TYPE> &point, int &faceId);
	bool findClosest( Point<TYPE> &point, Point<TYPE> &q, double &distance);
	void getPointToLocate(Point<TYPE> &point);

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
	inline Triangulation* getTriangulation() {return(&this->triangulation); };
	inline Voronoi* getVoronoi() {return(&this->voronoi); };
	inline Gabriel* getGabriel() {return(&this->gabriel); };

	// Main functions.
	void start(void);
	static void setInstance(Process *process);
};

#endif /* PROCESS_H_ */
