/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <cfloat>

#include "Circle.h"
#include "defines.h"
#include "Logging.h"
#include "Stack.h"
#include "StarTriangulation.h"


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_TRIANGULATION_DEBUG
//#define DEBUG_TRIANGULATION_CONVEX_HULL
//#define DEBUG_TRIANGULATION_DELAUNAY
//#define DEBUG_TRIANGULATION_FINDTWOCLOSEST
#endif


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	convexHull
* IN:		NONE
* OUT:		NONE
* RETURN:	true if the convex hull has been computed.
* 			false otherwise.
* GLOBAL:	the convex hull is stored in the "convexHull" attribute.
* Description: 	computes the convex hull of the DCEL set of points.
***************************************************************************/
bool StarTriangulation::convexHull()
{
    bool    isBuilt=false;
	int	 	index=0;			// Array index.
	int	 	first_Index=0;		// Index of first edge.
	Point<TYPE> *point;			// Current vertex.
	Edge 	*currentEdge;		// Current edge.
	Face 	*face;				// Current face.

	try
	{
		// Get outer face.
		face = this->dcel.getRefFace(0);

		// Get index of first edge.
		index = face->getEdge() - 1;
		first_Index = index;

		// Loop convex hull points.
		this->hull.reset();
		while (!isBuilt)
		{
			// Get edge info.
			currentEdge = this->dcel.getRefEdge(index);

			// Get origin of current edge.
			point = this->dcel.getRefPoint(currentEdge->getOrigin()-1);
#ifdef DEBUG_TRIANGULATION_CONVEX_HULL
			Logging::buildText(__FUNCTION__, __FILE__, "Point to add to convex hull: ");
			Logging::buildText(__FUNCTION__, __FILE__, currentEdge->getOrigin());
			Logging::write(true);
#endif

			// Insert next point.
			this->hull.add(*point);

			// Get next edge.
			index = currentEdge->getNext()-1;
			if (index == first_Index)
			{
                isBuilt = true;
#ifdef DEBUG_TRIANGULATION_CONVEX_HULL
				Logging::buildText(__FUNCTION__, __FILE__, "Convex hull computed.");
				Logging::write(true);
#endif
			}
		}
	}
	catch (exception &ex)
	{
		ex.what();
	}

	return isBuilt;
}


bool StarTriangulation::findTwoClosest(Point<TYPE> &p, Point<TYPE> &q)
{
	bool found=false;				// Return value.
	auto  lowestDistance=DBL_MAX;	// Current distance.

	// Loop all vertex.
	for (size_t i=0; i< this->dcel.getNumVertex(); i++)
	{
		// Get origin point.
        Point<TYPE> *origin = this->dcel.getRefPoint(i);

		// Compute distance to remaining points.
		for (size_t j=(i+1); j< this->dcel.getNumVertex(); j++)
		{
			// Get destination point.
            Point<TYPE>	*dest = this->dcel.getRefPoint(j);

			// Compute distance.
            TYPE distance = origin->distance(*dest);

			// Compare to current.
			if (distance < lowestDistance)
			{
				// Update lowest distance and output indexes.
				lowestDistance = distance;
				p = *dcel.getRefPoint(i);
                q = *dcel.getRefPoint(j);
				found = true;
			}
		}
	}

	return found;
}

//bool StarTriangulation::findFace(Point<TYPE> &point, int &faceId)
//{
//	bool 	found=false;		// Return value.
//
//	// TODO https://github.com/juannavascalvente/Delaunay/issues/63
//
//	return(found);
//}


bool StarTriangulation::findClosestPoint(Point<TYPE> &p, Point<TYPE> &q)
{
	bool found=false;				// Return value.

	// Initialize loop.
	TYPE shortestDistance = FLT_MAX;
    int	pointIndex=0;				// Loop counter.
	for (pointIndex=0; pointIndex<this->dcel.getNumVertex() ;pointIndex++)
	{
		// Get current point.
        Point<TYPE> *currentPoint = this->dcel.getRefPoint(pointIndex);

		// Check if new distance is lowest.
        TYPE dist = currentPoint->distance(p);
		if (dist < shortestDistance)
		{
			// Update output data.
			q = *currentPoint;
            shortestDistance = dist;
			found = true;
		}
	}

	return found;
}


