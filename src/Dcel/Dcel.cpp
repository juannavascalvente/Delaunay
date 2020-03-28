/*
 * Dcel.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#include "Dcel.h"
#include "Logging.h"

#include <cstring>
#include <iostream>

using namespace std;

#define HEADERS 10

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_POINTS_GET_FACE_POINTS
//#define DEBUG_DCEL_FILE
//#define DEBUG_DRAW_FACES_INFO
//#define DEBUG_READPOINTS
//#define DEBUG_WRITE_DCEL
//#define DEBUG_READ_DCEL_POINTS
//#define DEBUG_READ_DCEL
//#define DEBUG_RETURN_TURN
//#define DEBUG_SETHIGHEST_FIRST
//#define DEBUG_SETLOWESTS_FIRST
//#define DEBUG_GENERATE_CLUSTER
//#define DEBUG_GENERATE_RANDOM_DCEL
//#define DEBUG_SELECT_COLLINEAR_EDGE
//#define DEBUG_HAS_NEGATIVE_VERTEX
//#define DEBUG_IS_EXTERNAL_EDGE
//#define DEBUG_ADD_EDGE
//#define DEBUG_UPDATE_EDGE
//#define DEBUG_ADD_EDGE_EDGE
//#define DEBUG_DCEL_ADDVERTEX
//#define DEBUG_UPDATE_VERTEX_EDGE_AT
//#define DEBUG_DCEL_RESIZE
//#define DEBUG_DCEL_CONSTRUCTOR
//#define DEBUG_ADD_FACE
//#define DEBUG_DCELIMAGINARYFACE
//#define DEBUG_DCEL_BOTTOMMOSTFACE
//#define DEBUG_DELAUNAY_FIND_PATH
//#define DEBUG_DCEL_GET_EDGE_POINTS
//#define DEBUG_READBINARY_DCEL
//#define DEBUG_WRITEBINARY_DCEL
//#define DEBUG_DCEL_EDGE_INTERSECTION
#define DEBUG_DCEL_EQUAL
#endif


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/*------------------------------------------------------------------------
  Constructor/Destructor.
------------------------------------------------------------------------*/
Dcel::Dcel()
{
	// Initialize attributes.
	this->created = false;
	this->incremental = false;
	this->nVertex = 0;
	this->sizeVertex = 0;
	this->vertex = nullptr;
	this->nEdges = 0;
	this->sizeEdges = 0;
	this->edges = nullptr;
	this->nFaces = 0;
	this->sizeFaces = 0;
	this->faces = nullptr;
	this->minX = 0.0;
	this->minY = 0.0;
	this->maxX = 0.0;
	this->maxY = 0.0;
}


Dcel::Dcel(int nPoints, int nEdges, int nFaces)
{
	// Initialize max and min X and Y coordinates.
	this->minX = 0.0;
	this->minY = 0.0;
	this->maxX = 0.0;
	this->maxY = 0.0;

    try
    {
    	this->incremental = false;

		// Initialize vertex.
		this->nVertex = 0;
		this->sizeVertex = nPoints;
		this->vertex = new Vertex[this->sizeVertex];

		// Initialize edges attributes.
		this->nEdges = 0;
		this->sizeEdges = nEdges;
		if (this->sizeEdges == INVALID)
		{
			// Max number of edges is = 3xnPoints - 6.
			// NOTE: It is 6xnPoints because there are two directions per edge.
			this->sizeEdges = 6*(this->sizeVertex+2) - 6;
		}
		this->edges = new Edge[this->sizeEdges];

		// Initialize faces attributes. If # vertices and # edges are known then # faces = 2 - #v + #e
		this->nFaces = 0;
		this->sizeFaces = nFaces;
		if (nFaces == INVALID)
		{
			this->sizeFaces = 2 - this->sizeVertex + this->sizeEdges;
		}
		this->faces = new Face[this->sizeFaces];

#ifdef DEBUG_DCEL_CONSTRUCTOR
		Logging::buildText(__FUNCTION__, __FILE__, "Allocating ");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeVertex);
		Logging::buildText(__FUNCTION__, __FILE__, " points, ");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeEdges);
		Logging::buildText(__FUNCTION__, __FILE__, " edges and ");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeFaces);
		Logging::buildText(__FUNCTION__, __FILE__, " faces.");
		Logging::write(true, Info);
#endif
		this->created = true;
    }
    catch (exception &ex)
    {
    	this->invalidate();

    	ex.what();
#ifdef LOGGING
    	// PENDING RETURN ERROR IF MEMORY NOT ALLOCATED.
    	Logging::buildText(__FUNCTION__, __FILE__, "Error allocating data.\n");
    	Logging::write(true, Error);
#endif
	}
}


Dcel::~Dcel()
{
	// Reset attributes.
	this->created = false;
	this->incremental = false;
	this->nVertex = 0;
	this->sizeVertex = 0;
	this->nEdges = 0;
	this->sizeEdges = 0;
	this->nFaces = 0;
	this->sizeFaces = 0;

	// Deallocate data.
    delete[] this->vertex;
	delete[] this->edges;
	delete[] this->faces;
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	imaginaryFace
* IN:		edgeIndex			input edge index.
* OUT:		origin				origin point of the edge.
* 			dest				destination point of the edge.
* IN/OUT:	NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	gets the origin and destination points of the input edge
* 				index "edgeIndex".
***************************************************************************/
// PENDING WHY IS OUT OF BOUNDS WHEN PRINTIN DCEL INFO?
void Dcel::getEdgePoints(int edgeIndex, Point<TYPE> &origin, Point<TYPE> &dest) const
{
#ifdef DEBUG_OUTOFBOUNDS_EXCEPTION
	if (edgeIndex >= this->getNEdges())
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Edge index out of bounds");
		Logging::buildText(__FUNCTION__, __FILE__, edgeIndex);
		Logging::write(true, Info);
	}
#endif

	// Get origin and destination points of the edge.
	origin = *this->getRefPoint(this->getOrigin(edgeIndex)-1);
	dest   = *this->getRefPoint(this->getOrigin(this->getTwin(edgeIndex)-1)-1);
#ifdef DEBUG_DCEL_GET_EDGE_POINTS
	Logging::buildText(__FUNCTION__, __FILE__, "Edge extreme points are ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(edgeIndex));
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(this->getTwin(edgeIndex)-1));
	Logging::write(true, Info);
#endif
}

/***************************************************************************
* Name: 	imaginaryFace
* IN:		faceId			face identifier to check
* OUT:		NONE
* RETURN:	true if any of the vertex of the face is negative.
* GLOBAL:	NONE
* Description: 	checks if any of the vertex of the face is imaginary (only
* 				in incremental Delaunay triangulations).
***************************************************************************/
bool Dcel::imaginaryFace(int faceId) const
{
	bool	imaginary=false;		// Return value.
	int		edgeIndex=0;			// Edge index.

	// Get index edge from face.
	edgeIndex = this->getFaceEdge(faceId) - 1;

	if ((faceId == EXTERNAL_FACE) ||
		(this->getOrigin(edgeIndex) < 0) ||
		(this->getOrigin(this->getNext(edgeIndex)-1) < 0) ||
		(this->getOrigin(this->getPrevious(edgeIndex)-1) < 0))
	{
#ifdef DEBUG_DCELIMAGINARYFACE
		Logging::buildText(__FUNCTION__, __FILE__, "Face ");
		Logging::buildText(__FUNCTION__, __FILE__, faceId);
		Logging::buildText(__FUNCTION__, __FILE__, " is imaginary. Vertices: (");
		Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(edgeIndex));
		Logging::buildText(__FUNCTION__, __FILE__, ",");
		Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(this->getNext(edgeIndex)-1));
		Logging::buildText(__FUNCTION__, __FILE__, ",");
		Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(this->getPrevious(edgeIndex)-1));
		Logging::buildText(__FUNCTION__, __FILE__, ")");
		Logging::write(true, Info);
