/*
 * Draw.h
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#ifndef DRAW_H_
#define DRAW_H_


#include "Circle.h"
#include "Config.h"
#include "Delaunay.h"
#include "Gabriel.h"
#include "Queue.h"
#include "Line.h"
#include "Polygon.h"
#include "Point.h"
#include "Polyline.h"
#include "StarTriangulation.h"

#include <GL/glut.h>
#include "../visual/Status.h"

typedef Point<TYPE> PointT;

//****************************************************************************
// 							Data types definition
//****************************************************************************
enum ColorT { RED,
              GREEN,
              BLUE,
              LIGHT_BLUE,
              YELLOW,
              WHITE};

// Type of figure to draw.
enum drawingT {GABRIEL_DRAW};


//****************************************************************************
//                           	DRAW CLASS
//****************************************************************************
class Draw
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	static Draw *instance;

	int					windowID;		// Window id.
	Set<PointT> 		*pointsSet;		// List of points to draw.
	Set<int> 			*facesSet;		// Set of faces.

	static Dcel			 *dcel;			// Dcel data.
	static Delaunay		 *delaunay;		// Delaunay data.
	static StarTriangulation *triangulation;// Star triangulation data.
	static Voronoi		 *voronoi;		// Voronoi diagram data.
	static Gabriel		 *gabriel;		// Gabriel graph data.

	// General figures.
	void drawDelaunay(TYPE minLength);
	void drawGabriel();

	// Single figures.
	void draw( Dcel *dcel, TYPE minLength);
	void draw( Gabriel *gabriel);

	// Screen API.
	inline void clear()  { glClear(GL_COLOR_BUFFER_BIT);} ;
	inline void flush()  { glutSwapBuffers(); };

	// Type of lines.
	inline void startLine()   { glBegin(GL_LINE_STRIP); };
	inline void finish() { glEnd(); };

	// Size and font.
	void setColor(enum ColorT);
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	static Draw *
    getInstance(int argc, char **argv, Dcel *inDcel, Delaunay *inDelaunay, StarTriangulation *inTriangulation,
                Voronoi *inVoronoi, Gabriel *inGabriel, Status *inStatus);
	Draw() : windowID(0), pointsSet(NULL), facesSet(NULL) {} ;
	~Draw() {};

	// Main function.
	void drawFigures(enum drawingT type, bool error=false);
};
#endif /* DRAW_H_ */