bool StarTriangulation::build()
{
	bool	built=true;					// Return value.
	int		i=0;						// Loop counter.
	int		len=0;						// Loop length.
	struct ConvexPoint	convexPoint={};	// Point to insert in convex hull.
	struct ConvexPoint	convexEdge;		// Point to insert in convex hull.
	struct ConvexPoint	peakEdge;		// Last edge in convex hull.
	int	   convex_Peak[2];				// Last two points in convex hull.
	int		edgeId=0, faceId=0;			// Edge and face Id counters.
	int		savedEdge=0;
	int		nextConvexEdge=0;			// Next edge to add to convex hull.
	int		originVertex1=0, originVertex2=0;
	int		originVertexIndex1=0, originVertexIndex2=0;
	int		lowestPointIndex=0;			// Index of the lowest point.
	bool	finished;					// Loop control flag.
#ifdef STATISTICS_STAR_TRIANGULATION
	this->nCollinear = 0;
#endif

	// Allocate stack.
	Stack<ConvexPoint> stack = Stack<ConvexPoint>(DEFAUTL_CONVEXHULL_LEN);

	// Reset convex hull computed flag and set dcel.
	this->reset();

	// Set lowest Y coordinates point first.
	lowestPointIndex = this->dcel.getIndexLowest(&Point<TYPE>::lowerY);
	this->dcel.swapVertex(0, lowestPointIndex);

	// Sort DCEl set of points.
	this->dcel.sort();
#ifdef DEBUG_TRIANGULATION_DEBUG
	Logging::buildText(__FUNCTION__, __FILE__, "DCEL set sorted.");
		Logging::write(true, Info);
#endif
	// Insert invalid face. Updated when convex hull added.
	this->dcel.addFace(-1);

	// First vertex belongs to convex hull.
	convexPoint.vertexIndex = 0;
	convexPoint.edgeID = 1;
	stack.push(convexPoint);

	// Insert first edge.
	this->dcel.updateVertex(1, 0);
	this->dcel.addEdge(1, -1, 3, 2, 1);

	// Second vertex belongs to convex hull.
	convexPoint.vertexIndex = 1;
	convexPoint.edgeID = 2;
	stack.push(convexPoint);

	// Insert second edge.
	this->dcel.updateVertex(2, 1);
	this->dcel.addEdge(2, -1, 1, 3, 1);

	// Third element is inserted in convex hull at starting.
	convexPoint.vertexIndex = 2;
	convexPoint.edgeID = 3;
	stack.push(convexPoint);

	// Insert third edge.
	this->dcel.updateVertex(3, 2);
	this->dcel.addEdge(3, -1, 2, 1, 1);

	// Insert first face.
	this->dcel.addFace(1);

	// Initialize variables before loop.
	faceId = 2;
	edgeId = 4;

	// Parse rest of points.
	for (i=3; i< this->dcel.getNumVertex() ; i++)
	{
#ifdef DEBUG_TRIANGULATION_DEBUG
		Logging::buildText(__FUNCTION__, __FILE__, "-----------------------------------------------------");
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, "Inserting point index ");
			Logging::buildText(__FUNCTION__, __FILE__, i);
			Logging::write(true, Info);
#endif
		// Initialize indexes and values.
		originVertexIndex1 = 0;
		originVertex1 = 1;
		originVertexIndex2 = i-1;
		originVertex2 = i;

		finished = false;
		savedEdge = INVALID;
		nextConvexEdge = edgeId+2;
		peakEdge = stack.peak();
		while (!finished)
		{
			// Get last two vertex from convex hull.
			convexEdge = stack.peak();
			convex_Peak[0] = convexEdge.vertexIndex;
			convexEdge = stack.elementAt(2);
			convex_Peak[1] = convexEdge.vertexIndex;
#ifdef DEBUG_TRIANGULATION_DEBUG
			Logging::buildText(__FUNCTION__, __FILE__, "Points recovered from convex hull are ");
				Logging::buildText(__FUNCTION__, __FILE__, convex_Peak[0]);
				Logging::buildText(__FUNCTION__, __FILE__, " and ");
				Logging::buildText(__FUNCTION__, __FILE__, convex_Peak[1]);
				Logging::write(true, Info);
#endif

			// Insert new edge and update its twin.
			this->dcel.updateVertex(edgeId, originVertexIndex1);
			this->dcel.setTwin(peakEdge.edgeID-1, edgeId);
			this->dcel.addEdge(originVertex1, peakEdge.edgeID, edgeId+2, edgeId+1, faceId);

			// Insert new edge.
			this->dcel.updateVertex(edgeId+1, originVertexIndex2);
			this->dcel.addEdge(originVertex2, INVALID, edgeId, edgeId+2, faceId);

			// Insert new edge.
			this->dcel.updateVertex(edgeId+2, i);
			this->dcel.addEdge(i+1, savedEdge, edgeId+1, edgeId, faceId);
			if (savedEdge != -1)
			{
				this->dcel.setTwin(savedEdge-1, edgeId+2);
			}
			savedEdge = edgeId+1;
#ifdef DEBUG_TRIANGULATION_DEBUG
			Logging::buildText(__FUNCTION__, __FILE__, "Checking turn between points: ");
				Logging::buildText(__FUNCTION__, __FILE__, convex_Peak[1]);
				Logging::buildText(__FUNCTION__, __FILE__, ",");
				Logging::buildText(__FUNCTION__, __FILE__, convex_Peak[0]);
				Logging::buildText(__FUNCTION__, __FILE__, " and ");
				Logging::buildText(__FUNCTION__, __FILE__, i);
				Logging::write(true, Info);

				if (Debug::outOfBouds(convex_Peak[1], 0, this->dcel.getNVertex()))
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Index " );
					Logging::buildText(__FUNCTION__, __FILE__, convex_Peak[1]);
					Logging::buildText(__FUNCTION__, __FILE__, "out of bounds ");
					Logging::buildRange(__FUNCTION__, __FILE__, 0, this->dcel.getNVertex());
					Logging::write(true, Error);
				}
				else if (Debug::outOfBouds(convex_Peak[0], 0, this->dcel.getNVertex()))
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Index " );
					Logging::buildText(__FUNCTION__, __FILE__, convex_Peak[0]);
					Logging::buildText(__FUNCTION__, __FILE__, "out of bounds ");
					Logging::buildRange(__FUNCTION__, __FILE__, 0, this->dcel.getNVertex());
					Logging::write(true, Error);
				}
				else if (Debug::outOfBouds(i, 0, this->dcel.getNVertex()))
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Index " );
					Logging::buildText(__FUNCTION__, __FILE__, i);
					Logging::buildText(__FUNCTION__, __FILE__, "out of bounds ");
					Logging::buildRange(__FUNCTION__, __FILE__, 0, this->dcel.getNVertex());
					Logging::write(true, Error);
				}
