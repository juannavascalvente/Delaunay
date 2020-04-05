/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Logging.h"
#include "Circle.h"
#include "Voronoi.h"
#include "DcelReader.h"
#include "DcelWriter.h"


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_VORONOI_INIT
//#define DEBUG_VORONOI_EDGEEXISTS
//#define DEBUG_VORONOI_BUILD
//#define DEBUG_VORONOI_BUILDAREA
//#define DEBUG_VORONOI_GETEXTREMEVORONOI
//#define DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
//#define DEBUG_ISBOTTOMOSTFACE
//#define DEBUG_VORONOI_INNERTOAREA
//#define DEBUG_VORONOI_EDGE_INTERSECTION
//#define DEBUG_VORONOI_CORRECTBORDER
//#define DEBUG_VORONOI_FIND_PATH
#endif


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Voronoi::Voronoi(Dcel &t)
{
	// Initialize attributes.
	this->triangulation = t;
}


void Voronoi::reset()
{
    this->dcel.reset();
}


/***************************************************************************
* Name: 	build
* IN:		isIncremental	is incremental Delaunay flag.
* OUT:		NONE
* RETURN:	true 			if Voronoi diagram built.
* 			false			otherwise.
* GLOBAL:	this->voronoi	update with Voronoi edges and faces.
* Description: 	computes the Voronoi diagram associated to the current
* 				triangulation stored in the "triangulation" attribute.
***************************************************************************/
bool Voronoi::build(bool isIncremental)
{
	bool built=true;		// Return value.
	int	 pointIndex=0;		// Loop counter.

    this->dcel.addFace(INVALID);

#ifdef DEBUG_VORONOI_BUILD
    Logging::buildText(__FUNCTION__, __FILE__, "Start computing Voronoi diagram");
		Logging::write(true, Info);
#endif
    // Compute Voronoi circumcentres for every non-imaginary face.
    this->computeCircumcentres(isIncremental);

    // Compute Voronoi area for every point in Delaunay triangulation.
    for (pointIndex=0; pointIndex<this->triangulation.getNVertex() ;pointIndex++)
    {
        this->buildArea(pointIndex);
    }

#ifdef DEBUG_VORONOI_BUILD
    Logging::buildText(__FUNCTION__, __FILE__, "Voronoi diagram computed");
		Logging::write(true, Info);
#endif

	return built;
}


