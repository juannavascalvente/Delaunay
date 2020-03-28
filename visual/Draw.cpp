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
/*
Draw::~Draw()
{
	glutDestroyWindow(this->windowID);
}
*/

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	drawFigures
* IN:		type		type of drawing.
* 			error		error flag from calling function
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Draw depending on the type of figures to draw determined by
* 				the input "type" parameter. If the "error" parameter is true
* 				then some figures will not be completely displayed.
***************************************************************************/
void Draw::drawFigures(enum drawingT type, bool error)
{
	int	i=0;			// Loop counter.
	int	faceId=0;		// Face identifier.
	Point<TYPE>	p,q;	// Points to draw.
	Line	*line;		// Line to draw.

	// Clear screen.
	this->clear();

	// Check type of data to draw.
	switch(type)
	{
		// Draw triangulation and Gabriel graph.
		case GABRIEL_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawGabriel();
			break;
		}
		// Unknown type of drawing.
		default:
		{
			break;
		}
	}

	// Flush data.
	this->flush();
}

//------------------------------------------------------------------------
// Private functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	drawDelaunay
* IN:		TYPE minLength		min length of the edges.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw the Delaunay triangulaion filtering the edges if the
* 				minLength parameter is not INVALID.
***************************************************************************/
void Draw::drawDelaunay(TYPE minLength)
{
	this->setColor(RED);
	this->draw(this->dcel, minLength);
}


/***************************************************************************
* Name: 	drawGabriel
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw the Gabriel graph
***************************************************************************/
void Draw::drawGabriel()
{
	this->setColor(GREEN);
	this->draw(this->gabriel);
}


/***************************************************************************
* Name: 	draw
* IN:		dcel		DCEL data to draw.
* 			minLength	min length of the edges to draw.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draws the set of vertex of the DCEL and the triangulation.
* 				if a minimum length is supplied then only the edges longer
* 				then that length are drawn.
***************************************************************************/
void Draw::draw(Dcel *dcel, TYPE minLength)
{
	int		edgeIndex=0;		// Loop counter.
	Point<TYPE> *vertex1;	    // First vertex.
	Point<TYPE> *vertex2;	    // Second vertex.
	TYPE	dist=0.0;			// Points distance.

#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
	Logging::buildText(__FUNCTION__, __FILE__, "\n\nDrawing triangulation");
	Logging::write(true, Info);
#endif

	// Draw all edges.
	for (edgeIndex=0; edgeIndex<dcel->getNEdges() ;edgeIndex++)
	{
		// Skip triangle if any vertex is P_MINUS_1 or P_MINUS_2.
		if (!dcel->hasNegativeVertex(edgeIndex+1))
		{
		    // Not draw edges whose twin has been already drawn.
			if (edgeIndex < dcel->getTwin(edgeIndex)-1)
            {
                // Get origin vertex of edge.
				vertex1 = dcel->getRefPoint(dcel->getOrigin(edgeIndex)-1);

                // Get destination vertex of edge.
				vertex2 = dcel->getRefPoint(dcel->getOrigin(dcel->getTwin(edgeIndex)-1)-1);
				if (minLength != INVALID)
				{
					dist = vertex1->distance(*vertex2);
				}
#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
				Logging::buildText(__FUNCTION__, __FILE__, "Points are: ");
				Logging::buildText(__FUNCTION__, __FILE__, vertex1);
				Logging::buildText(__FUNCTION__, __FILE__, " ");
				Logging::buildText(__FUNCTION__, __FILE__, vertex2);
				Logging::write(true, Info);
#endif
				if ((minLength == INVALID) || (dist > minLength))
				{
#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
					Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write(true, Info);
#endif
					// Draw edges of current edge.
					this->startLine();

					// Draw first point.
					glVertex2f(vertex1->getX(), vertex1->getY());

					// Draw second point.
					glVertex2f(vertex2->getX(), vertex2->getY());

					this->finish();
				}
#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
				else
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write(true, Info);
				}
#endif
            }
#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
            else
            {
				Logging::buildText(__FUNCTION__, __FILE__, "Skipping edge " );
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, " because twin is lower.");
				Logging::write(true, Info);
            }
#endif
		}