#endif
		imaginary = true;
	}

	return(imaginary);
}

// PENDING: MOVE TO DELAUNAY INSTEAD OF DCEL
/***************************************************************************
* Name: 	isBottomMostFace
* IN:		faceId		face identifier to check
* OUT:		NONE
* RETURN:	true 		if faceId is the bottom most face
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	checks if input face is the bottom most face. This face has
* 				2 imaginary vertices.
***************************************************************************/
bool Dcel::isBottomMostFace(int faceId) const
{
	int	nImaginaryFaces=0;		// # imaginary vertices.
	int	edgeIndex=0;			// Edge index.

	// Get index edge from face.
	edgeIndex = this->getFaceEdge(faceId) - 1;

	if (this->getOrigin(edgeIndex) < 0)
	{
		nImaginaryFaces++;
	}
	if (this->getOrigin(this->getNext(edgeIndex)-1) < 0)
	{
		nImaginaryFaces++;
	}
	if (this->getOrigin(this->getPrevious(edgeIndex)-1) < 0)
	{
		nImaginaryFaces++;
	}
#ifdef DEBUG_DCEL_BOTTOMMOSTFACE
	Logging::buildText(__FUNCTION__, __FILE__, "# imaginary vertices in face ");
	Logging::buildText(__FUNCTION__, __FILE__, faceId);
	Logging::buildText(__FUNCTION__, __FILE__, " is ");
	Logging::buildText(__FUNCTION__, __FILE__, nImaginaryFaces);
	Logging::write(true, Info);
#endif

	return(nImaginaryFaces == 2);
}

// PENDING: MOVE TO DELAUNAY INSTEAD OF DCEL
/***************************************************************************
* Name: 	getBottomMostFaceNeighborFaces
* IN:		faceId			face identifier to check
* OUT:		faces			set of neighbor faces (only 2).
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	returns the face identifiers of the "faceId". The input face
* 				is the bottom most point and so it has only two neighbor
* 				faces (the third is the external face).
***************************************************************************/
void Dcel::getBottomMostFaceNeighborFaces(int faceId, Set<int> &faces) const
{
	int	edgeIndex=0;		// Edge index.

	if (this->isBottomMostFace(faceId))
	{
		// Get edge face.
		edgeIndex = this->getFaceEdge(faceId) - 1;
		do
		{
#ifdef DEBUG_DCEL_BOTTOMMOSTFACE
			Logging::buildText(__FUNCTION__, __FILE__, "Checking edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex);
			Logging::write(true, Info);
#endif
			// Skip edge whose origin is point -2.
			if (this->getOrigin(edgeIndex) != P_MINUS_2)
			{
				faces.add(this->getFace(this->getTwin(edgeIndex)-1));
#ifdef DEBUG_DCEL_BOTTOMMOSTFACE
				Logging::buildText(__FUNCTION__, __FILE__, "Edge origin ");
				Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(edgeIndex));
				Logging::buildText(__FUNCTION__, __FILE__, " is not -2. Adding face ");
				Logging::buildText(__FUNCTION__, __FILE__, this->getFace(this->getTwin(edgeIndex)-1));
				Logging::write(true, Info);
#endif
			}
			// Get next edge.
			edgeIndex = this->getNext(edgeIndex) - 1;
		} while(faces.getNElements() < 2);
	}
}

/***************************************************************************
* Name: 	getFaceVertices
* IN:		faceIndex			face identifier to check
* OUT:		*ids				array of indexes
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	returns the face identifiers of the "faceIndex" face.
***************************************************************************/
void Dcel::getFaceVertices(int faceIndex, int *ids) const
{
	int		edgeIndex=0;			// Edge index.

	// Get index edge from face.
	edgeIndex = this->getFaceEdge(faceIndex) - 1;

	// Get face vertices.
	ids[0] = this->getOrigin(edgeIndex);
	ids[1] = this->getOrigin(this->getNext(edgeIndex)-1);
	ids[2] = this->getOrigin(this->getPrevious(edgeIndex)-1);
}

/***************************************************************************
* Name: 	getFacePoints
* IN:		faceIndex			face identifier to check
* OUT:		p					1st face vertex point.
* 			q					2nd face vertex point
* 			r					3rd face vertex point
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	returns the face vertices of the "faceIndex" face.
***************************************************************************/
void Dcel::getFacePoints(int faceIndex, Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r)
{
	int		edgeIndex=0;			// Edge index.

	// Get index edge from face.
	edgeIndex = this->getFaceEdge(faceIndex) - 1;

#ifdef DEBUG_POINTS_GET_FACE_POINTS
	Logging::buildText(__FUNCTION__, __FILE__, "Face ");
	Logging::buildText(__FUNCTION__, __FILE__, faceIndex);
	Logging::buildText(__FUNCTION__, __FILE__, " points id are ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getOrigin(edgeIndex));
	Logging::buildText(__FUNCTION__, __FILE__, ",");
	Logging::buildText(__FUNCTION__, __FILE__, this->getNext(edgeIndex));
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getPrevious(edgeIndex));
	Logging::write(true, Info);
#endif

	// Get face vertices.
	p = *this->getRefPoint(this->getOrigin(edgeIndex)-1);
	q = *this->getRefPoint(this->getOrigin(this->getNext(edgeIndex)-1)-1);
	r = *this->getRefPoint(this->getOrigin(this->getPrevious(edgeIndex)-1)-1);
}

/***************************************************************************
* Name: 	reset
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	set the # elements in DCEL to zero.
***************************************************************************/
void Dcel::reset()
{
	// Reset counters.
	this->nVertex = 0;
	this->nEdges = 0;
	this->nFaces = 0;
#ifdef DEBUG_DCEL_RESET
	Logging::buildText(__FUNCTION__, __FILE__, "DCEL data reset");
	Logging::write(true, Info);
#endif
}

/***************************************************************************
* Name: 	clean
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	cleans all DCEL data but the points coordinates.
***************************************************************************/
void Dcel::clean()
{
	int	i=0;				// Loop counter.

	// Clean vectors data.
	for (i=0; i<this->nVertex; i++)
	{
		this->vertex[i].setOrigin(INVALID);
	}
	memset((void *) this->edges, 0, sizeof(Vertex)*this->sizeVertex);
	memset((void *) this->faces, 0, sizeof(Face)*this->sizeFaces);

	// Reset edges and faces number of elements.
	this->nEdges = 0;
	this->nFaces = 0;
#ifdef DEBUG_DCEL_CLEAN
	Logging::buildText(__FUNCTION__, __FILE__, "DCEL data reset");
	Logging::write(true, Info);
#endif
}