bool Voronoi::isInnerToArea(const Point<TYPE> &p, int areaId)
{
	bool	inner=true;				// Return value.
	int		firstEdgeIndex=0;		// First edge index.
	int		currentEdgeIndex=0;		// Current edge index.
	Point<TYPE> origin;				// Origin point.
	Point<TYPE> dest;				// Destination point.

	// Get edge in current face.
	firstEdgeIndex = this->dcel.getFaceEdge(areaId) - 1;
	currentEdgeIndex = firstEdgeIndex;
#ifdef DEBUG_VORONOI_INNERTOAREA
	Logging::buildText(__FUNCTION__, __FILE__, "Starting in edge ");
	Logging::buildText(__FUNCTION__, __FILE__, firstEdgeIndex+1);
	Logging::buildText(__FUNCTION__, __FILE__, " from face ");
	Logging::buildText(__FUNCTION__, __FILE__, areaId);
	Logging::write(true, Info);
#endif
	do
	{
		// Get origin and destination points.
		origin = *this->dcel.getRefPoint(this->dcel.getOrigin(currentEdgeIndex) - 1);
		dest   = *this->dcel.getRefPoint(this->dcel.getOrigin(this->dcel.getTwin(currentEdgeIndex) - 1) - 1);
#ifdef DEBUG_VORONOI_INNERTOAREA
		Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
		Logging::buildText(__FUNCTION__, __FILE__, currentEdgeIndex+1);
		Logging::buildText(__FUNCTION__, __FILE__, " and twin edge ");
		Logging::buildText(__FUNCTION__, __FILE__, this->voronoi.getTwin(currentEdgeIndex));
		Logging::buildText(__FUNCTION__, __FILE__, " whose extremes points are ");
		Logging::buildText(__FUNCTION__, __FILE__, this->voronoi.getOrigin(currentEdgeIndex));
		Logging::buildText(__FUNCTION__, __FILE__, " and ");
		Logging::buildText(__FUNCTION__, __FILE__, this->voronoi.getOrigin(this->voronoi.getTwin(currentEdgeIndex)-1));
		Logging::write(true, Info);
		Logging::buildText(__FUNCTION__, __FILE__, "Origin point ");
		Logging::buildText(__FUNCTION__, __FILE__, &origin);
		Logging::buildText(__FUNCTION__, __FILE__, " and destination point is ");
		Logging::buildText(__FUNCTION__, __FILE__, &dest);
		Logging::buildText(__FUNCTION__, __FILE__, ". Checking point is ");
		Logging::buildText(__FUNCTION__, __FILE__, &p);
		Logging::write(true, Info);
#endif
		// If right turn then it is not inner.
		if (origin.check_Turn(dest, p) == RIGHT_TURN)
		{
			inner = false;
#ifdef DEBUG_VORONOI_INNERTOAREA
			Logging::buildText(__FUNCTION__, __FILE__, "Right turn -> is not interior.");
			Logging::write(true, Info);
#endif
		}

		// Get nexte edge.
		currentEdgeIndex = this->dcel.getNext(currentEdgeIndex) - 1;
	} while ((currentEdgeIndex != firstEdgeIndex) && (inner));

	return(inner);
}


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	computeCircumcentres
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	this->voronoi		update the "voronoi" vertex array
* Description: 	comptues the voronoi centre of every face of the "dcel"
* 				triangulation and updates the "voronoi" vertex array.
***************************************************************************/
void Voronoi::computeCircumcentres(bool isIncremental)
{
	int			faceId=0;			// Current face id.
	int			adjacentFaceId=0;	// Adjacent face id.
	int 		nImaginary=0;		// # imaginary faces.
	int			lastIamginaryFace=0;// Last imaginary face id.
	int			edgeIndex=0;		// Edge index.
	int			twinEdgeIndex=0;	// Twin edge index.
	bool		realFaceFound;		// Adjacent face is real.
    Point<TYPE>	p, q, r;			// Vertices points.
	Point<TYPE>	*centre;			// Vertices points.
	Point<TYPE>	externalCentre;		// Circuemcentre in external face.
	Point<TYPE>	invalidPoint;		// Point to use in imaginary faces.

#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
	int nReal = 0;
	Logging::buildText(__FUNCTION__, __FILE__, "# circumcentres to compute: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->triangulation.getNFaces()-1);
	Logging::write(true, Info);
#endif

#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
	Logging::buildText(__FUNCTION__, __FILE__, "Real circumcentres.");
	Logging::write(true, Info);
#endif

	// Loop all faces from 1 because zero face has no circumcentre.
	for (faceId=1; faceId<this->triangulation.getNFaces() ;faceId++)
	{
		// Skip imaginary faces.
		if (!this->triangulation.imaginaryFace(faceId))
		{
			this->triangulation.getFacePoints(faceId, p, q, r);

		    // Build circle.
            vector<Point<TYPE>> vPoints;
            vPoints.push_back(p);
            vPoints.push_back(q);
            vPoints.push_back(r);
            Circle circle = Circle(vPoints);

			// Add circumcentre.
			this->dcel.addVertex(circle.getRefCentre(), INVALID);
#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
			Logging::buildText(__FUNCTION__, __FILE__, "Real face ");
			Logging::buildText(__FUNCTION__, __FILE__, faceId);
			Logging::buildText(__FUNCTION__, __FILE__, " circumcentre ");
			Logging::buildText(__FUNCTION__, __FILE__, circle.getRefCentre());
			Logging::write(true, Info);
			nReal++;
#endif
        }
		// Add invalid point for imaginary face.
		else
        {
#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
        	Logging::buildText(__FUNCTION__, __FILE__, "Imaginary face: ");
        	Logging::buildText(__FUNCTION__, __FILE__, faceId);
        	Logging::write(true, Info);

#endif
        	lastIamginaryFace = faceId;
        	nImaginary++;
			this->dcel.addVertex(&invalidPoint, INVALID);
        }
	}

#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
	Logging::buildText(__FUNCTION__, __FILE__, "Imaginary circumcentres.");
	Logging::write(true, Info);
#endif

	if (isIncremental)
	{
		// Build imaginary faces circumcentres.
		for (faceId=1; faceId<=lastIamginaryFace ;faceId++)
		{
			// Skip face bottom most face because it has no adjacent real faces.
			if (!this->isBottomMostFace(faceId))
			{
				// Parse only imaginary Delaunay faces.
				if (this->triangulation.imaginaryFace(faceId))
				{
#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
					Logging::buildText(__FUNCTION__, __FILE__, "Face ");
					Logging::buildText(__FUNCTION__, __FILE__, faceId);
					Logging::buildText(__FUNCTION__, __FILE__, " is imaginary.");
					Logging::write(true, Info);
#endif
					// Get edge in current Delaunay face.
					edgeIndex = this->triangulation.getFaceEdge(faceId)-1;

					realFaceFound = false;
					while (!realFaceFound)
					{
						// Check if twin edge belongs to real Delaunay face.
						twinEdgeIndex = this->triangulation.getTwin(edgeIndex)-1;
						adjacentFaceId = this->triangulation.getFace(twinEdgeIndex);
						if (!this->triangulation.imaginaryFace(adjacentFaceId))
						{
							// Adjacent face found.
							realFaceFound = true;
						}
						else
						{
							// Check next edge in current Delaunay face.
							edgeIndex = this->triangulation.getNext(edgeIndex)-1;
						}
					}

					// Get centre of adjacent Voronoi face.
					centre = this->dcel.getRefPoint(adjacentFaceId - 1);

					// Compute centre of external Delaunay face.
					externalCentre = this->computeExtremeVoronoi(edgeIndex, *centre);

					// Update Voronoi point coordinates.
					this->dcel.updateVertex(&externalCentre, faceId - 1);
				}
#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
				else
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Face ");
					Logging::buildText(__FUNCTION__, __FILE__, faceId);
					Logging::buildText(__FUNCTION__, __FILE__, " is real and skipped.");
					Logging::write(true, Info);
				}
#endif
			}
#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Face ");
				Logging::buildText(__FUNCTION__, __FILE__, faceId);
				Logging::buildText(__FUNCTION__, __FILE__, " is bottom most face and skipped.");
				Logging::write(true, Info);
			}
#endif
		}
	}