#endif
			// Check type of turn with new point.
			if ((this->dcel.getRefPoint(convex_Peak[1])->check_Turn(*this->dcel.getRefPoint(convex_Peak[0]), *this->dcel.getRefPoint(i)) == LEFT_TURN) ||
				(this->dcel.getRefPoint(convex_Peak[1])->check_Turn(*this->dcel.getRefPoint(convex_Peak[0]), *this->dcel.getRefPoint(i)) == COLLINEAR))
			{
#ifdef STATISTICS_STAR_TRIANGULATION
				if ((this->dcel.getRefPoint(convex_Peak[1])->check_Turn(*this->dcel.getRefPoint(convex_Peak[0]), *this->dcel.getRefPoint(i)) == COLLINEAR))
					{
						this->nCollinear++;
					}
#endif
				// Update peak of convex hull.
				peakEdge = stack.peak();
				peakEdge.edgeID = edgeId+1;
				stack.updatePeak(peakEdge);
#ifdef DEBUG_TRIANGULATION_DEBUG
				Logging::buildText(__FUNCTION__, __FILE__, "Inserted point ");
					Logging::buildText(__FUNCTION__, __FILE__, i);
					Logging::buildText(__FUNCTION__, __FILE__, " belongs to convex hull.");
					Logging::write(true, Info);
					Logging::buildText(__FUNCTION__, __FILE__, "Updating peak with edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeId+1);
					Logging::write(true, Info);
#endif
				// Add edge to convex hull.
				convexEdge.vertexIndex = i;
				convexEdge.edgeID = nextConvexEdge;
				stack.push(convexEdge);
				finished = true;
#ifdef DEBUG_TRIANGULATION_DEBUG
				Logging::buildText(__FUNCTION__, __FILE__, "Push point ");
					Logging::buildText(__FUNCTION__, __FILE__, convexEdge.vertexIndex);
					Logging::buildText(__FUNCTION__, __FILE__, " and edge ");
					Logging::buildText(__FUNCTION__, __FILE__, convexEdge.edgeID);
					Logging::write(true, Info);
#endif
			}
			else
			{
				// Update source points.
				convexEdge = stack.peak();
				originVertexIndex1 = convexEdge.vertexIndex;
				originVertex1 = convexEdge.vertexIndex + 1;

				// Pop convex hull.
#ifdef DEBUG_TRIANGULATION_DEBUG
				Logging::buildText(__FUNCTION__, __FILE__, "Right TURN. Pop element from stack.");
					Logging::write(true, Info);
#endif
				stack.pop();
				peakEdge = stack.peak();
				originVertexIndex2 = peakEdge.vertexIndex;
				originVertex2 = peakEdge.vertexIndex + 1;
			}

			// Insert face.
			this->dcel.addFace(edgeId);

			// Update counters.
			faceId++;
			edgeId = edgeId + 3;
		}
	}

	// Save index of first edge from convex hull.
	convexEdge = stack.peak();
	savedEdge = convexEdge.edgeID;

	stack.pop();
	peakEdge = stack.peak();

	// Insert first edge from convex hull.
	this->dcel.setTwin(peakEdge.edgeID - 1, edgeId);
	this->dcel.addEdge(convexEdge.vertexIndex+1, peakEdge.edgeID,
					   edgeId + stack.getLength(),
					   edgeId + 1,
					   0);

	// Next edge.
	edgeId = edgeId+1;

	// Update convex hull.
	len = stack.getLength()-1;
	for (i=0; i<len ;i++)
	{
		convexEdge = stack.peak();
		stack.pop();
		peakEdge = stack.peak();

		// Insert new edge from convex hull.
		this->dcel.setTwin(peakEdge.edgeID - 1, edgeId);
		this->dcel.addEdge(convexEdge.vertexIndex+1, peakEdge.edgeID, edgeId-1, edgeId+1, 0);

		// Next edge.
		edgeId++;
	}

	// Insert first edge from convex hull.
	this->dcel.setTwin(savedEdge - 1, edgeId);
	this->dcel.addEdge(1, savedEdge, edgeId-1, edgeId-len-1, 0);

	// Update convex hull face departing edge.
	this->dcel.updateFace(edgeId, 0);

	return built;
}

