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
#include "Status.h"
#include "Triangulation.h"

#include <GL/glut.h>

typedef Point<TYPE> PointT;

//****************************************************************************
// 							Data types definition
//****************************************************************************
enum ColorT { RED,
              GREEN,
              BLUE,
              LIGHT_BLUE,
              YELLOW,
              WHITE,
              BLACK};

// Type of figure to draw.
enum drawingT {CLEAR_SCREEN,
				  SET_DRAW,
				  TRIANGULATION_DRAW,
				  VORONOI_DRAW,
				  GABRIEL_DRAW,
				  CONVEXHULL_DRAW,
				  CLOSESTPOINT_DRAW,
				  FINDFACE_DRAW,
				  TRIANGULATION_PATH_DRAW,
				  VORONOI_PATH_DRAW,
				  TWOCLOSEST_DRAW,
				  FILTEREDGES_DRAW,
				  CIRCUMCENTRES_DRAW,
				  EDGESCIRCLES_DRAW,
				  DCEL_INFO_DRAW,
				  VORONOI_INFO_DRAW};

// Type of information to draw.
enum drawingInfoT {	PointsInfo,
					EdgesInfo,
					FacesInfo,
					AllInfo};

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
	static Triangulation *triangulation;// Star triangulation data.
	static Voronoi		 *voronoi;		// Voronoi diagram data.
	static Gabriel		 *gabriel;		// Gabriel graph data.
	static Status 		 *status;		// Current process status.
	static Config 		 *config;		// Configuration file.

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	// General figures.
	void drawSet();
	void drawDelaunay(TYPE minLength);
	void drawVoronoi();
	void drawGabriel();
	void drawPath( Dcel *dcel, Point<TYPE> *p1, Point<TYPE> *p2, Queue<int> &queue);

	// Single figures.
	void drawPoints( Dcel *dcel);
	void draw( PointT *point);
	void draw( Line *line);
	void draw( Polygon *polygon);
	void draw( Circle *circle, bool fill);
	void draw( Dcel *dcel);
	void draw( Dcel *dcel, TYPE minLength);
	void drawEdge( int edgeIndex, Dcel *dcel);
	void draw( Face *face, Dcel *dcel);
	void drawFace( int faceId, Dcel *dcel);
	void draw( Gabriel *gabriel);
	void drawCircumcentres( Dcel *dcel);
	void drawEdgesCircles( Dcel *dcel);

	// Screen API.
	inline void clear()  { glClear(GL_COLOR_BUFFER_BIT);} ;
	inline void flush()  { glutSwapBuffers(); };

	// Type of lines.
	inline void startPoints() { glBegin(GL_POINTS); };
	inline void startCircle() { glBegin(GL_LINE_LOOP); };
	inline void startLine()   { glBegin(GL_LINE_STRIP); };
	inline void finish() { glEnd(); };

	// Size and font.
	inline void setPointSize(float size) { glPointSize(size);} ;
	inline void setLineSize(float size) { glLineWidth(size);} ;
	void setColor(enum ColorT);
	void setFont(void *newFont);
	void drawText(float x, float y, char *string);

	// Information.
	void drawPointsInfo( Dcel *dcel);
	void drawEdgesInfo( Dcel *dcel);
	void drawFacesInfo( Dcel *dcel);
	void drawInfo( Dcel *dcel);
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	static Draw* getInstance(int argc, char **argv,
							 Dcel *dcel,
							 Delaunay *delaunay,
							 Triangulation *triangulation,
							 Voronoi *voronoi,
							 Gabriel *gabriel,
							 Status *status,
							 Config *config);
	Draw() {};
	~Draw() {};

	//------------------------------------------------------------------------
	// Public API.
	//------------------------------------------------------------------------
	inline void setPointsSet(Set<PointT> *set) { this->pointsSet = set; };
	inline void setFacesSet(Set<int> *faces) { this->facesSet = faces; };

	// Main function.
	void drawFigures(enum drawingT type, bool error=false);
};
#endif /* DRAW_H_ */