#ifdef DEBUG_VORONOI_COMPUTE_CIRCUMCENTRES
	Logging::buildText(__FUNCTION__, __FILE__, "# imaginary faces ");
	Logging::buildText(__FUNCTION__, __FILE__, nImaginary);
	Logging::buildText(__FUNCTION__, __FILE__, ".\t# real faces ");
	Logging::buildText(__FUNCTION__, __FILE__, nReal);
	Logging::write(true, Info);
#endif
}

/***************************************************************************
* Name: 	build
* IN:		pointIndex		index of the point whose area must be built
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	this
* Description: 	computes the Voronoi area of the "pointIndex" point of the
* 				triangulation and stores it in the "voronoi" DCEL.
***************************************************************************/
void Voronoi::buildArea(int pointIndex)
{
	bool 			finished;			// Loop control flag.

	int voronoiOriginId=0, voronoiDestId=0;	// Edge origin and destination points id.

	int				firstEdge=0;	 	// First edge in loop.
	int				edgeIndex=0;	 	// Current edge index.
	int				currentEdge=0; 		// Current edge.
	int				previousEdge; 		// Previous edge.
	int				newEdgeId=0;		// New edge it to be inserted.
	int				existingEdge=0;		// Existing edge id.
	int				existingEdgeIndex=0;// Existing edge index.
	Edge			*edge;
	bool			previousExist=false;
	int				newVoronoiFaceId=0;	// Face id in Voronoi area.

	int		firstEdgeInFace=0;			// First inserted edge in current Voronoi face.
	bool 	firstInserted=false;		// Flag to set to true after first edge of the face..

#ifdef DEBUG_VORONOI_BUILDAREA
	Logging::buildText(__FUNCTION__, __FILE__, "Building Voronoi area for point: ");
	Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
	Logging::write(true, Info);
#endif

	// Initialize variables.
	previousEdge = INVALID;
	newEdgeId = this->dcel.getNEdges() + 1;

	// Get edge departing from point.
	currentEdge = this->triangulation.getPointEdge(pointIndex);
	edgeIndex = currentEdge - 1;
#ifdef DEBUG_VORONOI_BUILDAREA
	Logging::buildText(__FUNCTION__, __FILE__, "Edge departing from point is ");
	Logging::buildText(__FUNCTION__, __FILE__, currentEdge);
	Logging::write(true, Info);
#endif

	// Get face where current edge is inserted.
	voronoiOriginId = this->triangulation.getFace(edgeIndex);

	// Get first edge from a real face.
	finished=false;
	while (!finished)
	{
		// Get face
		if (this->triangulation.imaginaryFace(voronoiOriginId) ||
			this->triangulation.isExternalEdge(edgeIndex))
		{
#ifdef DEBUG_VORONOI_BUILDAREA
			if (this->triangulation.imaginaryFace(voronoiOriginId))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Imaginary face ");
				Logging::buildText(__FUNCTION__, __FILE__, voronoiOriginId);
			}
			else if (this->triangulation.isExternalEdge(edgeIndex))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "External edge (or its twin) ");
				Logging::buildText(__FUNCTION__, __FILE__, currentEdge);
			}
			Logging::buildText(__FUNCTION__, __FILE__, ". Get next face.");
			Logging::write(true, Info);
