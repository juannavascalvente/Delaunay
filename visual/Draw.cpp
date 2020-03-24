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
Status 		  *Draw::status;		// Current process status.
Config 		  *Draw::config;		// Configuration file.


// Internal static functions.
void refresh(void);

//------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------
Draw* Draw::getInstance(int argc, char **argv, Dcel *inDcel, Delaunay *inDelaunay,
                        StarTriangulation *inTriangulation,
                        Voronoi *inVoronoi,
                        Gabriel *inGabriel,
                        Status *inStatus,
                        Config *inConfig)
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
		status = inStatus;
		config = inConfig;

		// Set x, y and z axis size.
		config->getScreenCoordinates(minX, minY, maxX, maxY);
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
		// Clear screen.
		case CLEAR_SCREEN:
		{
			// Do nothing.
			break;
		}
		// Draw set of points.
		case SET_DRAW:
		{
			this->drawSet();
			break;
		}
		// Draw triangulation.
		case TRIANGULATION_DRAW:
		{
			this->drawDelaunay(INVALID);
			break;
		}
		// Draw triangulation and Voronoi graph.
		case VORONOI_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawVoronoi();
			break;
		}
		// Draw triangulation and Gabriel graph.
		case GABRIEL_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawGabriel();
			break;
		}
		// Draw set of points and convex hull.
		case CONVEXHULL_DRAW:
		{
			this->drawSet();
			if (this->status->isDelaunayCreated())
			{
				// Draw convex hull using Delaunay.
				this->draw(this->delaunay->getConvexHull());
			}
			else
			{
				// Draw convex hull using any triangulation.
				this->draw(this->triangulation->getConvexHull());
			}
			break;
		}
		// Draw the triangulation, the point and the closest point.
		case CLOSESTPOINT_DRAW:
		case TWOCLOSEST_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->setColor(YELLOW);
			for (i=0; i<this->pointsSet->getNElements() ;i++)
			{
				p = *this->pointsSet->at(i);
				this->draw(&p);
			}
			break;
		}
		// Draw the triangulation, the point and its face.
		case FINDFACE_DRAW:
		{
			this->drawDelaunay(INVALID);
			p = *this->pointsSet->at(0);
			this->setColor(YELLOW);
			this->draw(&p);
			if (this->facesSet->getNElements() > 0)
			{
				this->drawFace(*this->facesSet->at(0), this->dcel);
			}
			break;
		}
		// Draw triangulation or Voronoi paths.
		case TRIANGULATION_PATH_DRAW:
		case VORONOI_PATH_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->setColor(YELLOW);
			p = *this->pointsSet->at(0);
			q = *this->pointsSet->at(1);
			line = new Line(p, q);
			this->draw(line);

			// Check type of path to draw.
			if (type == TRIANGULATION_PATH_DRAW)
			{
				if (!error)
				{
					// Draw faces.
					for (i=0; i<this->facesSet->getNElements() ;i++)
					{
						faceId = *this->facesSet->at(i);
						this->drawFace(faceId, this->delaunay->getRefDcel());
					}
				}
			}
			else
			{
				// Draw voronoi and faces.
				this->drawVoronoi();
				this->setColor(YELLOW);
				if (!error)
				{
					for (i=0; i<this->facesSet->getNElements() ;i++)
					{
						faceId = *this->facesSet->at(i);
						this->drawFace(faceId, this->voronoi->getRefDcel());
					}
				}
			}

			// Deallocate memory.
			delete line;
			break;
		}
		// Draw the triangulation filtering edges.
		case FILTEREDGES_DRAW:
		{
			this->drawDelaunay(this->config->getMinLengthEdge());
			for (i=0; i<this->pointsSet->getNElements() ;i++)
			{
				p = *this->pointsSet->at(i);
				this->draw(&p);
			}
			break;
		}
		// Draw the triangulation and the triangles circumcentres.
		case CIRCUMCENTRES_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawCircumcentres(this->dcel);
			break;
		}
		// Draw the triangulation and the triangles circumcentres.
		case EDGESCIRCLES_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawEdgesCircles(this->dcel);
			break;
		}
		// Draw triangulation information.
		case DCEL_INFO_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawInfo(this->dcel);
			break;
		}
		// Draw Voronoi information.
		case VORONOI_INFO_DRAW:
		{
			this->drawDelaunay(INVALID);
			this->drawVoronoi();
			this->drawInfo(this->voronoi->getRefDcel());
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
* Name: 	drawSet
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw the DCEL set of points.
***************************************************************************/
void Draw::drawSet()
{
	// Draw set of points.
	this->setPointSize(1.0);
	this->setColor(WHITE);
	this->drawPoints(this->dcel);
}

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
* Name: 	drawVoronoi
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw the Voronoi graph.
***************************************************************************/
void Draw::drawVoronoi()
{
	this->setPointSize(3.0);
	this->setColor(LIGHT_BLUE);
	this->draw(this->voronoi->getRefDcel(), INVALID);
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
* IN:		point		reference to point to draw.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: draws a point in the screen.
***************************************************************************/
void Draw::draw(PointT *point)
{
	// Draw point.
	this->startPoints();
	glVertex2f(point->getX(), point->getY());
	this->finish();
}

/***************************************************************************
* Name: 	drawPoints
* IN:		dcel		DCEL that stores set of points.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draws the set of vertex of the DCEL.
***************************************************************************/
void Draw::drawPoints(Dcel *dcel)
{
	int	i=0;			// Loop counter.

	this->startPoints();

	// Loop to draw points.
	for (i=0; i<dcel->getNVertex() ;i++)
	{
		this->draw(dcel->getRefPoint(i));
	}

	this->finish();
}

/***************************************************************************
* Name: 	draw
* IN:		line			line to draw.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw line between origin and destination points.
***************************************************************************/
void Draw::draw(Line *line)
{
	// Draw edges of current edge.
	this->startLine();

	// Draw first point.
	glVertex2f(line->getOrigin().getX(), line->getOrigin().getY());

	// Draw second point.
	glVertex2f(line->getDest().getX(), line->getDest().getY());

	this->finish();
}

/***************************************************************************
* Name: 	draw
* IN:		polygon		polygon to draw.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw lines between every pair of points.
***************************************************************************/
void Draw::draw(Polygon *polygon)
{
	int		i=0;			// Loop counter.
	Line	line;			// Temporary line.
	Point<TYPE> *p, *q;		// Temporary points.

	this->setColor(GREEN);

#ifdef DEBUG_DRAW_POLYLINE
	Logging::buildText(__FUNCTION__, __FILE__, "Number of points in convex hull:" );
	Logging::buildText(__FUNCTION__, __FILE__, polygon->getNElements());
	Logging::write(true, Info);
#endif
	// Computes distance between every pair of points.
	for (i=0; i<(polygon->getNElements()-1) ;i++)
	{
		p = polygon->at(i);
		q = polygon->at(i+1);
		line = Line(*p, *q);
		this->draw(&line);
#ifdef DEBUG_DRAW_POLYLINE
		Logging::buildText(__FUNCTION__, __FILE__, "Drawing point " );
		Logging::buildText(__FUNCTION__, __FILE__, i);
		Logging::write(true, Info);
#endif
	}

#ifdef DEBUG_DRAW_POLYLINE
	Logging::buildText(__FUNCTION__, __FILE__, "Drawing last line" );
	Logging::write(true, Info);
#endif

	// Close polygon.
	p = polygon->at(polygon->getNElements()-1);
	q = polygon->at(0);
	line = Line(*p, *q);
	this->draw(&line);
}

/***************************************************************************
* Name: 		draw
* IN:			circle		circle to draw
* 				fill		fill circle flag
* OUT:			NONE
* RETURN:		NONE
* GLOBAL:		NONE
* Description: 	draws circle.
***************************************************************************/
void Draw::draw(Circle *circle, bool fill)
{
	int		i=0;
	TYPE 	angle=0.0;

	this->startCircle();
	for(i=0; i<150; i++)
	{
		angle = i*2*PI/150;
		glVertex2f(circle->getRefCentre()->getX() + (cos(angle) * circle->getRadius()),
				   circle->getRefCentre()->getY() + (sin(angle) * circle->getRadius()));
	}

	this->finish();
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
* Name: 	drawEdge
* IN:		edgeIndex	index of the edge to draw.
* 			dcel		DCEL data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Draws the edge of the DCEL whose position is "edgeIndex".
***************************************************************************/
void Draw::drawEdge(int edgeIndex, Dcel *dcel)
{
	Point<TYPE> p1, p2;			// Points to draw.

	// Check edge is real.
	if (!dcel->hasNegativeVertex(edgeIndex+1))
	{
		// Get edge origin and destination points.
		dcel->getEdgePoints(edgeIndex, p1, p2);

		// Draw edge.
		this->startLine();

		// Draw first point.
		glVertex2f(p1.getX(), p1.getY());

		// Draw second point.
		glVertex2f(p2.getX(), p2.getY());

		this->finish();
	}
#ifdef DEBUG_DRAW_DRAWEDGE
	else
	{
    	Logging::buildText(__FUNCTION__, __FILE__, "Skipping edge because has no real vertex");
    	Logging::write(true, Error);
	}
#endif
}


/***************************************************************************
* Name: 	drawFace
* IN:		faceId		face id.
* 			dcel		DCEL data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draws the "faceId" face of the DCEL.
***************************************************************************/
void Draw::drawFace(int faceId, Dcel *dcel)
{
	int	edgeIndex=0;		// Edge index.
	int	firstEdgeIndex=0;
#ifdef DEBUG_DRAW_DRAWFACE
	Logging::buildText(__FUNCTION__, __FILE__, "Drawing face ");
	Logging::buildText(__FUNCTION__, __FILE__, faceId);
	Logging::write(true, Info);
#endif
	// Get edge in face.
	firstEdgeIndex = dcel->getFaceEdge(faceId)-1;
	edgeIndex = firstEdgeIndex;

	// Draw face loop.
	do
	{
#ifdef DEBUG_DRAW_DRAWFACE
		Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
		Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
		Logging::write(true, Info);
#endif
		// Draw edge.
		this->drawEdge(edgeIndex, dcel);

		// Get next edge.
		edgeIndex = dcel->getNext(edgeIndex)-1;
	} while (edgeIndex != firstEdgeIndex);
}

/***************************************************************************
* Name: 	draw
* IN:		face		pointer to face
* 			dcel		DCEL data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draws the triangle of the face. If any of the points is
* 				imaginary (incremental Delaunay algorithm) then the face
* 				is not drawn.
***************************************************************************/
void Draw::draw(Face *face, Dcel *dcel)
{
	// PENDING IS USED THIS FUNCTION?
//	int	 	edgeIndex=0;		// Edge index.
//	int		faceId=0;			// Face id.

	// Get face id.
	//faceId = dcel->getFace(face->getEdge()-1);

	/*
	// Check face is not imaginary.
	if (!dcel->imaginaryFace(faceId))
	{
		// Get face edge.
		edgeIndex = face->getEdge()-1;
		draw(dcel->getRefEdge(edgeIndex), dcel);
#ifdef DEBUG_DCEL_DRAW_FACE
    	Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
    	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
    	Logging::write(true, Info);
#endif

		// Get next edge.
		edgeIndex = dcel->getNext(edgeIndex)-1;
		draw(dcel->getRefEdge(edgeIndex), dcel);
#ifdef DEBUG_DCEL_DRAW_FACE
    	Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
    	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
    	Logging::write(true, Info);
#endif

		// Get last edge.
		edgeIndex = dcel->getNext(edgeIndex)-1;
		draw(dcel->getRefEdge(edgeIndex), dcel);
#ifdef DEBUG_DCEL_DRAW_FACE
    	Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
    	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
    	Logging::write(true, Info);
#endif
	}
#ifdef DEBUG_DCEL_DRAW_FACE
	else
	{
    	Logging::buildText(__FUNCTION__, __FILE__, "Skipping face because is not real");
    	Logging::write(true, Error);
	}
#endif*/
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
* Name: 		drawCircumcentres
* IN:			dcel			DCEL data.
* OUT:			NONE
* RETURN:		NONE
* GLOBAL:		NONE
* Description: 	draws the circles using the triangles of the input dcel
* 				as the basis for every circle. For every triangle the circle
* 				formed by its three points is drawn.
***************************************************************************/
void Draw::drawCircumcentres(Dcel *dcel)
{
	int		faceID=0;		  			// Loop counter.
	Circle	circle;						// Circle to draw.
	int		points[NPOINTS_TRIANGLE];	// Triangle points.

	// Set circles color.
	this->setColor(BLUE);
	this->setLineSize(1.0);

	// Draw all points of the set.
	for (faceID=1; faceID<dcel->getNFaces() ;faceID++)
	{
		// Skip imaginary faces.
        if (!dcel->imaginaryFace(faceID))
        {
        	// Get points of the triangle.
        	dcel->getFaceVertices(faceID, points);
#ifdef DEBUG_DRAW_CIRCUMCENTRES
			Logging::buildText(__FUNCTION__, __FILE__, "Drawing face ");
			Logging::buildText(__FUNCTION__, __FILE__, faceID);
			Logging::buildText(__FUNCTION__, __FILE__, " circumcentre whose points are ");
			Logging::buildText(__FUNCTION__, __FILE__, points[0]);
			Logging::buildText(__FUNCTION__, __FILE__, ",");
			Logging::buildText(__FUNCTION__, __FILE__, points[1]);
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, points[2]);
			Logging::write(true, Info);
#endif
            // Build circle.
        	circle = Circle(dcel->getRefPoint(points[0]-1),
							dcel->getRefPoint(points[1]-1),
							dcel->getRefPoint(points[2]-1));

        	// Draw circle.
        	this->draw(&circle, false);
        }
#ifdef DEBUG_DRAW_CIRCUMCENTRES
        else
        {
			Logging::buildText(__FUNCTION__, __FILE__, "Skipping imaginary face ");
			Logging::buildText(__FUNCTION__, __FILE__, faceID);
			Logging::write(true, Info);
        }
#endif
	}
}

/***************************************************************************
* Name: 	drawEdgesCircles
* IN:		dcel			DCEL data.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draws the triangulation and a circle for each edge.
***************************************************************************/
void Draw::drawEdgesCircles(Dcel *dcel)
{
	int		edgeIndex=0;        // Edge index.
	int		nEdges=0;			// # edges in the DCEL.
	TYPE  	radius=0.0;			// Circle radius.
	Line	line;				// Edge line.
	Circle	circle;				// Circle to draw.
	Point<TYPE> origin, dest;  	// Origin and destination points.
	Point<TYPE> middle;	    	// Edge middle point.

	this->setLineSize(1.0);
	this->setColor(BLUE);

    // Loop all faces (but external).
	nEdges = dcel->getNEdges();
	for (edgeIndex=0; edgeIndex<nEdges ;edgeIndex++)
	{
		// Skip imaginary edges.
        if (!dcel->hasNegativeVertex(edgeIndex+1))
        {
        	// Create line.
        	dcel->getEdgePoints(edgeIndex, origin, dest);
        	// PENDING REMOVE CODE.
        	//origin  = *dcel->getRefPoint(dcel->getOrigin(edgeIndex)-1);
        	//dest    = *dcel->getRefPoint(dcel->getOrigin(dcel->getTwin(edgeIndex)-1)-1);
        	line = Line(origin, dest);

            // Compute middle point of edge.
        	line.getMiddle(middle);

            // Create circle
        	radius = origin.distance(middle);
        	circle = Circle(&middle, radius);

            // Draw circle.
        	this->draw(&circle, false);
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

/***************************************************************************
* Name: 		setFont
* IN:			newFont		font to write text.
* OUT:			NONE
* RETURN:		NONE
* GLOBAL:		NONE
* Description: 	set the text font.
***************************************************************************/
void Draw::setFont(void *newFont)
{
	font = newFont;
}

/***************************************************************************
* Name: 	drawText
* IN:		x			x coordinate to write text
* 			y			y coordinate to write text
* 			string		text to print
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	writes the "string" text at position (x,y) using "font" and
* 				r,g,b color.
***************************************************************************/
void Draw::drawText(float x, float y, char *string)
{
	int		i=0;		// Loop counter.
	int		len=0;		// Array length.

	// Set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	// Get the length of the string to display
	len = (int) strlen(string);

	//loop to display character by character
	for (i=0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

/***************************************************************************
* Name: 	drawPointsInfo
* IN:		dcel	dcel data.
* OUT:		NONE
* IN/OUT:	NONE
* RETURN:	NONE
* Description: draw point identifiers.
***************************************************************************/
void Draw::drawPointsInfo(Dcel *dcel)
{
	int			i=0;		// Loop counter.
	Point<TYPE>	*point;		// Current point.
	char	text_Info[50];

#ifdef DEBUG_DRAW_POINTS_INFO
	Logging::buildText(__FUNCTION__, __FILE__, "# points to draw: ");
	Logging::buildText(__FUNCTION__, __FILE__, dcel->getNEdges());
	Logging::write(true, Info);
#endif

	// Draw all points of the set.
	for (i=0; i<dcel->getNVertex() ;i++)
	{
		// Get and draw i-point.
		point = dcel->getRefPoint(i);
		sprintf(text_Info, "%d", i+1);
		this->drawText(point->getX(), point->getY(), text_Info);
#ifdef DEBUG_DRAW_POINTS_INFO
		Logging::buildText(__FUNCTION__, __FILE__, "Drawing point ");
		Logging::buildText(__FUNCTION__, __FILE__, point);
		Logging::write(true, Info);
#endif

	}
}

/***************************************************************************
* Name: 	drawEdgesInfo
* IN:		dcel	dcel data.
* OUT:		NONE
* IN/OUT:	NONE
* RETURN:	NONE
* Description: draw edges identifiers.
***************************************************************************/
void Draw::drawEdgesInfo(Dcel *dcel)
{
	int		edgeIndex=0;		// Edge index.
    char	text_Info[50];		// Text to print.
    Point<TYPE> origin, dest;	// Extreme points of edges.
    Point<TYPE> middle;         // Middle point of the edge.

#ifdef DEBUG_DRAW_FACES_INFO
	Logging::buildText(__FUNCTION__, __FILE__, "# edges to draw: ");
	Logging::buildText(__FUNCTION__, __FILE__, dcel->getNEdges());
	Logging::write(true, Info);
#endif

	// Loop all edges.
	for (edgeIndex=0; edgeIndex<dcel->getNEdges() ;edgeIndex++)
	{
		// Check if twin edge already visited.
		if ((edgeIndex+1) < dcel->getTwin(edgeIndex))
        {
			// Check edge is real.
			if (!dcel->hasNegativeVertex(edgeIndex+1))
			{
				// Get edge extreme points.
				dcel->getEdgePoints(edgeIndex, origin, dest);

				// Compute middle point of edge.
				Point<TYPE>::middlePoint(&origin, &dest, &middle);

				// Print information.
				sprintf(text_Info, "%d - %d", edgeIndex+1, dcel->getTwin(edgeIndex));
				this->drawText(middle.getX(), middle.getY(), text_Info);
#ifdef DEBUG_DRAW_EDGES_INFO
				Logging::buildText(__FUNCTION__, __FILE__, "Drawing edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::write(true, Info);
#endif
			}
#ifdef DEBUG_DRAW_EDGES_INFO
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Negative vertex in edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::write(true, Info);
			}
#endif
        }
#ifdef DEBUG_DRAW_EDGES_INFO
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Twin edge already drawn ");
			Logging::buildText(__FUNCTION__, __FILE__, dcel->getTwin(edgeIndex));
			Logging::write(true, Info);
		}
#endif
	}
}

/***************************************************************************
* Name: 	drawFacesInfo
* IN:		NONE
* OUT:		NONE
* IN/OUT:	NONE
* RETURN:	NONE
* Description: draw faces identifiers.
***************************************************************************/
void Draw::drawFacesInfo(Dcel *dcel)
{
    int		faceId=0;			// Loop counter.
    char	text_Info[50];		// Text to print.
    int		edgeIndex=0;		// First edge in current face.
    int		firstEdgeIndex=0;	// Current edge in current face.
    Queue<PointT> points(10);	// List of points.
    Polygon polygon(10);
    Point<TYPE> origin;			// Edge origin point.
    Point<TYPE> p1, p2, p3;		// Temporary points.
    Point<TYPE> center;			// Middle point of the edge.

#ifdef DEBUG_DRAW_FACES_INFO
	Logging::buildText(__FUNCTION__, __FILE__, "# faces to draw: ");
	Logging::buildText(__FUNCTION__, __FILE__, dcel->getNFaces());
	Logging::write(true, Info);
#endif

	// Loop all faces (skip external face).
	for (faceId=0; faceId<dcel->getNFaces() ;faceId++)
	{
		// If any vertex is imaginary then face is not drawn.
		if (!dcel->imaginaryFace(faceId))
		{
#ifdef DEBUG_DRAW_FACES_INFO
			Logging::buildText(__FUNCTION__, __FILE__,"Drawing face id ");
			Logging::buildText(__FUNCTION__, __FILE__, faceId);
			Logging::write(true, Info);
#endif
			// Get edge in current face.
			firstEdgeIndex = dcel->getFaceEdge(faceId)-1;
			edgeIndex = 0;
			edgeIndex = firstEdgeIndex;
			do
			{
#ifdef DEBUG_DRAW_FACES_INFO
				Logging::buildText(__FUNCTION__, __FILE__,"Adding point ");
				Logging::buildText(__FUNCTION__, __FILE__, dcel->getOrigin(edgeIndex));
				Logging::write(true, Info);
#endif
				// Add origin point to polygon.
				origin = *dcel->getRefPoint(dcel->getOrigin(edgeIndex)-1);
				polygon.add(&origin);

				// Next edge in face.
				edgeIndex = dcel->getNext(edgeIndex)-1;
			} while(edgeIndex != firstEdgeIndex);

#ifdef DEBUG_DRAW_FACES_INFO
			Logging::buildText(__FUNCTION__, __FILE__,"Number of points in polygon is ");
			Logging::buildText(__FUNCTION__, __FILE__, points.getNElements());
			Logging::write(true, Info);
#endif
			// Compute face centroid.
			polygon.centroid(center);
			polygon.reset();

			// Print information.
			sprintf(text_Info, "%d", faceId);
			this->drawText(center.getX(), center.getY(), text_Info);
		}
#ifdef DEBUG_DRAW_FACES_INFO
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Face not drawn because of negative vertex. Face id ");
			Logging::buildText(__FUNCTION__, __FILE__, faceId);
			Logging::write(true, Info);
		}
#endif
	}
}

/***************************************************************************
* Name: 	drawInfo
* IN:		dcel		DCEL data.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw the DCEL data and all its information.
***************************************************************************/
void Draw::drawInfo(Dcel *dcel)
{
	// Points information.
	this->setColor(WHITE);
	this->setFont(GLUT_BITMAP_HELVETICA_10);
	this->drawPointsInfo(dcel);

	// Edges information.
	this->setColor(LIGHT_BLUE);
	this->setFont(GLUT_BITMAP_HELVETICA_12);
	this->drawEdgesInfo(dcel);

	// Faces information.
	this->setColor(YELLOW);
	this->setFont(GLUT_BITMAP_HELVETICA_18);
	this->drawFacesInfo(dcel);
}

/***************************************************************************
* Name: 	drawPath
* IN:		dcel		DCEL data.
* 			p1			path origin point.
* 			p2			path destination point.
* 			queue		list of faces in the path.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	draw the DCEL data, the segment between p1 and p2 and
* 				highlights the faces between both points.
***************************************************************************/
void Draw::drawPath(Dcel *dcel, Point<TYPE> *p1, Point<TYPE> *p2, Queue<int> &queue)
{
	int		i=0;			// Counters.
	int		nElements=0;	// # elements in queue.
	Line	line;			// Segment between two points.

	// Draw line.
	line = Line(*p1, *p2);
	this->setColor(YELLOW);
	this->draw(&line);

	// Draw input DCEL.
	this->draw(dcel, INVALID);

	// Draw faces.
	nElements = queue.getNElements();
	for (i=0; i<nElements ;i++)
	{
		// PENDING Draw Voronoi face.
		printf("Face %d is %d\n", i+1, queue.at(i));
	}
}

void refresh(void)
{
	glutPostRedisplay();
}