/***************************************************************************
* Name: 		delaunay
* IN:			NONE
* OUT:			NONE
* RETURN:		true if the delaunay triangulation has been computed.
* 				false otherwise
* GLOBAL:		NONE
* Description: 	transforms the star triangulation into a Delaunay
* 				triangulation.
***************************************************************************/
bool StarTriangulation::delaunay()
{
	// PENDING TO CHECK ERROR WHILE BUILDING DELAUNAY.
	bool	built=true;			// Return value.
	int		edgeIndex=0;		// Edge index.
	int		originPoint;		// Origin point.
	Edge	*edge;				// Pointer to current edge.
	Edge	*twin;				// Pointer to twin edge of current edge.
#ifdef STATISTICS_STAR_TRIANGULATION
	this->nFlips = 0;
#endif

	// Initialize variables.
	edgeIndex=0;									// First edge index.
	this->nPending = this->dcel.getNumEdges();		// # edges to check.
	bool *edgeChecked = new bool[this->nPending];	// Already checked edges array.
	memset(edgeChecked, false, sizeof(bool)*this->nPending);

#ifdef DEBUG_TRIANGULATION_DELAUNAY
	Logging::buildText(__FUNCTION__, __FILE__, "Convert to Delaunay triangulation. # edges to check ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nPending);
	Logging::write(true, Info);
	this->dcel.print(std::cout);
#endif

	// Analyze all edges.
	while (this->nPending > 0)
	{
		// Check edge is still pending.
		if (!edgeChecked[edgeIndex])
		{
#ifdef DEBUG_TRIANGULATION_DELAUNAY
			Logging::buildText(__FUNCTION__, __FILE__, "Checking edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::write(true, Info);
#endif
			// Get edge information.
			edge = this->dcel.getRefEdge(edgeIndex);

			// Check edge is not in convex hull.
			if (!this->dcel.isExternalEdge(edgeIndex))
			{
				// Create face circle.
				vector<Point<TYPE>> vPoints;
                vPoints.push_back(*this->dcel.getRefPoint(edge->getOrigin()-1));
                vPoints.push_back(*this->dcel.getRefPoint(this->dcel.getOrigin(edge->getNext()-1)-1));
                vPoints.push_back(*this->dcel.getRefPoint(this->dcel.getOrigin(edge->getPrevious()-1)-1));
				Circle circle = Circle(vPoints);

				// Get twin edge.
				twin = this->dcel.getRefEdge(this->dcel.getTwin(edgeIndex)-1);
#ifdef DEBUG_TRIANGULATION_DELAUNAY
				Logging::buildText(__FUNCTION__, __FILE__, "Creating circle with points ");
				Logging::buildText(__FUNCTION__, __FILE__, edge->getOrigin());
				Logging::buildText(__FUNCTION__, __FILE__, ",");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getOrigin(edge->getNext()-1));
				Logging::buildText(__FUNCTION__, __FILE__, " and ");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getOrigin(edge->getPrevious()-1));
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Point to check ");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getOrigin(twin->getPrevious()-1));
				Logging::write(true, Info);
#endif

				// Check if remaining point from adjacent face is in circle.
				if (circle.inCircle(*this->dcel.getRefPoint(this->dcel.getOrigin(twin->getPrevious()-1)-1)))
				{
#ifdef STATISTICS_STAR_TRIANGULATION
					this->nFlips++;
#endif
#ifdef DEBUG_TRIANGULATION_DELAUNAY
					Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::buildText(__FUNCTION__, __FILE__, " and its twin must be flipped.");
					Logging::write(true, Info);
					Logging::buildText(__FUNCTION__, __FILE__, edge->toStr());
					Logging::buildText(__FUNCTION__, __FILE__, twin->toStr());
					Logging::write(true, Info);
#endif
					// Update vertex.
					this->dcel.updateVertex(twin->getNext(), edge->getOrigin()-1);
					this->dcel.updateVertex(edge->getNext(), twin->getOrigin()-1);

					// Update origin of current and twin edges.
					originPoint = this->dcel.getOrigin(edge->getPrevious()-1);
					this->dcel.setOrigin(twin->getTwin()-1, this->dcel.getOrigin(twin->getPrevious()-1));
					this->dcel.setOrigin(edge->getTwin()-1, originPoint);

					// Update next edges.
					this->dcel.setNext(edge->getNext()-1, edge->getTwin());
					this->dcel.setNext(twin->getNext()-1, twin->getTwin());
					this->dcel.setNext(edge->getPrevious()-1, twin->getNext());
					this->dcel.setNext(twin->getPrevious()-1, edge->getNext());
					this->dcel.setNext(twin->getTwin()-1, this->dcel.getPrevious(twin->getTwin()-1));
					this->dcel.setNext(edge->getTwin()-1, this->dcel.getPrevious(edge->getTwin()-1));

					// Update previous edges.
					this->dcel.setPrevious(twin->getTwin()-1, this->dcel.getNext(edge->getNext()-1));
					this->dcel.setPrevious(edge->getTwin()-1, this->dcel.getNext(twin->getNext()-1));
					this->dcel.setPrevious(edge->getNext()-1, this->dcel.getNext(edge->getPrevious()-1));
					this->dcel.setPrevious(twin->getNext()-1, this->dcel.getNext(twin->getPrevious()-1));
					this->dcel.setPrevious(edge->getPrevious()-1, this->dcel.getNext(twin->getTwin()-1));
					this->dcel.setPrevious(twin->getPrevious()-1, this->dcel.getNext(edge->getTwin()-1));

					// Update faces in edges.
					this->dcel.setFace(edge->getPrevious()-1, edge->getFace());
					this->dcel.setFace(twin->getPrevious()-1, twin->getFace());

					// Update faces.
					this->dcel.updateFace(twin->getTwin(), edge->getFace());
					this->dcel.updateFace(edge->getTwin(), twin->getFace());

					// Update pending edges.
					edgeChecked[edgeIndex] = true;
					this->nPending--;

					// If twin not checked set as checked.
					if (!edgeChecked[edge->getTwin()-1])
					{
						this->nPending--;
						edgeChecked[edge->getTwin()-1] = true;
					}

					// Update pending edges.
					this->setNotChecked(edge->getPrevious() - 1, edgeChecked);
					this->setNotChecked(edge->getNext() - 1, edgeChecked);
					this->setNotChecked(twin->getPrevious() - 1, edgeChecked);
					this->setNotChecked(twin->getNext() - 1, edgeChecked);
#ifdef DEBUG_TRIANGULATION_DELAUNAY
					Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::buildText(__FUNCTION__, __FILE__, " flipped. Pending edges: ");
					Logging::buildText(__FUNCTION__, __FILE__, this->nPending);
					Logging::write(true, Info);

					Logging::buildText(__FUNCTION__, __FILE__, "New faces are ");
					Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getRefFace(edge->getFace())->toStr());
					Logging::buildText(__FUNCTION__, __FILE__, " and ");
					Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getRefFace(twin->getFace())->toStr());
					Logging::write(true, Info);
#endif
				}
				// Edge OK -> Not to be flipped.
				else
				{
					edgeChecked[edgeIndex] = true;
					this->nPending--;
#ifdef DEBUG_TRIANGULATION_DELAUNAY
					Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::buildText(__FUNCTION__, __FILE__, " is OK. Pending edges: ");
					Logging::buildText(__FUNCTION__, __FILE__, this->nPending);
					Logging::write(true, Info);
#endif
				}
			}
			else
			{
				// Edge in convex hull -> Not to be flipped.
				edgeChecked[edgeIndex] = true;
				this->nPending--;
#ifdef DEBUG_TRIANGULATION_DELAUNAY
				Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, " or its twin ");
				Logging::buildText(__FUNCTION__, __FILE__, edge->getTwin());
				Logging::buildText(__FUNCTION__, __FILE__, " is in external face. Pending edges: ");
				Logging::buildText(__FUNCTION__, __FILE__, this->nPending);
				Logging::write(true, Info);
#endif
			}
		}