#endif
			// Update current edge and its index.
			currentEdge = this->triangulation.getTwin(this->triangulation.getPrevious(edgeIndex)-1);
			edgeIndex = currentEdge - 1;

			// Get face where current edge is inserted.
			voronoiOriginId = this->triangulation.getFace(edgeIndex);
		}
		else
		{
			finished = true;
			firstEdge = currentEdge;
		}
	}

#ifdef DEBUG_VORONOI_BUILDAREA
	Logging::buildText(__FUNCTION__, __FILE__, "Start building area using Delaunay edge ");
	Logging::buildText(__FUNCTION__, __FILE__, firstEdge);
	Logging::buildText(__FUNCTION__, __FILE__, " in face ");
	Logging::buildText(__FUNCTION__, __FILE__, voronoiOriginId);
	Logging::write(true, Info);
#endif

	// Get current face id.
	newVoronoiFaceId = this->dcel.getNFaces();

	// Loop until all faces with "pointIndex" as vertex have been checked.
	do
	{
#ifdef DEBUG_VORONOI_BUILDAREA
		Logging::buildText(__FUNCTION__, __FILE__, "Next Delaunay edge ");
		Logging::buildText(__FUNCTION__, __FILE__, currentEdge);
		Logging::buildText(__FUNCTION__, __FILE__, ". Data: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->triangulation.getRefEdge(edgeIndex)->toStr());
		Logging::write(true, Info);
#endif
		// Skip imaginary edges.
		if (!this->triangulation.hasNegativeVertex(currentEdge))
		{
			// Get origin and destination points of Voronoi edge.
			voronoiOriginId = this->triangulation.getFace(this->triangulation.getTwin(edgeIndex)-1);
			voronoiDestId   = this->triangulation.getFace(edgeIndex);
#ifdef DEBUG_VORONOI_BUILDAREA
			Logging::buildText(__FUNCTION__, __FILE__, "Voronoi origin ");
			Logging::buildText(__FUNCTION__, __FILE__, voronoiOriginId);
			Logging::buildText(__FUNCTION__, __FILE__, " and destination ");
			Logging::buildText(__FUNCTION__, __FILE__, voronoiDestId);
			Logging::write(true, Info);
#endif

			// Get current triangulation edge data.
			edge = this->triangulation.getRefEdge(edgeIndex);

			// Check if Voronoi edge already created.
			if (this->edgeExists(*edge))
			{
				// Get one of the edges of existing Voronoi face.
				existingEdge = this->dcel.getFaceEdge(this->triangulation.getOrigin(this->triangulation.getTwin(edgeIndex) - 1));
				existingEdgeIndex = existingEdge - 1;

#ifdef DEBUG_VORONOI_BUILDAREA
				Logging::buildText(__FUNCTION__, __FILE__, "Voronoi face initial edge ");
				Logging::buildText(__FUNCTION__, __FILE__, existingEdge);
				Logging::write(true, Info);
#endif
				// Loop until existing edge is found.
				while (voronoiDestId != this->dcel.getOrigin(existingEdgeIndex))
				{
					// Get next edge.
					existingEdge = this->dcel.getNext(existingEdgeIndex);
					existingEdgeIndex = existingEdge - 1;
#ifdef DEBUG_VORONOI_BUILDAREA
					Logging::buildText(__FUNCTION__, __FILE__, "Trying next Voronoi existing edge ");
					Logging::buildText(__FUNCTION__, __FILE__, existingEdge);
					Logging::buildText(__FUNCTION__, __FILE__, ". Expected origin ");
					Logging::buildText(__FUNCTION__, __FILE__, voronoiDestId);
					Logging::buildText(__FUNCTION__, __FILE__, " and is ");
					Logging::buildText(__FUNCTION__, __FILE__, this->voronoi.getOrigin(existingEdgeIndex));
					Logging::write(true, Info);
#endif
				}

				// Get existing edge data.
				edge = this->dcel.getRefEdge(existingEdgeIndex);
#ifdef DEBUG_VORONOI_BUILDAREA
				Logging::buildText(__FUNCTION__, __FILE__, "Found existing Voronoi edge ");
				Logging::buildText(__FUNCTION__, __FILE__, existingEdgeIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, " whose data is: ");
				Logging::buildText(__FUNCTION__, __FILE__, edge->toStr());
				Logging::write(true, Info);
#endif
				// Get twin of existing Voronoi edge (now existing edge is in current Voronoi face!!!!).
				existingEdge = edge->getTwin();
				existingEdgeIndex = existingEdge - 1;

				// Update existing edge.
				this->dcel.updateEdge(NO_UPDATE, NO_UPDATE, previousEdge, newEdgeId, newVoronoiFaceId, existingEdgeIndex);
				if (previousExist)
				{
					this->dcel.updateEdge(NO_UPDATE, NO_UPDATE, NO_UPDATE, existingEdge, NO_UPDATE, previousEdge - 1);
				}

				// Update previous edge.
				previousEdge = existingEdge;

				// Save edge if this is first edge in face.
				if (!firstInserted)
				{
					firstInserted = true;
					firstEdgeInFace = existingEdge;
				}
			}
			// Current and twin edge did not exist -> create new edge and its twin.
			else
			{
#ifdef DEBUG_VORONOI_BUILDAREA
				Logging::buildText(__FUNCTION__, __FILE__, "Creating new Voronoi edges ");
				Logging::buildText(__FUNCTION__, __FILE__, newEdgeId);
				Logging::buildText(__FUNCTION__, __FILE__, " and its twin ");
				Logging::buildText(__FUNCTION__, __FILE__, newEdgeId+1);
				Logging::write(true, Info);
#endif
				// If first edge inserted -> save its id.
				if (!firstInserted)
				{
					firstInserted = true;
					firstEdgeInFace = newEdgeId;
				}

				// Add edge and its twin.
				this->dcel.addEdge(voronoiOriginId, newEdgeId + 1, previousEdge, newEdgeId + 2, newVoronoiFaceId);
				this->dcel.addEdge(voronoiDestId, newEdgeId, INVALID, INVALID, INVALID);

				// Update points.
				this->dcel.updateVertex(newEdgeId, voronoiOriginId - 1);
				this->dcel.updateVertex(newEdgeId + 1, voronoiDestId - 1);

				// Update prvious and next edges id.
				previousEdge = newEdgeId;
				newEdgeId = newEdgeId + 2;
			}

			// First edge has been processed.
			previousExist = true;
		}
#ifdef DEBUG_VORONOI_BUILDAREA
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Skipping imaginary edge ");
			Logging::buildText(__FUNCTION__, __FILE__, currentEdge);
			Logging::write(true, Info);
		}
#endif
		// Update current edge and its index.
		currentEdge = this->triangulation.getTwin(this->triangulation.getPrevious(edgeIndex)-1);
		edgeIndex = currentEdge - 1;

	} while (currentEdge != firstEdge);
