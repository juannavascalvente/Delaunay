/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Circle.h"
#include "defines.h"
#include "Logging.h"
#include "Stack.h"
#include "StarTriangulation.h"

#include <cfloat>
#include <queue>


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
* Private definition
***********************************************************************************************************************/
struct ConvexPoint
{
    int		vertexIndex;
    int		edgeID;
};



/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool StarTriangulation::convexHull()
{
    bool    isHullBuilt=false;

	try
	{
		// Get outer face.
        Face *face = this->dcel.getRefFace(0);

		// Get index of first edge.
        int iCurrentIdx = face->getEdge() - 1;
        int iFirstIdx = iCurrentIdx;

		// Loop convex hull points.
		this->hull.reset();

        Point<TYPE> *point;			// Current point
        Edge 	*currentEdge;		// Current edge
		while (!isHullBuilt)
		{
			// Get edge info.
			currentEdge = this->dcel.getRefEdge(iCurrentIdx);

			// Get origin of current edge.
			point = this->dcel.getRefPoint(currentEdge->getOrigin()-1);

			// Insert next point.
			this->hull.add(*point, iCurrentIdx + 1);

			// Get next edge.
			iCurrentIdx = currentEdge->getNext() - 1;
			if (iCurrentIdx == iFirstIdx)
			{
                isHullBuilt = true;
			}
		}
	}
	catch (exception &ex)
	{
		ex.what();
	}

	return isHullBuilt;
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


bool StarTriangulation::findFace(Point<TYPE> &point, int &faceId)
{
    bool isSuccess=false;           // Return value

    // Get edge from a random point
    int iIdx=(int) drand48();
    iIdx %= dcel.getNumVertex();

    // Get point face
    int iEdgeIdx = dcel.getPointEdge(iIdx) - 1;
    faceId = dcel.getFace(iEdgeIdx);

    // Get face centroid (face is a triangle)
    vector<Point<TYPE>> vPoints;
    dcel.getFacePoints(faceId, vPoints);
    Polygon triangle(vPoints);
    Point<TYPE> centre;
    triangle.centroid(centre);

    // Compute line between face centroid and point to locate
    Line l (point, centre);

    bool isFinished=false;
    do
    {
        // Get edge face
        faceId = dcel.getFace(iEdgeIdx);

        // Check if point is interior to current face
        if (dcel.isInsideFace(point, faceId))
        {
            // Face found
            isFinished = true;
            isSuccess = true;
        }
        // Move to adjacent face
        else
        {
            // Point is out of convex hull
            if (faceId == EXTERNAL_FACE)
            {
                isFinished = true;
                isSuccess = true;
            }
            else
            {
                // Find out what edge intersects line between face centre and target point
                queue<int> qEdgesId;
                if (dcel.getEdgeIntersection(l, faceId, qEdgesId))
                {
                    // If two edges intersect line -> remove the one that is equal to previous edge index
                    if (qEdgesId.size() == 2)
                    {
                        if ((qEdgesId.front()-1) == iEdgeIdx)
                        {
                            qEdgesId.pop();
                        }
                    }

                    // Get edge that intersects
                    iEdgeIdx = qEdgesId.front() - 1;
                    qEdgesId.pop();

                    // Get twin edge to move to next face in next iteration
                    iEdgeIdx = dcel.getTwin(iEdgeIdx) - 1;
                }
                else
                {
                    isFinished = true;
                    isSuccess = false;
                    cout << "Error finding face in Star triangulation. No face found" << endl;
                }
            }
        }

    } while (!isFinished);

    return isSuccess;
}


/**
 * @fn    getConvexFacesIntersections
 * @brief Get faces and edge in convex hull that intersects input line
 *
 * @param line      (IN) Line to check
 * @param vFaces    (OUT) Faces in convex hull that intersect line
 * @param vEdges    (OUT) Edges in convex hull that intersect line
 */
void StarTriangulation::getConvexFacesIntersections(Line &line, vector<int> &vFaces, vector<int> &vEdges)
{
    // Initialize output
    vFaces.clear();

    // Get convex hull edges
    vector<int> vConvexEdges;
    hull.getConvexHullEdges(vConvexEdges);

    Point<TYPE> o;
    Point<TYPE> d;
    for (auto edgeId : vConvexEdges)
    {
        // Get edge extreme points
        dcel.getEdgePoints(edgeId - 1, o, d);

        // Build edge line
        Line l(o, d);

        // If line intersects -> add face
        if (line.intersect(l))
        {
            int iTwinEdge = dcel.getTwin(edgeId - 1);
            vFaces.push_back(dcel.getFace(iTwinEdge - 1));
            vEdges.push_back(edgeId);
        }
    }
}


bool StarTriangulation::findPath(Point<TYPE> &origin, Point<TYPE> &dest, vector<int> &vFacesId)
{
    bool isSuccess=false;           // Return value

    // Initialize output
    vFacesId.clear();

    // Set origin point as starting point of the path
    Point<TYPE> startPoint = origin;

    // Compute convex hull if not already computed
    if (!isConvexHullComputed())
    {
        convexHull();
    }

    // Get convex hull
    auto *polygon = new Polygon();
    getConvexHull(*polygon);

    // Check if origin and destination are internal to convex hull
    bool isOriginInterior = polygon->isInternal(origin);

    int faceId=0;
    int iEdgeIdx=0;

    // Check if origin point is not interior -> get first face in convex hull
    bool isFaceFound=true;
    if (!isOriginInterior)
    {
        // Get faces that intersect line
        Line line(origin, dest);
        vector<int> vFaces;
        vector<int> vEdges;
        getConvexFacesIntersections(line, vFaces, vEdges);

        // If line intersect twice convex hull -> Select closest face.
        if (vFaces.size() == 2)
        {
            // Get origin point for edges
            Point<TYPE> *p1 = dcel.getRefPoint(dcel.getOrigin(vEdges.at(0) - 1) - 1);
            Point<TYPE> *p2 = dcel.getRefPoint(dcel.getOrigin(vEdges.at(1) - 1) - 1);

            // Closest point to origin line point is the face that must be used as starting point
            TYPE distanceOrigin = origin.distance(*p1);
            TYPE distanceDestination = origin.distance(*p2);
            if (distanceOrigin < distanceDestination)
            {
                faceId = vFaces.at(0);
                iEdgeIdx = vEdges.at(0) - 1;
                iEdgeIdx = dcel.getTwin(iEdgeIdx) - 1;
            }
            else
            {
                faceId = vFaces.at(1);
                iEdgeIdx = vEdges.at(1) - 1;
                iEdgeIdx = dcel.getTwin(iEdgeIdx) - 1;
            }
        }
        // Line intersects convex hull once -> select that face
        else if (vFaces.size() == 1)
        {
            faceId = vFaces.at(0);
            iEdgeIdx = vEdges.at(0) - 1;
            iEdgeIdx = dcel.getTwin(iEdgeIdx) - 1;
        }
        // No intersection between convex hull and line between input points -> both are external to convex hull
        else
        {
            isFaceFound = false;
        }
    }
    // Origin point is interior -> find origin point face
    else
    {
        findFace(startPoint, faceId);
        iEdgeIdx = dcel.getFaceEdge(faceId) - 1;
    }

    // Line between points and set of points do not intersect -> there is no faces path
    if (!isFaceFound)
    {
        return true;
    }

    // Compute line between start and destination points
    Line l (startPoint, dest);

    bool isFinished=false;
    do
    {
        // Check if point is interior to current face
        if (dcel.isInsideFace(dest, faceId))
        {
            // Add last face and finish
            isFinished = true;
            isSuccess = true;
            vFacesId.push_back(faceId);
        }
        // Move to adjacent face
        else
        {
            // Point is out of convex hull
            if (faceId == EXTERNAL_FACE)
            {
                isFinished = true;
                isSuccess = true;
            }
            else
            {
                // Find out what edge in current face intersects line between origin and end points
                queue<int> qEdgesId;
                if (dcel.getEdgeIntersection(l, faceId, qEdgesId))
                {
                    // If two edges intersect line -> remove the one that is equal to previous edge index
                    if (qEdgesId.size() == 2)
                    {
                        if ((qEdgesId.front()-1) == iEdgeIdx)
                        {
                            qEdgesId.pop();
                        }
                    }

                    // Add face
                    vFacesId.push_back(faceId);

                    // Get edge that intersects
                    iEdgeIdx = qEdgesId.front() - 1;
                    qEdgesId.pop();

                    // Get twin edge to move to next face in next iteration
                    iEdgeIdx = dcel.getTwin(iEdgeIdx) - 1;

                    // Get next face
                    faceId = dcel.getFace(iEdgeIdx);
                }
                else
                {
                    isFinished = true;
                    isSuccess = false;
                    cout << "Error finding face in Star triangulation. No face found" << endl;
                }
            }
        }

    } while (!isFinished);

    return isSuccess;
}


bool StarTriangulation::findClosestPoint(Point<TYPE> &in, Voronoi *voronoi, Point<TYPE> &out, int &pointIndex)
{
	bool found=false;				// Return value.

	// Initialize loop.
	TYPE shortestDistance = FLT_MAX;
    pointIndex=0;				// Loop counter.
	for (pointIndex=0; pointIndex<this->dcel.getNumVertex() ;pointIndex++)
	{
		// Get current point.
        Point<TYPE> *currentPoint = this->dcel.getRefPoint(pointIndex);

		// Check if new distance is lowest.
        TYPE dist = currentPoint->distance(in);
		if (dist < shortestDistance)
		{
			// Update output data.
            out = *currentPoint;
            shortestDistance = dist;
			found = true;
		}
	}

	return found;
}


bool StarTriangulation::build()
{
	int		i=0;						// Loop counter.
	int		len=0;						// Loop length.
	struct ConvexPoint	convexPoint={};	// Point to insert in convex hull.
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
        struct ConvexPoint peakEdge = stack.peak();
		while (!finished)
		{
			// Get last two vertex from convex hull.
            struct ConvexPoint convexEdge = stack.peak();
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
    struct ConvexPoint convexEdge = stack.peak();
	savedEdge = convexEdge.edgeID;

	stack.pop();
    struct ConvexPoint peakEdge = stack.peak();

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

    isBuilt = true;
	return isBuilt;
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
		edgeIndex = edgeIndex % (int) this->dcel.getNumEdges();
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