#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
		else
		{
			int index1, index2;
			dcel->getEdgeVertices(edgeIndex+1, index1, index2);
			Logging::buildText(__FUNCTION__, __FILE__, "Imaginary points in edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, " are ");
			Logging::buildText(__FUNCTION__, __FILE__, index1);
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, index2);
			Logging::write(true, Info);
		}
#endif
	}

#ifdef DEBUG_DRAW_DCEL_TRIANGULATION
	Logging::buildText(__FUNCTION__, __FILE__, "Finished drawing Delaunay triangulation");
	Logging::write(true, Info);
#endif
}


/***************************************************************************
* Name: 	draw
* IN:		gabriel		Gabriel graph data.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draws the triangulation filtering edges that do not match
* 				the Gabriel graph restriction.
***************************************************************************/
void Draw::draw(Gabriel *gabriel)
{
	int		edgeIndex=0;		// Loop counter.
	Point<TYPE> *vertex1;	    // First vertex.
	Point<TYPE> *vertex2;	    // Second vertex.
	Dcel	*dcel;

	// Check if input data is valid.
	if (gabriel->isValid())
	{
		// Get reference to gabriel dcel.
		dcel = gabriel->getDcel();
#ifdef DEBUG_DRAW_GABRIEL
		Logging::buildText(__FUNCTION__, __FILE__, "Drawing Gabriel graph. Length ");
		Logging::buildText(__FUNCTION__, __FILE__, gabriel->getSize());
		Logging::write(true, Info);
#endif
		// Draw Gabriel edges.
		for (edgeIndex=0; edgeIndex<gabriel->getSize() ;edgeIndex++)
		{
#ifdef DEBUG_DRAW_GABRIEL
			Logging::buildText(__FUNCTION__, __FILE__, "Checking edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::write(true, Info);
#endif
			// Check if current edge mamtches Gabriel restriction.s
			if (gabriel->isSet(edgeIndex))
			{
#ifdef DEBUG_DRAW_GABRIEL
				Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::write(true, Info);
#endif
				// Get origin vertex of edge.
				vertex1 = dcel->getRefPoint(dcel->getOrigin(edgeIndex)-1);

                // Get destination vertex of edge.
				vertex2 = dcel->getRefPoint(dcel->getOrigin(dcel->getTwin(edgeIndex)-1)-1);

				// Draw edges of current edge.
				this->startLine();

				// Draw first point.
				glVertex2f(vertex1->getX(), vertex1->getY());

				// Draw second point.
				glVertex2f(vertex2->getX(), vertex2->getY());

				this->finish();
			}
#ifdef DEBUG_DRAW_GABRIEL
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Non Gabriel edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::write(true, Info);
			}
#endif
		}
	}
}


/***************************************************************************
* Name: 	setColor
* IN:		color		color to set
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: set current color
***************************************************************************/
void Draw::setColor(enum ColorT color)
{
    // Check input color.
    switch(color)
    {
        // Set red as current color.
        case RED:
        {
            glColor3f(1.0, 0.0, 0.0);
            break;
        }
        // Set red as current color.
        case GREEN:
        {
            glColor3f(0.0, 1.0, 0.0);
            break;
        }
        // Set red as current color.
        case BLUE:
        {
            glColor3f(0.0, 0.0, 1.0);
            break;
        }
        // Set red as current color.
        case LIGHT_BLUE:
        {
            glColor3f(0.0, 1.0, 1.0);
            break;
        }
        // Set yellow as current color.
        case YELLOW:
        {
            glColor3f(1.0, 1.0, 0.0);
            break;
        }
        // Set red as current color.
        case WHITE:
        {
            glColor3f(1.0, 1.0, 1.0);
            break;
        }
        // Set red as current color.
        default:
        {
            glColor3f(0.0, 0.0, 0.0);
            break;
        }
    }
}


void refresh(void)
{
	glutPostRedisplay();
}