#ifdef DEBUG_VORONOI_BUILDAREA
	Logging::buildText(__FUNCTION__, __FILE__, "Closing external Voronoi area using edges ");
	Logging::buildText(__FUNCTION__, __FILE__, firstEdgeInFace);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, previousEdge);
	Logging::write(true, Info);
#endif
	// Update "previous" edge for first inserted edge.
	this->dcel.updateEdge(NO_UPDATE, NO_UPDATE, previousEdge, NO_UPDATE, NO_UPDATE, firstEdgeInFace - 1);

	// Update "next" edge for last inserted edge.
	this->dcel.updateEdge(NO_UPDATE, NO_UPDATE, NO_UPDATE, firstEdgeInFace, NO_UPDATE, previousEdge - 1);

	// Add new face.
	this->dcel.addFace(firstEdgeInFace);
#ifdef DEBUG_VORONOI_BUILDAREA
	Logging::buildText(__FUNCTION__, __FILE__, "Voronoi area built.");
	Logging::write(true, Info);
	Face *face;
	face = this->voronoi.getRefFace(this->voronoi.getNFaces()-1);
	face->print(std::cout);
#endif
}


/***************************************************************************
* Name: 	computeExtremeVoronoi
* IN:		edgeIndex			edge index
* 			centre				previous voronoi area centre.
* OUT:		NONE
* RETURN:	imaginary extreme point in border.
* GLOBAL:	this->voronoi
* Description: 	if a Voronoi area is an external area (not closed) it is
* 				necessary to compute an imaginary point in border area. Given
* 				the previous Voronoi area centre and a Delaunay edge whose
* 				Voronoi edge is being computed it is possible to compute
* 				this border point.
***************************************************************************/
Point<TYPE> Voronoi::computeExtremeVoronoi(int edgeIndex, Point<TYPE> &centre)
{
	Point<TYPE> *origin, *destination;		// Origin and destination edge points.
	Line line;								// Line data.
	Point<TYPE> middlePoint;				// Edge middle point.
	Edge *edge;								// Edge data.
	enum Turn_T turn;						// Turn type (left, right collinear).
	Point<TYPE> *p1, *p2;					// Aux points.
	Point<TYPE>  extreme;					// Return value.

	// Get real edge in current face.
	edge = this->triangulation.getRefEdge(edgeIndex);

	// Get Delaunay edge extreme points.
	origin = this->triangulation.getRefPoint(edge->getOrigin() - 1);
	destination = this->triangulation.getRefPoint(this->triangulation.getOrigin(edge->getTwin() - 1) - 1);

	// Compute middle point in Delaunay triangulation edge.
	line = Line(*origin, *destination);
	line.getMiddle(middlePoint);

#ifdef DEBUG_VORONOI_GETEXTREMEVORONOI
	Logging::buildText(__FUNCTION__, __FILE__, "Middle point for edge ");
	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
	Logging::buildText(__FUNCTION__, __FILE__, " whose vertex are ");
	Logging::buildText(__FUNCTION__, __FILE__, edge->getOrigin());
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, this->triangulation.getOrigin(edge->getTwin() - 1));
	Logging::buildText(__FUNCTION__, __FILE__, " is ");
	Logging::buildText(__FUNCTION__, __FILE__, &middlePoint);
	Logging::write(true, Info);