#ifdef DEBUG_TRIANGULATION_DELAUNAY
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, " already checked.");
			Logging::write(true, Info);
		}
#endif
		// Next edge.
		edgeIndex++;
		edgeIndex = edgeIndex % this->dcel.getNumEdges();
	}

	// Deallocate checked boolean flag array.
	delete[] edgeChecked;

	return(built);
}


/**
 * @fn			setNotChecked
 * @brief		updates the "index" edge as pending to be checked.
 *
 * @param index				(IN)	Index to update
 * @param isEdgeChecked 	(IN) 	Array to check
 *
 * @return		false if index out of bounds or edge is in external face
 * 				true otherwise
 */
bool StarTriangulation::setNotChecked(int index, bool *isEdgeChecked)
{
	bool updated=false;		// Return value.

	// Check if index out of bounds.
	if (index < this->dcel.getNumEdges())
	{
		// If edge or its twin is in external face then do not update.
		if (!this->dcel.isExternalEdge(index))
		{
			// If edge already checked then switch state to check it again.
			if (isEdgeChecked[index])
			{
				this->nPending++;
				isEdgeChecked[index] = false;
				updated = true;
			}
		}
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Index out of bounds when trying to update checked edge ");
		Logging::buildText(__FUNCTION__, __FILE__, index+1);
		Logging::buildRange(__FUNCTION__, __FILE__, 0, this->dcel.getNumEdges());
		Logging::write(true, Error);
		updated = false;
	}

	return(updated);
}
