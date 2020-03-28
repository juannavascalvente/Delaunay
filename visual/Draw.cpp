/*
 * Draw.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: jnavas
 */

#include "../visual/Draw.h"

#include "Queue.h"
#include <string.h>

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_DRAW_CIRCUMCENTRES
//#define DEBUG_DCEL_DRAWEDGE
//#define DEBUG_DRAW_DCEL_TRIANGULATION
//#define DEBUG_DRAW_POLYGON
//#define DEBUG_DCEL_DRAW_FACE
//#define DEBUG_DRAW_GABRIEL
//#define DEBUG_DRAW_POLYLINE
//#define DEBUG_DRAW_DRAWEDGE
//#define DEBUG_DRAW_DRAWFACE
//#define DEBUG_DRAW_POINTS_INFO
//#define DEBUG_DRAW_EDGES_INFO
//#define DEBUG_DRAW_FACES_INFO
#endif

#ifdef WINDOWS
#include <windows.h>
#endif
#include <GL/glut.h>
#include <GL/gl.h>

// Internal static variables.
void 		  *font;
Draw 		  *Draw::instance=NULL; // Drawer instance.
Dcel		  *Draw::dcel;			// Dcel data.
Delaunay	  *Draw::delaunay;		// Delaunay data.
StarTriangulation *Draw::triangulation;	// Star triangulation data.
Voronoi		  *Draw::voronoi;		// Voronoi diagram data.
Gabriel		  *Draw::gabriel;		// Gabriel graph data.


// Internal static functions.
void refresh(void);

//------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------
Draw * Draw::getInstance(int argc, char **argv, Dcel *inDcel, Delaunay *inDelaunay, StarTriangulation *inTriangulation,
                         Voronoi *inVoronoi, Gabriel *inGabriel, Status *inStatus)
{
	int minX, minY, maxX, maxY;		// Window dimension.

	// Check if drawer already created.
	if (!instance)
	{
		// Get instance to drawer.
		instance = new Draw();

		// Initialize openGL.
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(1100, 750);
		glutInitWindowPosition(100, 100);
		instance->windowID = glutCreateWindow("GEOMETRICAL");
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Set background color.
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glMatrixMode(GL_MODELVIEW);
		glutIdleFunc(refresh);

		// Get reference to data to draw.
		dcel = inDcel;
		delaunay = inDelaunay;
		triangulation = inTriangulation;
		voronoi = inVoronoi;
		gabriel = inGabriel;

		// Set x, y and z axis size.
		Config::getScreenCoordinates(minX, minY, maxX, maxY);
		glOrtho(minX-100, maxX+100, minY-100, maxY+100, -1, 3);
		font = GLUT_BITMAP_TIMES_ROMAN_10;
	}

	return(instance);
}


//------------------------------------------------------------------------
// Private functions.
//------------------------------------------------------------------------
void refresh(void)
{
	glutPostRedisplay();
}