#endif
	// Check turn between edge in convex hull and previous circumcentre.
	turn = line.checkTurn(&centre);
	if (turn == RIGHT_TURN)
	{
#ifdef DEBUG_VORONOI_GETEXTREMEVORONOI
		Logging::buildText(__FUNCTION__, __FILE__,"RIGHT turn.");
		Logging::write(true, Info);
#endif
		p1 = &centre;
		p2 = &middlePoint;
	}
	// Check if voronoi centre is into the convex hull.
	else if (turn == LEFT_TURN)
	{
#ifdef DEBUG_VORONOI_GETEXTREMEVORONOI
		Logging::buildText(__FUNCTION__, __FILE__,"LEFT turn.");
		Logging::write(true, Info);
#endif
		p1 = &middlePoint;
		p2 = &centre;
	}
	else
	{
#ifdef DEBUG_VORONOI_GETEXTREMEVORONOI
		Logging::buildText(__FUNCTION__, __FILE__, "COLLINEAR.");
		Logging::write(true, Info);
#endif
		//printf("Collinear points in edge index %d\n", edgeID-1);
		// PENDING
		printf("CRITICAL ERROR\n");
		exit(0);
	}

	// Compute extreme of Voronoi edge.
	line = Line(*p1, *p2);
	line.extendToBoundary(extreme);

	return(extreme);
}