/***************************************************************************
* Name: 	resize
* IN:		size	size of new vector
* 			copy	flag to copy data to new location
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	resizes the dcel and copy data to new location if required.
***************************************************************************/
void Dcel::resize(int size, bool copy)
{
	try
	{
#ifdef DEBUG_DCEL_RESIZE
		Logging::buildText(__FUNCTION__, __FILE__, "Resizing to size ");
		Logging::buildText(__FUNCTION__, __FILE__, size);
		Logging::buildText(__FUNCTION__, __FILE__, " and current size is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->getSizeVertex());
		Logging::buildText(__FUNCTION__, __FILE__, " and current # elements is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->getNVertex());
		Logging::write(true, Info);
#endif
		// Check if new size is higher than current.
		if (size > this->sizeVertex)
		{
			Vertex 	*tmpPoints;
			Edge 	*tmpEdges;
			Face 	*tmpFaces;

			// Check if current content must be copied.
			if (copy)
			{
#ifdef DEBUG_DCEL_RESIZE
				Logging::buildText(__FUNCTION__, __FILE__, "Copying existing data");
				Logging::write(true, Info);
#endif
				tmpPoints = this->vertex;
				tmpEdges = this->edges;
				tmpFaces = this->faces;
			}
			// If not -> reset # elements.
			else
			{
#ifdef DEBUG_DCEL_RESIZE
				Logging::buildText(__FUNCTION__, __FILE__, "Reseting data");
				Logging::write(true, Info);
#endif
				this->nVertex = 0;
				this->nEdges = 0;
				this->nFaces = 0;
			}

			// New size unknown -> allocate twice current space.
			if (size == INVALID)
			{
#ifdef DEBUG_DCEL_RESIZE
				Logging::buildText(__FUNCTION__, __FILE__, "Doubling size");
				Logging::write(true, Info);
#endif
				this->sizeVertex = this->sizeVertex*2;
			}
			else
			{
				this->sizeVertex = size;
			}
			this->sizeEdges = 6*(this->sizeVertex+2) - 6;
			this->sizeFaces = 2 - this->sizeVertex + this->sizeEdges;

			// PENDING What to do if size is negative?
			// Check if new size is negative.
			if (this->sizeVertex < 0)
			{
				Logging::buildText(__FUNCTION__, __FILE__, "New DCEL size is negative: ");
				Logging::buildText(__FUNCTION__, __FILE__, this->sizeVertex);
				Logging::write(true, Error);
			}
			else
			{
				// Allocate new memory.
				tmpPoints = new Vertex[this->sizeVertex];
				tmpEdges = new Edge[this->sizeEdges];
				tmpFaces = new Face[this->sizeFaces];

				// copy current data.
				if (copy)
				{
					// Copy current data.
					memcpy((void *) tmpPoints, (void *) this->vertex, this->nVertex*sizeof(Vertex));
					memcpy((void *) tmpEdges, (void *) this->edges, this->nEdges*sizeof(Edge));
					memcpy((void *) tmpFaces, (void *) this->faces, this->nFaces*sizeof(Face));
				}

				// Deallocate data.
				if (this->created)
				{
					delete[] this->vertex;
					delete[] this->edges;
					delete[] this->faces;
				}

				// Reference new data.
				this->vertex = tmpPoints;
				this->edges = tmpEdges;
				this->faces = tmpFaces;

#ifdef DEBUG_DCEL_RESIZE
				Logging::buildText(__FUNCTION__, __FILE__, "Resizing ");
				Logging::buildText(__FUNCTION__, __FILE__, this->sizeVertex);
				Logging::buildText(__FUNCTION__, __FILE__, " points, ");
				Logging::buildText(__FUNCTION__, __FILE__, this->sizeEdges);
				Logging::buildText(__FUNCTION__, __FILE__, " edges and ");
				Logging::buildText(__FUNCTION__, __FILE__, this->sizeFaces);
				Logging::buildText(__FUNCTION__, __FILE__, " faces.");
				Logging::write(true, Info);
#endif
				this->created = true;
			}
		}
		else
		{
			this->nVertex = 0;
			this->nEdges = 0;
			this->nFaces = 0;
#ifdef DEBUG_DCEL_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "No resize required. New size ");
			Logging::buildText(__FUNCTION__, __FILE__, size);
			Logging::buildText(__FUNCTION__, __FILE__, " is not higher than current ");
			Logging::buildText(__FUNCTION__, __FILE__, this->sizeVertex);
			Logging::write(true, Info);
#endif
		}
	}
	catch (bad_alloc &ex)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory. New # vertices is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeVertex);
		Logging::write(true, Error);
	}
	catch (exception &ex)
	{
		std::cout << ex.what();
	}
}

/***************************************************************************
* Name: 	invalidate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	resets all object fields.
***************************************************************************/
void Dcel::invalidate()
{
	// Set invalid and default values.
	this->created = false;
	this->incremental = false;
	this->nVertex = INVALID;
	this->sizeVertex = INVALID;
	this->vertex = nullptr;
	this->nEdges = INVALID;
	this->sizeEdges = INVALID;
	this->edges = nullptr;
	this->nFaces = INVALID;
	this->sizeFaces = INVALID;
	this->faces = nullptr;
}

/***************************************************************************
* Name: 	addVertex
* IN:		p			new point coordinates
* 			edge		new edge identifier.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new point to the vertex array.
***************************************************************************/
void Dcel::addVertex(const Point<TYPE> *p, const int edge)
{
	try
	{
#ifdef DEBUG_DCEL_ADDVERTEX
		Logging::buildText(__FUNCTION__, __FILE__, "Adding vector to position ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nVertex+1);
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeVertex);
		Logging::write(true, Info);
#endif
		// Add vertex and increase # vertex.
		this->vertex[this->nVertex] = Vertex(edge, *p);
		this->nVertex++;
	}
	catch (exception &ex)
	{
		ex.what();
	}
}

/***************************************************************************
* Name: 	addVertex
* IN:		vertex			new vertex to add
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new vertex to the vertex array.
***************************************************************************/
void Dcel::addVertex(const Vertex *vertex)
{
	try
	{
		// Add vertex and increase # vertex.
		this->vertex[this->nVertex] = (*vertex);
		this->nVertex++;
	}
	catch (exception &ex)
	{
		ex.what();
	}
}

/***************************************************************************
* Name: 	updateVertex
* IN:		edgeID			new edge to set for point
* 			index			point index to update
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	sets the edgeId as the edge departing from point at "index"
* 				position.
***************************************************************************/
void Dcel::updateVertex(int edge_ID, int index)
{
	// Check if index is out of bounds.
	if (index < this->sizeVertex)
	{
		// Update vertex edge.
		this->vertex[index].setOrigin(edge_ID);
#ifdef DEBUG_UPDATE_VERTEX_EDGE_AT
		Logging::buildText(__FUNCTION__, __FILE__, "Update vertex ");
		Logging::buildText(__FUNCTION__, __FILE__, index+1);
		Logging::buildText(__FUNCTION__, __FILE__, " origin edge to ");
		Logging::buildText(__FUNCTION__, __FILE__, edge_ID);
		Logging::write(true, Info);
#endif
	}
	else
	{
		// PENDING: RESIZE? EXCEPTION?
		Logging::buildText(__FUNCTION__, __FILE__, "Index ");
		Logging::buildText(__FUNCTION__, __FILE__, index);
		Logging::buildText(__FUNCTION__, __FILE__, " out of bounds: ");
		Logging::buildRange(__FUNCTION__, __FILE__, 0, this->sizeVertex);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	updateVertex
* IN:		p			reference to point to copy.
* 			index		point index to update
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	sets the edgeId as the edge departing from point at "index"
* 				position.
***************************************************************************/
void Dcel::updateVertex(Point<TYPE> *p, int index)
{
	// Check if index is out of bounds.
	if (index < this->sizeVertex)
	{
		// Update vertex edge.
		this->vertex[index].setPoint(p);
#ifdef DEBUG_UPDATE_VERTEX_EDGE_AT
		Logging::buildText(__FUNCTION__, __FILE__, "Vertex ");
		Logging::buildText(__FUNCTION__, __FILE__, index+1);
		Logging::buildText(__FUNCTION__, __FILE__, " updated point to ");
		Logging::buildText(__FUNCTION__, __FILE__, p);
		Logging::write(true, Info);
#endif
	}
	else
	{
		// PENDING: RESIZE? EXCEPTION?
		Logging::buildText(__FUNCTION__, __FILE__, "Index ");
		Logging::buildText(__FUNCTION__, __FILE__, index);
		Logging::buildText(__FUNCTION__, __FILE__, " out of bounds: ");
		Logging::buildRange(__FUNCTION__, __FILE__, 0, this->sizeVertex);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	addEdge
* IN:		edge			new edge to add
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new edge to the edges array.
***************************************************************************/
void Dcel::addEdge(const Edge *edge)
{
	// Check if edges vector is full.
	if (this->nEdges < this->sizeEdges)
	{
		// Add edge and increase # edges.
		this->edges[this->nEdges] = (*edge);
		this->nEdges++;
#ifdef DEBUG_ADD_EDGE_EDGE
		Logging::buildText(__FUNCTION__, __FILE__, "Edges vector size: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeEdges);
		Logging::buildText(__FUNCTION__, __FILE__, ". Added edge ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nEdges);
		Logging::buildText(__FUNCTION__, __FILE__, " with data ");
		Logging::buildText(__FUNCTION__, __FILE__, this->edges[this->nEdges-1].toStr());
		Logging::write(true, Info);
#endif
	}
	else
	{
		// PENDING: RESIZE? EXCEPTION?
		Logging::buildText(__FUNCTION__, __FILE__, "Edges vector is full.");
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	addEdge
* IN:		origin		edge origin point
* 			twin		twin edge
* 			previous	previous edge
* 			next		next edge
* 			face		edge face
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new edge to the edges array.
***************************************************************************/
void Dcel::addEdge(int origin, int twin, int previous, int next, int face)
{
	// Check if faces vector is full.
	if (this->nEdges < this->sizeEdges)
	{
		// Add edge and increase # edges.
		this->edges[this->nEdges] = Edge(origin, twin, previous, next, face);
		this->nEdges++;
#ifdef DEBUG_ADD_EDGE
		Logging::buildText(__FUNCTION__, __FILE__, "Edges vector size: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeEdges);
		Logging::buildText(__FUNCTION__, __FILE__, ". Added edge ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nEdges);
		Logging::buildText(__FUNCTION__, __FILE__, " with data ");
		Logging::buildText(__FUNCTION__, __FILE__, this->edges[this->nEdges-1].toStr());
		Logging::write(true, Info);
#endif
	}
	else
	{
		// PENDING: RESIZE? EXCEPTION?
		Logging::buildText(__FUNCTION__, __FILE__, "Edges vector is full.");
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	updateEdge
* IN:		origin		edge origin point
* 			twin		twin edge
* 			previous	previous edge
* 			next		next edge
* 			face		edge face
* 			index		edge index to update
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	updates the edge fields. If any input value is "NO_UPDATE"
* 				then that value is not updated.
***************************************************************************/
void Dcel::updateEdge(int origin, int twin, int previous, int next, int face, int index)
{
	// Check if faces vector is full.
	if (index < this->sizeEdges)
	{
#ifdef DEBUG_UPDATE_EDGE
		Logging::buildText(__FUNCTION__, __FILE__, "Updating edge ");
		Logging::buildText(__FUNCTION__, __FILE__, index+1);
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->sizeEdges);
		Logging::buildText(__FUNCTION__, __FILE__, ". Fields updated are:");
		Logging::write(true, Info);
		Logging::buildText(__FUNCTION__, __FILE__, "Origin ");
		Logging::buildText(__FUNCTION__, __FILE__, origin);
		Logging::buildText(__FUNCTION__, __FILE__, ".Twin ");
		Logging::buildText(__FUNCTION__, __FILE__, twin);
		Logging::buildText(__FUNCTION__, __FILE__, ".Previous ");
		Logging::buildText(__FUNCTION__, __FILE__, previous);
		Logging::buildText(__FUNCTION__, __FILE__, ".Next ");
		Logging::buildText(__FUNCTION__, __FILE__, next);
		Logging::buildText(__FUNCTION__, __FILE__, ".Face ");
		Logging::buildText(__FUNCTION__, __FILE__, face);
		Logging::write(true, Info);
#endif
		// Check if origin point field must be updated.
		if (origin != NO_UPDATE)
		{
			this->edges[index].setOrigin(origin);
		}

		// Check if twin edge field must be updated.
		if (twin != NO_UPDATE)
		{
			this->edges[index].setTwin(twin);
		}

		// Check if previous edge field must be updated.
		if (previous != NO_UPDATE)
		{
			this->edges[index].setPrevious(previous);
		}

		// Check if next edge field must be updated.
		if (next != NO_UPDATE)
		{
			this->edges[index].setNext(next);
		}

		// Check if face field must be updated.
		if (face != NO_UPDATE)
		{
			this->edges[index].setFace(face);
		}
	}
	else
	{
		// PENDING: EXCEPTION?
		Logging::buildText(__FUNCTION__, __FILE__, "Updating edge with index ");
		Logging::buildText(__FUNCTION__, __FILE__, index);
		Logging::buildText(__FUNCTION__, __FILE__, " out of bounds.");
		Logging::buildRange(__FUNCTION__, __FILE__, 0, this->nEdges);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	addFace
* IN:		face		face to add
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new face to the faces array.
***************************************************************************/
void Dcel::addFace(const Face *face)
{
	try
	{
#ifdef DEBUG_ADD_FACE
		Logging::buildText(__FUNCTION__, __FILE__, "Creating face ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nFaces);
		Logging::write(true, Info);
#endif
		// Add face and increase # faces.
		this->faces[this->nFaces] = (*face);
		this->nFaces++;
	}
	catch (exception &ex)
	{
		ex.what();
	}
}

/***************************************************************************
* Name: 	addFace
* IN:		face		edge in face
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new face to the faces array.
***************************************************************************/
void Dcel::addFace(const int edge)
{
	// Check if faces vector is full.
	if (this->nFaces < this->sizeFaces)
	{
#ifdef DEBUG_ADD_FACE
			Logging::buildText(__FUNCTION__, __FILE__, "Creating face ");
			Logging::buildText(__FUNCTION__, __FILE__, this->nFaces);
			Logging::buildText(__FUNCTION__, __FILE__, " with edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edge);
			Logging::write(true, Info);
#endif
			// Add face and increase # faces.
			this->faces[this->nFaces] = Face(edge);
			this->nFaces++;
	}
	else
	{
		// PENDING: RESIZE? EXCEPTION?
		Logging::buildText(__FUNCTION__, __FILE__, "Faces vector is full.");
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	updateFace
* IN:		face		edge in face
*			imaginary	imaginary face identifier
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	update face at position "index" in the face array.
***************************************************************************/
void Dcel::updateFace(int edge_ID, int index)
{
	// JNC PENDING ALL INDEX ACCESSES TO THROW EXCEPTION.
	try
	{
		// Update edge id of face at index-th position.
		this->faces[index].setEdge(edge_ID);
	}
	catch (exception &ex)
	{
		ex.what();
	}
}

/***************************************************************************
* Name: 		isExternalEdge
* IN:			edgeIndex		index of the edge to check.
* OUT:			N/A
* IN/OUT:		N/A
* RETURN:		TRUE if edge at "index" position is in external face.
* 				FALSE i.o.c.
* Description: 	Checks if edge at "index" position is in external face.
***************************************************************************/
bool Dcel::isExternalEdge(int edgeIndex) const
{
	bool isExternal=false;			// Return value.

	// Check index is not out of bounds.
	if (edgeIndex < this->nEdges)
	{
		// Check if this edge or its twin belong face #0.
		if ((this->edges[edgeIndex].getFace() == EXTERNAL_FACE) ||
			(this->edges[this->edges[edgeIndex].getTwin()-1].getFace() == EXTERNAL_FACE))
		{
			// Edge is external.
			isExternal = true;
#ifdef DEBUG_IS_EXTERNAL_EDGE
			Logging::buildText(__FUNCTION__, __FILE__, "One of the edges is in external face:");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, ",");
			Logging::buildText(__FUNCTION__, __FILE__, this->edges[edgeIndex].getTwin());
			Logging::buildText(__FUNCTION__, __FILE__, this->edges[edgeIndex].toStr());
			Logging::buildText(__FUNCTION__, __FILE__, this->edges[this->edges[edgeIndex].getTwin()-1].toStr());
			Logging::write(true, Info);
#endif
		}
#ifdef DEBUG_IS_EXTERNAL_EDGE
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Edges are not in external face:");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, ",");
			Logging::buildText(__FUNCTION__, __FILE__, this->edges[edgeIndex].getTwin());
			Logging::buildText(__FUNCTION__, __FILE__, this->edges[edgeIndex].toStr());
			Logging::buildText(__FUNCTION__, __FILE__, this->edges[this->edges[edgeIndex].getTwin()-1].toStr());
			Logging::write(true, Info);
			isExternal = false;
		}
#endif
	}
	else
	{
    	Logging::buildText(__FUNCTION__, __FILE__,
    					"Trying to check an edge out of bounds. Edge index is ");
    	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex);
    	Logging::buildRange(__FUNCTION__, __FILE__, 0, this->nEdges);
    	Logging::write(true, Error);
		isExternal = false;
	}

	return(isExternal);
}

/***************************************************************************
* Name: 		hasNegativeVertex
* IN:			edgeID		edge identifier to check
* OUT:			N/A
* IN/OUT:		N/A
* RETURN:		TRUE if any of the vertex of the edge is negative
* 				FALSE i.o.c.
* Description: 	Checks if "edgeID" edge has an imaginary point as one of
* 				its extreme vertex.
***************************************************************************/
bool Dcel::hasNegativeVertex(int edgeID) const
{
    bool hasNegative=false;      // Return value.

	// Check face is not out of bounds.
	if (edgeID <= this->nEdges)
	{
		// Check if any of the vertex of the triangle is negative.
		if ((this->edges[edgeID-1].getOrigin() < 0) ||
			(this->edges[this->edges[edgeID-1].getNext()-1].getOrigin() < 0))
		{
#ifdef DEBUG_HAS_NEGATIVE_VERTEX
	    	Logging::buildText(__FUNCTION__, __FILE__, "Edge vertex are \n");
	    	Logging::buildText(__FUNCTION__, __FILE__, this->edges[edgeID-1].getOrigin());
	    	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	    	Logging::buildText(__FUNCTION__, __FILE__, this->edges[this->edges[edgeID-1].getNext()-1].getOrigin());
	    	Logging::write(true, Info);
#endif
			hasNegative = true;
		}
	}
	else
	{
#ifdef LOGGING
    	Logging::buildText(__FUNCTION__, __FILE__,
    					"Trying to check an edge out of bounds. Edge ID is ");
    	Logging::buildText(__FUNCTION__, __FILE__, edgeID);
    	Logging::buildRange(__FUNCTION__, __FILE__, 0, this->nEdges);
    	Logging::write(true, Error);
#endif
    	hasNegative = false;
	}

    return(hasNegative);
}


/***************************************************************************
* Name: 	getEdgeVertices
* IN:		edgeID		edge to return vertex.
* OUT:		index1		first edge vertex
* 			index2		seconds edge vertex
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	returns the edge vertex
**************************************************************************/
void Dcel::getEdgeVertices(int edgeID, int &index1, int &index2) const
{
	// Get edge extreme points.
	index1 = this->edges[edgeID-1].getOrigin();
	index2 = this->edges[this->edges[edgeID-1].getNext()-1].getOrigin();
}

/***************************************************************************
* Name: 	getCollinear
* IN:		pointIndex	collinear point index.
* 			edgeID		first edge of the triangle where point is collinear
* OUT:		NONE
* RETURN:	collinear edge identifier.
* GLOBAL:	NONE
* Description: 	returns the edge where "pointIndex" point is collinear.
***************************************************************************/
int	Dcel::getCollinear(int pointIndex, int edgeID)
{
	int 	collinearID;			// Return value.
	int		edgeIndex=0;			// Index of edge of current face.
	int		id1=0, id2=0, id3=0;	// IDs of vertex points.
    Point<TYPE> *p;					// Reference to new point.

	// Set edge index.
    edgeIndex = edgeID - 1;

	// Get index of the vertex of current face.
    id1 = this->getOrigin(this->getPrevious(edgeIndex) - 1);
    id2 = this->getOrigin(edgeIndex);
    id3 = this->getOrigin(this->getNext(edgeIndex) - 1);

#ifdef DEBUG_SELECT_COLLINEAR_EDGE
	Logging::buildText(__FUNCTION__, __FILE__, "Triangles points are ");
	Logging::buildText(__FUNCTION__, __FILE__, id1);
	Logging::buildText(__FUNCTION__, __FILE__, ",");
	Logging::buildText(__FUNCTION__, __FILE__, id2);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, id3);
	Logging::write(true, Info);
#endif

    // Get point coordinates.
	p = this->getRefPoint(pointIndex);

	// Check if point is collinear to previous edge.
	if (returnTurn(p, id1, id2) == COLLINEAR)
	{
#ifdef DEBUG_SELECT_COLLINEAR_EDGE
    	Logging::buildText(__FUNCTION__, __FILE__, "Collinear to points");
    	Logging::buildText(__FUNCTION__, __FILE__, id1);
    	Logging::buildText(__FUNCTION__, __FILE__, " and ");
    	Logging::buildText(__FUNCTION__, __FILE__, id2);
    	Logging::write(true, Info);
#endif
		collinearID = this->getPrevious(edgeIndex);
	}
	// Check if point is collinear to input edge.
	else if (returnTurn(p, id2, id3) == COLLINEAR)
	{
#ifdef DEBUG_SELECT_COLLINEAR_EDGE
    	Logging::buildText(__FUNCTION__, __FILE__, "Collinear to points");
    	Logging::buildText(__FUNCTION__, __FILE__, id2);
    	Logging::buildText(__FUNCTION__, __FILE__, " and ");
    	Logging::buildText(__FUNCTION__, __FILE__, id3);
    	Logging::write(true, Info);
#endif
		collinearID = edgeID;
	}
	// Check if point is collinear to next edge.
	else if (returnTurn(p, id3, id1) == COLLINEAR)
	{
#ifdef DEBUG_SELECT_COLLINEAR_EDGE
    	Logging::buildText(__FUNCTION__, __FILE__, "Collinear to points");
    	Logging::buildText(__FUNCTION__, __FILE__, id3);
    	Logging::buildText(__FUNCTION__, __FILE__, " and ");
    	Logging::buildText(__FUNCTION__, __FILE__, id1);
    	Logging::write(true, Info);
#endif
		collinearID = this->getNext(edgeIndex);
	}
	else
	{
    	Logging::buildText(__FUNCTION__, __FILE__, "Checking when point is collinear but it is not");
    	Logging::buildText(__FUNCTION__, __FILE__, "Triangles points are ");
    	Logging::buildText(__FUNCTION__, __FILE__, id1);
    	Logging::buildText(__FUNCTION__, __FILE__, ",");
    	Logging::buildText(__FUNCTION__, __FILE__, id2);
    	Logging::buildText(__FUNCTION__, __FILE__, " and ");
    	Logging::buildText(__FUNCTION__, __FILE__, id3);
    	Logging::write(true, Error);

		collinearID = -1;
	}

#ifdef DEBUG_SELECT_COLLINEAR_EDGE
   	Logging::buildText(__FUNCTION__, __FILE__, "Collinear edge is ");
   	Logging::buildText(__FUNCTION__, __FILE__, collinearID);
   	Logging::write(true, Info);
#endif

	return(collinearID);
}

/***************************************************************************
* Name: 	isValid
* IN:		NONE
* OUT:		NONE
* RETURN:	true		if the DCEL is valid
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	Checks all the data in the dcel. This functions checks:
* 				1.-
* 				2.-
* 				3.-
* https://github.com/juannavascalvente/Delaunay/issues/3
***************************************************************************/
bool Dcel::isValid()
{
	return(true);
}

/***************************************************************************
* Name: 	shake
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	shake all points in DCEL.
***************************************************************************/
void Dcel::shake()
{
	int	i=0;			// Loop counter.

	// Points loop.
	for (i=0;i<this->nVertex ;i++)
	{
		this->vertex[i].shake();
	}
}


/***************************************************************************
* Name: 	sort
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	sorts the DCEL set of points placing the lowest Y coordinate
* 				point at first position.
***************************************************************************/
void Dcel::sort()
{
	// Call quicksort with DCEL set of points.
	quicksort(&this->vertex[0], &this->vertex[1], 0, this->getNVertex()-2);
}


// PENDING SPEC.
int Dcel::getIndexLowest(bool (*f)(Point<TYPE> *, Point<TYPE> *))
{
    int     i=0;            // Loop counter.
    int     index=0;	// Index of highest point.

    // Get number of vertexes.
    index = 0;

    // Find index of highest point.
    for (i=1; i<this->nVertex ;i++)
    {
    	if ((*f) (this->vertex[i].getRefPoint(), this->vertex[index].getRefPoint()))
        {
    		index = i;
        }
    }
#ifdef DEBUG_SETLOWESTS_FIRST
	Logging::buildText(__FUNCTION__, __FILE__, "Lowest point at position ");
	Logging::buildText(__FUNCTION__, __FILE__, highest_Index);
	Logging::buildText(__FUNCTION__, __FILE__, " coordinates ");
	Logging::buildText(__FUNCTION__, __FILE__, this->vertex[0].getPoint().toStr());
	Logging::write(true, Info);
#endif
	return(index);
}


/***************************************************************************
* Name: 	getIndexHighest
* IN:		f			function to select point
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	searches highest point in the DCEL vertex set and places
* 				it in the first position of the array.
***************************************************************************/
int Dcel::getIndexHighest(bool (*f)(const Point<TYPE> *, const Point<TYPE> *))
{
    int     i=0;                // Loop counter.
    int     index=0;    // Index of highest point.

    // Get number of vertexes.
    index=0;

    // Find index of highest point.
    for (i=1; i<this->nVertex ;i++)
    {
    	if ((*f) (this->vertex[i].getRefPoint(), this->vertex[index].getRefPoint()))
        {
            index = i;
        }
    }
#ifdef DEBUG_SETHIGHEST_FIRST
	Logging::buildText(__FUNCTION__, __FILE__, "Highest point at position ");
	Logging::buildText(__FUNCTION__, __FILE__, index);
	Logging::buildText(__FUNCTION__, __FILE__, " coordinates ");
	Logging::buildText(__FUNCTION__, __FILE__, this->vertex[0].getPoint().toStr());
	Logging::write(true, Info);
#endif

    return(index);
}


/***************************************************************************
* Name: 	return_Turn
* IN:		p					reference to first point
* 			sourcePoint			second point index in dcel points array
* 			destPoint			third point index in dcel points array
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	searches highest point in the DCEL vertex set and places
* 				it in the first position of the array.
***************************************************************************/
enum Turn_T Dcel::returnTurn(const Point<TYPE> *p, int sourcePoint, int destPoint)
{
    enum Turn_T turn;         // Return value.

    // Normal source point.
    if (sourcePoint > 0)
    {
        // Normal destination point.
        if (destPoint > 0)
        {
            // If turn right then point is not in triangle.
			//turn = check_Turn(&this->vertex[sourcePoint-1].point, &this->vertex[destPoint-1].point, p);
			turn = this->vertex[sourcePoint-1].getRefPoint()->check_Turn(*this->vertex[destPoint-1].getRefPoint(), *p);
        }
        // Destination point is P-2.
        else if (destPoint == P_MINUS_2)
        {
            if (sourcePoint == 1)
        	{
        		turn = LEFT_TURN;
        	}
            // Check if point is over line from source_Index point to P-2.
            else if (Point<TYPE>::higher_Point(p, this->vertex[sourcePoint-1].getRefPoint(), &Point<TYPE>::lexicographicHigher))
            {
                turn = RIGHT_TURN;
            }
            else
			{
				turn = LEFT_TURN;
			}
        }
        // Destination point is P-1.
        else
        {
            // Check if point is over line from source_Index point to P-1.
            if (Point<TYPE>::higher_Point(p, this->vertex[sourcePoint-1].getRefPoint(), &Point<TYPE>::lexicographicHigher))
            {
				turn = LEFT_TURN;
            }
            else
			{
				turn = RIGHT_TURN;
			}
        }
    }
    else
    {
        // Source point is P-1 and destination cannot be p-2.
        if (sourcePoint == P_MINUS_1)
        {
            if (destPoint == 1)
        	{
        		turn = LEFT_TURN;
        	}
            // Check if point is over line from P-1 point to dest_Index point.
            else if (Point<TYPE>::higher_Point(p, this->vertex[destPoint-1].getRefPoint(), &Point<TYPE>::lexicographicHigher))
            {
				turn = RIGHT_TURN;
            }
            else
			{
				turn = LEFT_TURN;
			}
        }
        // Source point is P-2.
        else
        {
            // Check destination point.
            if (destPoint != P_MINUS_1)
            {
				if (Point<TYPE>::higher_Point(p, this->vertex[destPoint-1].getRefPoint(), &Point<TYPE>::lexicographicHigher))
				{
					turn = LEFT_TURN;
				}
				else
				{
					turn = RIGHT_TURN;
				}
            }
            else
			{
				// Points can only do a left turn.
				turn = LEFT_TURN;
			}
        }
    }

#ifdef DEBUG_RETURN_TURN
	Logging::buildText(__FUNCTION__, __FILE__, "Turn between segment points ");
	Logging::buildText(__FUNCTION__, __FILE__, sourcePoint);
	Logging::buildText(__FUNCTION__, __FILE__, " with coordinates ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getRefPoint(sourcePoint-1)->toStr());
	Logging::buildText(__FUNCTION__, __FILE__, " and point ");
	Logging::buildText(__FUNCTION__, __FILE__, destPoint);
	Logging::buildText(__FUNCTION__, __FILE__, " with coordinates ");
	Logging::buildText(__FUNCTION__, __FILE__, this->getRefPoint(destPoint-1)->toStr());
	Logging::buildText(__FUNCTION__, __FILE__, " and point ");
	Logging::buildText(__FUNCTION__, __FILE__, p->toStr());

    if (turn == LEFT_TURN)
    {
    	Logging::buildText(__FUNCTION__, __FILE__, " is LEFT");
    }
    else if (turn == RIGHT_TURN)
    {
    	Logging::buildText(__FUNCTION__, __FILE__, " is RIGHT");
    }
    else
    {
    	Logging::buildText(__FUNCTION__, __FILE__, " is COLLINEAR");
    }
	Logging::write(true, Info);
#endif

    return(turn);
}


/***************************************************************************
* Name: 	signedArea
* IN:		id1			1st point index
* 			id2			2nd point index
* 			id3			3rd point index
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description:	computes the signed area formed by points located at indexes
* 				id1, id2 and id3 of the dcel vertex array.
***************************************************************************/
double Dcel::signedArea(int id1, int id2, int id3)
{
	double area;					// Return value.
	Point<TYPE> *p1, *p2, *p3;		// Temporary points

	// Get references to points.
	if ((id1 < 0) || (id2 < 0) || (id3 < 0))
	{
        area = 0.0;
	}
	else
	{
        p1 = this->getRefPoint(id1);
        p2 = this->getRefPoint(id1);
        p3 = this->getRefPoint(id1);
        area = p1->signedArea(*p2, *p3);
    }

	return(area);
}

/***************************************************************************
* Name: 	getEdgeInserection
* IN:		line			line to check intersection.
* 			face			face to check all its edges.
* IN/OUT:	edgeId			edge that intersects with line.
* RETURN:	true			if any face edge intersects "line".
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	checks if any edge of the input "face" intersects input
* 				"line". If so then returns true and edgeId has the id of the
* 				intersected edge.
***************************************************************************/
bool Dcel::getEdgeInserection(Line &line, int face, int &edgeId)
{
	bool intersect=false;		// Return value.
	int  edgeIndex=0;			// Edge index.
	Line edgeLine;				// Current edge line.
	Point<TYPE> origin, dest;	// Edge extreme points.

	// Get first edge in face.
	if (edgeId == INVALID)
	{
		edgeIndex = this->getFaceEdge(face)-1;
	}
	else
	{
		edgeIndex = edgeId-1;
	}
#ifdef DEBUG_DCEL_EDGE_INTERSECTION
	Logging::buildText(__FUNCTION__, __FILE__, "Starting in edge ");
	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
	Logging::buildText(__FUNCTION__, __FILE__, " from face ");
	Logging::buildText(__FUNCTION__, __FILE__, face);
	Logging::write(true, Info);
#endif

	// Loop until intersection found.
	do
	{
		// Check if current edge is real.
		if (!this->hasNegativeVertex(edgeIndex+1))
		{
#ifdef DEBUG_DCEL_EDGE_INTERSECTION
			int	 originId=0;
			int	 destId=0;
			originId = this->getOrigin(edgeIndex);
			destId = this->getOrigin(this->getTwin(edgeIndex)-1);
			Logging::buildText(__FUNCTION__, __FILE__, "Current edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, " and its twin ");
			Logging::buildText(__FUNCTION__, __FILE__, this->getTwin(edgeIndex));
			Logging::buildText(__FUNCTION__, __FILE__, ". Origin point ");
			Logging::buildText(__FUNCTION__, __FILE__, originId);
			Logging::buildText(__FUNCTION__, __FILE__, " and destination point ");
			Logging::buildText(__FUNCTION__, __FILE__, destId);
			Logging::write(true, Info);
#endif
			// Get current edge extreme points.
			this->getEdgePoints(edgeIndex, origin, dest);
			edgeLine = Line(origin, dest);

			// Check intersection.
			if (line.intersect(edgeLine))
			{
				// Update return value.
				edgeId = edgeIndex+1;
				intersect = true;
#ifdef DEBUG_DCEL_EDGE_INTERSECTION
				Logging::buildText(__FUNCTION__, __FILE__, "Intersection found in edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeId);
				Logging::write(true, Info);
#endif
			}
			else
			{
#ifdef DEBUG_DCEL_EDGE_INTERSECTION
				Logging::buildText(__FUNCTION__, __FILE__, "Do not intersect. Next edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::write(true, Info);
#endif
			}
		}
#ifdef DEBUG_DCEL_EDGE_INTERSECTION
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Not real edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::write(true, Info);
		}
#endif
		// Next edge.
		edgeIndex = this->getNext(edgeIndex)-1;
	} while (!intersect);

	return(intersect);
}

/***************************************************************************
* Name: 	findPath
* IN:		extremeFaces		set storing first and last faces.
* 			line				line between both points
* OUT:		face				list of faces between both points.
* RETURN:	true				if path found.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	finds the list of faces where input line lays.
***************************************************************************/
bool Dcel::findPath(Set<int> &extremeFaces, Line &line, Set<int> &faces)
{
	bool found=true;		// Return value.
	int	edgeId=INVALID;		// Edge id.
	int firstFace=0;		// First face in path.
	int lastFace=0;			// First face in path.

	// Get origin and destination faces of the path.
	firstFace = *extremeFaces.at(0);
	lastFace = *extremeFaces.at(1);
#ifdef DEBUG_DELAUNAY_FIND_PATH
	Logging::buildText(__FUNCTION__, __FILE__, "Searching path between faces ");
	Logging::buildText(__FUNCTION__, __FILE__, firstFace);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, lastFace);
	Logging::write(true, Info);
#endif
	// Check both points are not in the same face.
	if (firstFace != lastFace)
	{
		do
		{
			// Get edge intersected by p1-p2 line.
			if (this->getEdgeInserection(line, firstFace, edgeId))
			{
				// Insert current face.
				faces.add(firstFace);
#ifdef DEBUG_DELAUNAY_FIND_PATH
				Logging::buildText(__FUNCTION__, __FILE__, "Intersected edge is ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeId);
				Logging::buildText(__FUNCTION__, __FILE__, ".Inserted face ");
				Logging::buildText(__FUNCTION__, __FILE__, firstFace);
				Logging::write(true, Info);
#endif
				// Get next face.
				edgeId = this->getTwin(edgeId-1);
#ifdef DEBUG_DELAUNAY_FIND_PATH
				Logging::buildText(__FUNCTION__, __FILE__, "Twin edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeId);
#endif
				edgeId = this->getNext(edgeId-1);
				firstFace = this->getFace(edgeId-1);
#ifdef DEBUG_DELAUNAY_FIND_PATH
				Logging::buildText(__FUNCTION__, __FILE__, ".New edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeId);
				Logging::buildText(__FUNCTION__, __FILE__, " in face ");
				Logging::buildText(__FUNCTION__, __FILE__, firstFace);
				Logging::write(true, Info);
#endif
			}
			else
			{
				found = false;
				Logging::buildText(__FUNCTION__, __FILE__, "Current face ");
				Logging::buildText(__FUNCTION__, __FILE__, firstFace);
				Logging::buildText(__FUNCTION__, __FILE__, " does not intersect line.");
				Logging::write(true, Info);
			}
		} while ((firstFace != lastFace) && found);
	}
#ifdef DEBUG_DELAUNAY_FIND_PATH
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Both points in the same face.");
		Logging::write(true, Info);
	}
#endif
#ifdef DEBUG_DELAUNAY_FIND_PATH
	Logging::buildText(__FUNCTION__, __FILE__, "Added last face ");
	Logging::buildText(__FUNCTION__, __FILE__, firstFace);
	Logging::write(true, Info);
#endif
	// Insert last face.
	faces.add(firstFace);

	return(found);
}


/***************************************************************************
* Name: 	readPoints
* IN:		fileName	file name to write data into.
* OUT:		NONE
* RETURN:	true 		if file read.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	writes the set of points in DCEL separated by spaces to
* 				"fileName"
***************************************************************************/
bool Dcel::operator==(const Dcel& other) const
{
	int i=0;		// Loop counter
	bool equal;	    // Return value.

	// Compare # vertex, edges and faces.
	equal = (this->nVertex == other.nVertex) &&
			(this->nEdges == other.nEdges) &&
			(this->nFaces == other.nFaces);
	if (equal)
	{
		// Vertex loop.
		i=0;
		while (equal && (i<this->nVertex))
		{
			// Compare vertex.
			equal = this->vertex[i].getOrigin() == other.vertex[i].getOrigin();
#ifdef DEBUG_DCEL_EQUAL
			if (!equal)
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Different origin vertx edge at index ");
				Logging::buildText(__FUNCTION__, __FILE__, i);
				Logging::write(true, Info);
			}
#endif
			i++;
		}

		// Edges loop.
		i=0;
		while (equal && i< this->nEdges)
		{
			// Compare edges.
			equal = (this->edges[i] == other.edges[i]);
#ifdef DEBUG_DCEL_EQUAL
			if (!equal)
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Different edges at index ");
				Logging::buildText(__FUNCTION__, __FILE__, i);
				Logging::write(true, Info);
			}
#endif
			i++;
		}

		// Faces loop.
		i=0;
		while (equal && i< this->nFaces)
		{
			// Compare faces.
			equal = (this->faces[i] == other.faces[i]);
#ifdef DEBUG_DCEL_EQUAL
			if (!equal)
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Different faces at index ");
				Logging::buildText(__FUNCTION__, __FILE__, i);
				Logging::write(true, Info);
			}
#endif
			i++;
		}
	}
#ifdef DEBUG_DCEL_EQUAL
	else
	{
		if (this->nVertex != other.nVertex)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "# vertex is different. This # vertex is");
			Logging::buildText(__FUNCTION__, __FILE__, this->nVertex);
			Logging::buildText(__FUNCTION__, __FILE__, " and input dcel #vertex is ");
			Logging::buildText(__FUNCTION__, __FILE__, other.nVertex);
			Logging::write(true, Info);
		}
		if (this->nEdges != other.nEdges)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "# edges is different. This # edges is");
			Logging::buildText(__FUNCTION__, __FILE__, this->nEdges);
			Logging::buildText(__FUNCTION__, __FILE__, " and input dcel #edges is ");
			Logging::buildText(__FUNCTION__, __FILE__, other.nEdges);
			Logging::write(true, Info);
		}
		if (this->nFaces != other.nFaces)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "# faces is different. This # faces is");
			Logging::buildText(__FUNCTION__, __FILE__, this->nFaces);
			Logging::buildText(__FUNCTION__, __FILE__, " and input dcel #faces is ");
			Logging::buildText(__FUNCTION__, __FILE__, other.nFaces);
			Logging::write(true, Info);
		}
	}
#endif

	return(equal);
}


/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	quicksort
* IN:		N/A
* OUT:		N/A
* IN/OUT:	N/A
* RETURN:	N/A
* Description: clutters the set of point of the DCEL.
***************************************************************************/
void Dcel::quicksort(Vertex *origin, Vertex *list, int first, int last)
{
	int pivotIndex=0;			// Index of the element used as pivot.

	// Check if list is one element long.
    if(first < last)
	{
		// Move first element to its position.
    	pivotIndex = movePivot(origin, list, first, last);

		// Order from initial element to previous element to pivot.
		quicksort(origin, list, first, pivotIndex-1);

		// Order from next element to pivot until end of set.
		quicksort(origin, list, pivotIndex+1, last);
	}
}


/***************************************************************************
* Name: 	movePivot
* IN:		N/A
* OUT:		N/A
* IN/OUT:	N/A
* RETURN:	N/A
* Description: clutters the set of point of the DCEL.
***************************************************************************/
int	 Dcel::movePivot(Vertex *origin, Vertex *list, int first, int last)
{
    int pivotIndex=0;				// Return value.
	int i=0, firstIndex=0;			// Loop variables.
	Vertex pivotPoint;				// Pivot point.
	Vertex temp;					// Register to exchange elements in list.

	// Get position and value of pivot point.
    pivotIndex = first;
    pivotPoint = list[pivotIndex];

	// Set first index.
	firstIndex = first + 1;

	// Loop.
    for (i=firstIndex; i<=last; i++)
	{
		// If turn is right then angle is lower in list[i] than in pivot element.
		if ((origin->getRefPoint()->check_Turn(*pivotPoint.getRefPoint(), *list[i].getRefPoint()) == RIGHT_TURN) ||
		   ((origin->getRefPoint()->check_Turn(*pivotPoint.getRefPoint(), *list[i].getRefPoint()) == COLLINEAR) &&
			(pivotPoint.getPoint().getX() > list[i].getPoint().getX())))
		{
			// Increase pivot index.
			pivotIndex++;

			// Move current element to lower position in set.
            temp = list[i];
            list[i] = list[pivotIndex];
            list[pivotIndex] = temp;
		}
	}

	// Move pivot point.
    temp = list[first];
    list[first] = list[pivotIndex];
    list[pivotIndex] = temp;

	return(pivotIndex);
}

/***************************************************************************
* Name: 	swapVertex
* IN:		N/A
* OUT:		N/A
* IN/OUT:	N/A
* RETURN:	N/A
* Description: swaps vertex positions in array.
***************************************************************************/
void Dcel::swapVertex(int index1, int index2)
{
	Vertex vertex;	// Temporary variable.

	if ((index1 < this->getNVertex()) || (index2 < this->getNVertex()))
	{
		// Swap vertices.
		vertex = this->vertex[index1];
		this->vertex[index1] = this->vertex[index2];
		this->vertex[index2] = vertex;
	}
	else
	{
    	Logging::buildText(__FUNCTION__, __FILE__, "Error swapping vertex between positions ");
    	Logging::buildText(__FUNCTION__, __FILE__, index1);
    	Logging::buildText(__FUNCTION__, __FILE__, " and ");
    	Logging::buildText(__FUNCTION__, __FILE__, index2);
    	Logging::write(true, Error);
	}
}