/***************************************************************************
* Name: 	edgeExists
* IN:		edge			edge to check.
* OUT:		NONE
* RETURN:	true 			if the Voronoi edge already created
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	Checks if the Voronoi edge associated to this Delaunay edge
* 				has been already created. The Voronoi edge has been already
* 				created if the destination point of the Delaunay edge is
* 				lower than origin because Delaunay points are parsed in
* 				ascending order.
***************************************************************************/
bool Voronoi::edgeExists(Edge &edge)
{
	bool 	exists=false;	// Return value.

	// If destination point < origin point -> Voronoi edge already created.
	if (this->triangulation.getOrigin(edge.getTwin()-1) < edge.getOrigin())
	{
		exists = true;
#ifdef DEBUG_VORONOI_EDGEEXISTS
		Logging::buildText(__FUNCTION__, __FILE__, "Recovering already existing edge because destination ");
		Logging::buildText(__FUNCTION__, __FILE__, this->triangulation.getOrigin(edge.getTwin()-1));
		Logging::buildText(__FUNCTION__, __FILE__, " is lower than origin point ");
		Logging::buildText(__FUNCTION__, __FILE__, edge.getOrigin());
		Logging::write(true, Info);
#endif
	}

	return(exists);
}


/***************************************************************************
* Name: 	isBottomMostFace
* IN:		faceId			id of the face to check.
* OUT:		NONE
* RETURN:	true 			if the face os the bottom most face in the
* 							triangulation
* GLOBAL:	NONE
* Description: 	Checks if the face is the bottom most face in a incremental
* 				triangulation.
***************************************************************************/
bool Voronoi::isBottomMostFace(int faceId)
{
	int	i=0;				// Loop counter.
	int	edgeIndex=0;		// Current edge index.
	int	nImaginaryPoints=0;	// # imaginary points in face.

	// Get edge in current face.
	edgeIndex = this->triangulation.getFaceEdge(faceId)-1;

	for (i=0; i<NPOINTS_TRIANGLE; i++)
	{
		// Check if origin point is lower than 0.
		if (this->triangulation.getOrigin(edgeIndex) < 0)
		{
#ifdef DEBUG_ISBOTTOMOSTFACE
			Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, " origin is ");
			Logging::buildText(__FUNCTION__, __FILE__, this->triangulation.getOrigin(edgeIndex));
			Logging::write(true, Info);
#endif
			nImaginaryPoints++;
		}
		edgeIndex = this->triangulation.getNext(edgeIndex)-1;
	}

	return(nImaginaryPoints == 2);
}
