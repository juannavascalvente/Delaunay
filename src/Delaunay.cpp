/*
 * Delaunay.cpp
 *
 *  Created on: Jul 11, 2016
 *      Author: jnavas
 */

#include "Delaunay.h"
#include "Logging.h"
#include "Point.h"
#include "Queue.h"
#include "Triangle.h"
#include "Voronoi.h"
#include <float.h>

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_DELAUNAY
//#define DEBUG_SPLIT_NODE
//#define DEBUG_IS_STRICTLY_INTERIOR_TO_NODE
//#define DEBUG_IS_INTERIOR_TO_NODE
//#define DEBUG_DELAUNAY_INSERTPOINT
//#define DEBUG_DELAUNAY_LOCATENODE
//#define DEBUG_NODE
//#define DEBUG_GET_CONVEX_HULL
//#define DEBUG_FLIP_EDGES
//#define DEBUG_CHECK_EDGES
//#define DEBUG_DELAUNAY_INCREMENTAL
//#define DEBUG_GRAPH_INITIALIZEGRAPH
//#define DEBUG_DELAUNAY_FINDTWOCLOSEST
//#define DEBUG_DELAUNAY_FINDFACE
//#define DEBUG_DELAUNAY_GETINITIALFACES
//#define DEBUG_DELAUNAY_GET_INTERNAL_FACE
//#define DEBUG_DELAUNAY_FIND_CLOSESTPOINT
//#define DEBUG_DELAUNAY_FIND_CLOSESTPOINT_ANCHORS
#define DEBUG_DELAUNAY_FIND_TRIANG_PATH
#endif

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
//Delaunay::Delaunay()
//{
//	try
//	{
//		this->algorithm= NONE;
//		this->dcel = NULL;
//		this->graph = NULL;
//		this->graphAllocated = false;
//		this->hull = new Polygon(DEFAUTL_CONVEXHULL_LEN);
//		this->convexHullComputed = false;
//	}
//	catch (std::bad_alloc& ba)
//	{
//		std::cerr << "bad_alloc caught: " << ba.what() << endl;
//	}
//}
//
//Delaunay::Delaunay(Dcel *dcel)
//{
//	try
//	{
//		this->algorithm= NONE;
//		this->dcel = dcel;
//		this->graphAllocated = true;
//		this->graph = new Graph(this->dcel->getNVertex()*10);
//		this->hull = new Polygon(DEFAUTL_CONVEXHULL_LEN);
//		this->convexHullComputed = false;
//	}
//	catch (std::bad_alloc& ba)
//	{
//		std::cerr << "bad_alloc caught: " << ba.what() << endl;
//	}
//}

Delaunay::~Delaunay()
{
	this->dcel = NULL;
	if (this->graph != NULL)
	{
		//this->graph->~Graph();
		// PENDING TO UNCOMMENT.
		delete this->graph;
	}
	this->setGraphAllocated(false);
	this->setConvexHullComputed(false);
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	reset
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description:
***************************************************************************/
void Delaunay::reset()
{
	// Reset data flags.
	this->setGraphAllocated(false);
	this->setConvexHullComputed(false);
}

/***************************************************************************
* Name: 	incremental
* IN:		node		node whose area must be computed
* OUT:		NONE
* RETURN:	true		if triangulation built
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	computes the Delaunay triangulation using the incremental
* 				algorithm.
***************************************************************************/
//#define DELAUNAY_STATISTICS
bool Delaunay::incremental()
{
	bool 	built=true;		// Return value.
	bool	inserted=true;	// Point inserted flag.
	int  	pointIndex=0;	// Points loop counter.
	int	 	nPoints=0;		// Loop upper bound.

	// Set type of algorithm.
	this->setAlgorithm(INCREMENTAL);

	// Check if DCEL data is referenced.
	if (this->dcel == NULL)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "DCEL not referenced");
		Logging::write(true, Error);
		built = false;
	}
	else
	{
		// If no graph allocated then create a new graph.
		if (this->initializeGraph())
		{
#ifdef DELAUNAY_STATISTICS
			// Initialize graph statistics (first 2 points are located in first triangle).
			delaunay_Stat.trianglesFound[0] = 2;
			delaunay_Stat.trianglesFound[1] = 0;
			delaunay_Stat.trianglesFound[2] = 0;
			delaunay_Stat.nFlipped = 0;
#endif
			// Set highest point at first position of the DCEL vertex array.
			this->dcel->setHighestFirst(&Point<TYPE>::lexicographicHigher);

			// Insert root node.
			Node node(1, P_MINUS_2, P_MINUS_1, 1);
			this->graph->insert(node);

			// Update edge from new point.
			this->dcel->updateVertex(1, 0);

			// Insert first 6 edges due to first point.
			this->dcel->addEdge(1, 4, 3, 2, 1);
			this->dcel->addEdge(P_MINUS_2, 6, 1, 3, 1);
			this->dcel->addEdge(P_MINUS_1, 5, 2, 1, 1);
			this->dcel->addEdge(P_MINUS_2, 1, 6, 5, 0);
			this->dcel->addEdge(1, 3, 4, 6, 0);
			this->dcel->addEdge(P_MINUS_1, 2, 5, 4, 0);

			// Insert first internal face and external face.
			this->dcel->addFace(4);
			this->dcel->addFace(1);
#ifdef DEBUG_DELAUNAY_INCREMENTAL
			Logging::buildText(__FUNCTION__, __FILE__, "Starting triangulation. Set length is: ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getNVertex());
			Logging::write(true, Info);
#endif
			// Reset convex hull flag.
			this->setConvexHullComputed(false);
			this->hull.reset();
			this->hullEdges.reset();

			// Loop all other points.
			pointIndex=1;
			nPoints = this->dcel->getNVertex();
			inserted = built;
			while ((pointIndex<nPoints) && (inserted))
			{
#ifdef DEBUG_DELAUNAY_INCREMENTAL
				Logging::buildText(__FUNCTION__, __FILE__, "Inserting point ");
				Logging::buildText(__FUNCTION__, __FILE__, pointIndex);
				Logging::write(true, Info);
#endif
				// Insert new point into triangle where it is located.
				inserted = this->insertPoint(pointIndex);
				built = inserted;
				pointIndex++;
			}

#ifdef DEBUG_DELAUNAY_INCREMENTAL
			Logging::buildText(__FUNCTION__, __FILE__, "Delaunay triangulation computed");
			Logging::write(true, Info);
#endif
		}
	}

   	return(built);
}


void Delaunay::checkEdge(int edge_ID)
{
	bool flipEdges=false;			// Flip needed flag.
	int	 edgeIndex=0;				// Edge index.

	Point<TYPE> *common1=NULL, *common2=NULL, *p=NULL, *q=NULL;

	// Get edge index.
	edgeIndex = edge_ID-1;

	// Check if the edge is NOT in the external face.
	flipEdges = false;
	if (!this->dcel->isExternalEdge(edgeIndex))
	{
		// Check if any of the vertex of current edge is P-2 or P-1.
		if (this->dcel->hasNegativeVertex(edge_ID))
		{
			// Check if any of the vertex of incident faces is P-2 or P-1.
			if ((this->dcel->getOrigin(this->dcel->getPrevious(edgeIndex)-1) > 0) &&
				(this->dcel->getOrigin(this->dcel->getPrevious(this->dcel->getTwin(edgeIndex)-1)-1) > 0))
			{
				// Get points of incident faces to current edge.
				p = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getPrevious(edgeIndex)-1)-1);
				q = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getPrevious(this->dcel->getTwin(edgeIndex)-1)-1)-1);

				// Set p as the vertex with highest y-coordinate.
				if (p->getY() < q->getY())
				{
					p = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getPrevious(this->dcel->getTwin(edgeIndex)-1)-1)-1);
					q = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getPrevious(edgeIndex)-1)-1);
				}

				// Origin vertex is negative.
				if (this->dcel->getOrigin(edgeIndex) < 0)
				{
					// Get destination point of current edge.
					common1 = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getTwin(edgeIndex)-1)-1);

					// Check if negative vertex is P-2.
					if (this->dcel->getOrigin(edgeIndex) == P_MINUS_2)
					{
						// If turn LEFT_TURN then flip edge.
						if (p->check_Turn(*q, *common1) == LEFT_TURN)
						{
							flipEdges = true;
						}
					}
					else
					{
						// If turn RIGHT then flip edge.
						if (p->check_Turn(*q, *common1) == RIGHT_TURN)
						{
							flipEdges = true;
						}
					}
				}
				// Destination vertex is negative.
				else
				{
					// Get origin point of current edge.
					common1 = this->dcel->getRefPoint(this->dcel->getOrigin(edgeIndex)-1);

					// Check if negative vertex is P-2.
					if (this->dcel->getOrigin(this->dcel->getTwin(edgeIndex)-1) == P_MINUS_2)
					{
						// If turn LEFT_TURN then flip edge.
						if (p->check_Turn(*q, *common1) == LEFT_TURN)
						{
							flipEdges = true;
						}
					}
					else
					{
						// If turn RIGHT then flip edge.
						if (p->check_Turn(*q, *common1) == RIGHT_TURN)
						{
							flipEdges = true;
						}
					}
				}
			}
		}
		// Vertex of candidate edge are positive.
		else
		{
			// Check if any of the other points of the triangles are P-2 or P-1.
			if ((this->dcel->getOrigin(this->dcel->getPrevious(edgeIndex)-1) > 0) &&
				(this->dcel->getOrigin(this->dcel->getPrevious(this->dcel->getTwin(edgeIndex)-1)-1) > 0))
			{
				// All points are positive -> normal in circle check.
				// Get points of edge to flip.
				common1 = this->dcel->getRefPoint(this->dcel->getOrigin(edgeIndex)-1);
				common2 = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getNext(edgeIndex)-1)-1);

				// Get points of faces.
				p = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getPrevious(edgeIndex)-1)-1);
				q = this->dcel->getRefPoint(this->dcel->getOrigin(this->dcel->getPrevious(this->dcel->getTwin(edgeIndex)-1)-1)-1);

				// Check if q falls into circle defined by common1-common2-p.
				if (Point<TYPE>::inCircle(common1, common2, p, q))
				{
					flipEdges = true;
				}
			}
		}
	}
#ifdef DEBUG_CHECK_EDGES
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Edge: ");
		Logging::buildText(__FUNCTION__, __FILE__, edge_ID);
		Logging::buildText(__FUNCTION__, __FILE__, " in external face.");
		Logging::write(true, Info);
	}
#endif

	// Check if edges must be flipped.
	if (flipEdges)
	{
#ifdef DELAUNAY_STATISTICS
		// Update triangle where point is located.
		delaunay_Stat.nFlipped++;
#endif
#ifdef DEBUG_CHECK_EDGES
		Logging::buildText(__FUNCTION__, __FILE__, "Edge: ");
		Logging::buildText(__FUNCTION__, __FILE__, edge_ID);
		Logging::buildText(__FUNCTION__, __FILE__, " must be flipped. Its data and its twin is:\n");
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefEdge(edge_ID-1)->toStr());
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefEdge(this->dcel->getNext(edge_ID-1)-1)->toStr());
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefEdge(this->dcel->getPrevious(edge_ID-1)-1)->toStr());

		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefEdge(this->dcel->getTwin(edge_ID-1)-1)->toStr());
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefEdge(this->dcel->getNext(this->dcel->getTwin(edge_ID-1)-1)-1)->toStr());
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefEdge(this->dcel->getPrevious(this->dcel->getTwin(edge_ID-1)-1)-1)->toStr());
		Logging::write(true, Info);
#endif
		// Flip edges.
		this->flipEdges(edge_ID);
	}
#ifdef DEBUG_CHECK_EDGES
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Edge: ");
		Logging::buildText(__FUNCTION__, __FILE__, edge_ID);
		Logging::buildText(__FUNCTION__, __FILE__, " OK.");
		Logging::write(true, Info);
	}
#endif
}


void Delaunay::flipEdges(int edge_ID)
{
	int	 temp=0;									// Temp variable.
	int	 edge_Index=0;								// Edge index.
	Node *node, newNode;
	int	 old_Node_ID1=0, old_Node_ID2=0;			// Old nodes id.
	Edge *twin, *edge;

	// Get edge index.
	edge_Index = edge_ID-1;

	// Get edge and twin edge information.
	edge = this->dcel->getRefEdge(edge_Index);
	twin = this->dcel->getRefEdge(this->dcel->getTwin(edge_Index)-1);

#ifdef DEBUG_FLIP_EDGES
	Logging::buildText(__FUNCTION__, __FILE__, "Edges to flip: ");
	Logging::buildText(__FUNCTION__, __FILE__, edge_Index+1);
	Logging::buildText(__FUNCTION__, __FILE__, " and its twin ");
	Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getTwin(edge_Index));
	Logging::write(true, Info);
	if ((edge->getFace() > this->dcel->getNFaces()) || (twin->getFace() > this->dcel->getNFaces()))
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Faces where edges are flipped out of range: ");
		Logging::buildText(__FUNCTION__, __FILE__, edge->getFace());
		Logging::buildText(__FUNCTION__, __FILE__, " or ");
		Logging::buildText(__FUNCTION__, __FILE__, twin->getFace());
		Logging::buildRange(__FUNCTION__, __FILE__, 0, this->dcel->getNFaces());
		Logging::write(true, Info);
		// PENDING WHAT TO DO.
		exit(0);
	}
#endif

	// Store nodes ID that are going to be updated to internal nodes.
	old_Node_ID1 = this->graph->getNodeAssigned(edge->getFace());
	old_Node_ID2 = this->graph->getNodeAssigned(twin->getFace());

#ifdef DEBUG_FLIP_EDGES
	Logging::buildText(__FUNCTION__, __FILE__, "Faces where edges are located: ");
	Logging::buildText(__FUNCTION__, __FILE__, edge->getFace());
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, twin->getFace());
	Logging::write(true, Info);
	Logging::buildText(__FUNCTION__, __FILE__, "Nodes where edges are located: ");
	Logging::buildText(__FUNCTION__, __FILE__, old_Node_ID1);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, old_Node_ID2);
	Logging::buildText(__FUNCTION__, __FILE__, "\nNodes data\n");
	Logging::buildText(__FUNCTION__, __FILE__, this->graph->getRefNode(old_Node_ID1)->toStr());
	Logging::buildText(__FUNCTION__, __FILE__, this->graph->getRefNode(old_Node_ID2)->toStr());
	Logging::write(true, Info);

	if ((old_Node_ID1 <= 0) || (old_Node_ID1 > this->graph->getNElements()) ||
		(old_Node_ID2 <= 0) || (old_Node_ID2 > this->graph->getNElements()))
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Node where edges are flipped out of range: ");
		Logging::buildText(__FUNCTION__, __FILE__, old_Node_ID1);
		Logging::buildText(__FUNCTION__, __FILE__, " or ");
		Logging::buildText(__FUNCTION__, __FILE__, old_Node_ID2);
		Logging::buildRange(__FUNCTION__, __FILE__, 0, this->graph->getNElements());
		Logging::write(true, Info);
		exit(0);
	}
#endif

	// Update vertex of flipped edge.
	if (edge->getOrigin() > 0)
	{
		this->dcel->updateVertex(twin->getNext(), edge->getOrigin()-1);
	}
	if (this->dcel->getOrigin(edge->getNext()-1) > 0)
	{
		this->dcel->updateVertex(edge->getNext(), this->dcel->getOrigin(edge->getNext()-1)-1);
	}

	// Update origin of current and twin edges.
	// PENDING this can be implemented by a swap.
	temp = this->dcel->getOrigin(edge->getPrevious()-1);
	this->dcel->setOrigin(edge_Index, this->dcel->getOrigin(twin->getPrevious()-1));
	this->dcel->setOrigin(edge->getTwin()-1, temp);

	// Update next edges.
	this->dcel->setNext(edge->getNext()-1, edge->getTwin());
	this->dcel->setNext(twin->getNext()-1, edge_ID);
	this->dcel->setNext(edge->getPrevious()-1, twin->getNext());
	this->dcel->setNext(twin->getPrevious()-1, edge->getNext());
	this->dcel->setNext(edge_Index, edge->getPrevious());
	this->dcel->setNext(edge->getTwin()-1, twin->getPrevious());

	// Update previous edges.
	this->dcel->setPrevious(edge_Index, this->dcel->getNext(this->dcel->getNext(edge_Index)-1));
	this->dcel->setPrevious(edge->getTwin()-1, this->dcel->getNext(twin->getNext()-1));
	this->dcel->setPrevious(edge->getNext()-1, edge_ID);
	this->dcel->setPrevious(twin->getNext()-1, edge->getTwin());
	this->dcel->setPrevious(edge->getPrevious()-1, edge->getNext());
	this->dcel->setPrevious(twin->getPrevious()-1, twin->getNext());

	// Update faces of edges that have moved to another face.
	this->dcel->setFace(edge->getPrevious()-1, edge->getFace());
	this->dcel->setFace(twin->getPrevious()-1, twin->getFace());

	// Update faces.
	this->dcel->setFaceEdge(edge->getFace(), edge_ID);
	this->dcel->setFaceEdge(twin->getFace(), edge->getTwin());

	// Get node of current edge and update it.
	node = this->graph->getRefNode(old_Node_ID1);
	node->setChildren(this->graph->getNElements(), this->graph->getNElements()+1);
	this->graph->update(old_Node_ID1, 2, node);

	// Get node of twin edge and update it.
	node = this->graph->getRefNode(old_Node_ID2);
	node->setChildren(this->graph->getNElements(), this->graph->getNElements()+1);
    this->graph->update(old_Node_ID2, 2, node);

    // Insert two new nodes.
    newNode = Node(this->dcel->getOrigin(edge->getPrevious()-1),
    				this->dcel->getOrigin(edge_Index),
					this->dcel->getOrigin(edge->getNext()-1),
					edge->getFace());
    this->graph->insert(newNode);

    newNode = Node(this->dcel->getOrigin(twin->getPrevious()-1),
    				this->dcel->getOrigin(edge->getTwin()-1),
					this->dcel->getOrigin(twin->getNext()-1),
					twin->getFace());
    this->graph->insert(newNode);

	// Check recursively edges that could be illegal.
	this->checkEdge(edge->getPrevious());
	this->checkEdge(twin->getNext());
}

/***************************************************************************
* Name: 	convexHull
* IN:		NONE
* OUT:		NONE
* RETURN:	true if the convex hull has been computed.
* 			false otherwise.
* GLOBAL:	the convex hull is stored in the "convexHull" attribute.
* Description: 	computes the convex hull of the DCEL set of points.
***************************************************************************/
bool Delaunay::convexHull()
{
	bool finished=false;		// Loop control flag.
	int	edgeIndex=0;			// Edge index.
	int firstIndex;				// First edge index of he convex hull.

	try
	{
		// Initialize return value.
		this->setConvexHullComputed(false);
#ifdef DEBUG_GET_CONVEX_HULL
		Logging::buildText(__FUNCTION__, __FILE__, "Inserting point ");
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getOrigin(0));
		Logging::write(true, Info);
		Logging::buildText(__FUNCTION__, __FILE__, "Point coordinates are: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefPoint(this->dcel->getOrigin(0)-1));
		Logging::write(true, Info);
#endif
		// Insert initial point (always in the convex hull).
		this->hull.add(this->dcel->getRefPoint(this->dcel->getOrigin(0)-1));

		// Get an edge departing from 0 point.
		edgeIndex = this->dcel->getPointEdge(0) - 1;

#ifdef DEBUG_GET_CONVEX_HULL
		Logging::buildText(__FUNCTION__, __FILE__, "First edge in: ");
		Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
		Logging::write(true, Info);
#endif
		// Get edge departing from 0 to MINUS_2.
		finished = false;
		while (!finished)
		{
			if ((this->dcel->getOrigin(edgeIndex) == 1) &&
				(this->dcel->getOrigin(this->dcel->getTwin(edgeIndex)-1) == P_MINUS_2))
			{
				finished = true;
#ifdef DEBUG_GET_CONVEX_HULL
				Logging::buildText(__FUNCTION__, __FILE__, "Found first edge index ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex);
				Logging::buildText(__FUNCTION__, __FILE__, "Origin point is ");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getOrigin(edgeIndex));
				Logging::buildText(__FUNCTION__, __FILE__, ".Destination point is ");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getOrigin(this->dcel->getTwin(edgeIndex)-1));
				Logging::write(true, Info);
#endif
			}
			else
			{
				// Get next edge departing from 0 point.
				edgeIndex = this->dcel->getTwin(this->dcel->getPrevious(edgeIndex) - 1) - 1;
			}
		}

		// Get previous edge as it is the first edge in the convex hull.
		edgeIndex = this->dcel->getPrevious(edgeIndex) - 1;
		firstIndex = edgeIndex;

		// Get all convex hull points.
		finished = false;
		while (!finished)
		{
			// Insert next point.
			this->hull.add(this->dcel->getRefPoint(this->dcel->getOrigin(edgeIndex)-1));
			this->hullEdges.add(edgeIndex+1);
#ifdef DEBUG_GET_CONVEX_HULL
			Logging::buildText(__FUNCTION__, __FILE__, "Added point ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getOrigin(edgeIndex));
			Logging::buildText(__FUNCTION__, __FILE__, ". Point coordinates: ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefPoint(this->dcel->getOrigin(edgeIndex)-1)->toStr());
			Logging::write(true, Info);
#endif

			// Get next edge.
			edgeIndex = this->dcel->getPrevious(edgeIndex)-1;
			edgeIndex = this->dcel->getTwin(edgeIndex)-1;
			edgeIndex = this->dcel->getPrevious(edgeIndex)-1;

			// If point is imaginary then skip edge.
			if (this->dcel->getOrigin(edgeIndex) < 0)
			{
				edgeIndex = this->dcel->getTwin(edgeIndex)-1;
				edgeIndex = this->dcel->getPrevious(edgeIndex)-1;
			}

			// Check if first edge reached.
			if (edgeIndex == firstIndex)
			{
				finished = true;
				this->setConvexHullComputed(true);
#ifdef DEBUG_GET_CONVEX_HULL
				Logging::buildText(__FUNCTION__, __FILE__, "Convex hull computed.");
				Logging::write(true, Info);
#endif
			}
		}
	}
	catch (exception &ex)
	{
		ex.what();
	}

	return(this->isConvexHullComputed());
}

/***************************************************************************
* Name: 	print
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	print dcel and graph data.
***************************************************************************/
void Delaunay::print()
{
	// Print DCEL and graph data.
	if (this->dcel != NULL)
	{
		this->dcel->print(std::cout);

		// If graph allocated print graph data.
		if (this->isGraphAllocated())
		{
			this->graph->print(std::cout);
		}
	}
}


/***************************************************************************
* Name: 	print
* IN:		node		pointer to node to print
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	print the node vertices and edges.
***************************************************************************/
void Delaunay::print(Node *node)
{
	int	edgeID=0;		// Edge identifier.
	int	faceID=0;		// Face identifier.
	Edge *edge;			// Pointer to edge.
	Face *face;			// Pointer to node face.

	// Get face identifier.
	faceID = node->getFace();
	face = dcel->getRefFace(faceID);

	// Print current edge and its twin.
	edgeID = face->getEdge();
	edge = this->dcel->getRefEdge(edgeID-1);
	std::cout << "Current edge:\t\t";
	edge->print(std::cout);
	edge = this->dcel->getRefEdge(this->dcel->getTwin(edgeID-1)-1);
	std::cout << "\t\t\t";
	edge->print(std::cout);

	// Print next edge and its twin.
	edge = this->dcel->getRefEdge(this->dcel->getNext(edgeID-1)-1);
	std::cout << "Next edge:\t\t";
	edge->print(std::cout);
	edge = this->dcel->getRefEdge(this->dcel->getTwin(this->dcel->getNext(edgeID-1)-1)-1);
	std::cout << "\t\t\t";
	edge->print(std::cout);

	// Print previous edge and its twin.
	edge = this->dcel->getRefEdge(this->dcel->getPrevious(edgeID-1)-1);
	std::cout << "Previous edge:\t\t";
	edge->print(std::cout);
	edge = this->dcel->getRefEdge(this->dcel->getTwin(this->dcel->getPrevious(edgeID-1)-1)-1);
	std::cout << "\t\t\t";
	edge->print(std::cout);
}

/***************************************************************************
* Name: 	read
* IN:		fileName		file name to read from.
* 			graphFileName	incremental graph filename.
* OUT:		NONE
* RETURN:	true			if read
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	reads the DCEL and the graph data from two files, first
* 				named "fileName" and the graph from the "filaNameGraph.txt".
***************************************************************************/
bool Delaunay::read(string fileName, string graphFileName)
{
	bool	read=true;		// Return value.

	// Read DCEL data.
	read = this->dcel->read(fileName, false);
	if (read)
	{
		// Initialize graph.
		this->initializeGraph();

		// Read graph data.
		read = this->graph->read(graphFileName);
		if (read)
		{
			this->setGraphAllocated(true);
			this->setConvexHullComputed(false);
		}
	}

	return(read);
}

/***************************************************************************
* Name: 	write
* IN:		fileName	file name to write to.
* 			graphFileName	incremental graph filename.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	writes the DCEL and the graph data to two files, first
* 				named "fileName" and the graph to the "filaNameGraph.txt".
***************************************************************************/
bool Delaunay::write(string fileName, string graphFileName)
{
	bool	success=true;		// Return value.

	// Write DCEL data.
	success = this->dcel->write(fileName, false);
	if (success)
	{
		// Write graph data if graph exists.
		if (this->graph->getNElements() > 0)
		{
			success = this->graph->write(graphFileName);
		}
	}

	return(success);
}



int  selectClosest(int index)
{
	int	ret=SUCCESS;			// Return value.

	return(ret);
}

/***************************************************************************
* Name: 	findTwoClosest
* IN:		NONE
* OUT:		first		index of one of the closest points.
* 			second		index of the other of the closest points.
* RETURN:	true		if two closest points found.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	Finds the two closest point in the DCEL. It is only necessary
* 				to check the neighbors of every point because there are no
* 				closer points to a given point than its neighbors.
***************************************************************************/
bool Delaunay::findTwoClosest(int &first, int &second)
{
	bool found=false;				// Return value.
	int	 pointIndex=0;				// Loop counter.
	int  firstEdgeId=0;				// First edge for current point.
	int	 currentEdgeId=0;			// Current edge id.
	int	 currentEdgeIndex=0;		// Current edge index.
	double	distance=0.0;			// Current distance.
	double	lowestDistance=DBL_MAX;	// Current distance.
	Point<TYPE>	*origin;			// First point.
	Point<TYPE>	*dest;				// Second point.
	int	destination=0;				// Destination point id.
	int lastPointIndex=0;			// Last point must no be checked.

	// Check all vertex.
	lastPointIndex = this->dcel->getNVertex()-1;
	for (pointIndex=0; pointIndex<lastPointIndex; pointIndex++)
	{
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
		Logging::buildText(__FUNCTION__, __FILE__, "Checking point ");
		Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
		Logging::write(true, Info);
#endif
		// Get edge departing from current point.
		currentEdgeId = this->dcel->getPointEdge(pointIndex);
		firstEdgeId = currentEdgeId;
		currentEdgeIndex = currentEdgeId - 1;

		// Get reference to origin point.
		origin = this->dcel->getRefPoint(pointIndex);

		do
		{
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
			Logging::buildText(__FUNCTION__, __FILE__, "Checking edge ");
			Logging::buildText(__FUNCTION__, __FILE__, currentEdgeId);
			Logging::write(true, Info);
#endif
			// Skip no-real edges.
			if (!this->dcel->hasNegativeVertex(currentEdgeId))
			{
				// Get reference to destination point.
				destination = this->dcel->getOrigin(this->dcel->getTwin(currentEdgeIndex)-1);
				dest = this->dcel->getRefPoint(destination-1);
				if ((pointIndex+1) < destination)
				{
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
					Logging::buildText(__FUNCTION__, __FILE__, "Computing distance between ");
					Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
					Logging::buildText(__FUNCTION__, __FILE__, " and ");
					Logging::buildText(__FUNCTION__, __FILE__, destination);
					Logging::write(true, Info);
#endif
					// Compute distance.
					distance = origin->distance(*dest);
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
					Logging::buildText(__FUNCTION__, __FILE__, "New distance is ");
					Logging::buildText(__FUNCTION__, __FILE__, distance);
					Logging::write(true, Info);
#endif
					// Compare to current.
					if (distance < lowestDistance)
					{
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
						Logging::buildText(__FUNCTION__, __FILE__, "New distance is lower than ");
						Logging::buildText(__FUNCTION__, __FILE__, lowestDistance);
						Logging::write(true, Info);
#endif
						// Update lowest distance and output indexes.
						lowestDistance = distance;
						first = pointIndex;
						second = destination-1;
						found = true;
					}
				}
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
				else
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Edge already checked. Destination ");
					Logging::buildText(__FUNCTION__, __FILE__, destination);
					Logging::buildText(__FUNCTION__, __FILE__, " is lower than ");
					Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
					Logging::write(true, Info);
				}
#endif
			}
#ifdef DEBUG_DELAUNAY_FINDTWOCLOSEST
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Edge is not real");
				Logging::write(true, Info);
			}
#endif
			// Get next edge departing from edge.
			currentEdgeId = this->dcel->getTwin(this->dcel->getPrevious(currentEdgeIndex)-1);
			currentEdgeIndex = currentEdgeId - 1;
		} while (currentEdgeId != firstEdgeId);
	}

	return(found);
}


/***************************************************************************
* Name: 	findClosestPoint
* IN:		p			input point whose closest is going to be located.
* 			voronoi		Voronoi areas data.
* OUT:		q			closest point to p
* 			pointIndex 	index of the point in the DCEL.
* 			distance 	p-q distance.
* RETURN:	true		if closes point found.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	finds the closest point q to input point p. To do so it first
* 				locates the face that surrounds p and then
***************************************************************************/
bool Delaunay::findClosestPoint(Point<TYPE> &p, Voronoi &voronoi,
															Point<TYPE> &q,
															int	&pointIndex,
															double &dist)
{
	bool found=false;			// Return value.
	int	 nodeIndex=0;			// Node index.
	int	 firstPointIndex=0;		// First point in loop.
	int	 currentPointIndex=0;	// Current point in loop.
	int	 currentEdgeIndex=0;	// Current edge in loop.
	int  i=0;					// Loop counter.
	Queue<int> queue(NPOINTS_TRIANGLE*2);
	bool *insertedPoints;		// Array of previously point checked.

#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
	Logging::buildText(__FUNCTION__, __FILE__, "Searching point ");
	Logging::buildText(__FUNCTION__, __FILE__, &p);
	Logging::write(true, Info);
#endif

	// Initialize variables.
	pointIndex = 0;
	insertedPoints = new bool[this->dcel->getNVertex()];
	memset(insertedPoints, 0, sizeof(bool)*this->dcel->getNVertex());

	// Get node index of the face that surrounds point.
	found = this->locateNode(p, nodeIndex);
	if (found)
	{
		// Insert points from node.
		for (i=0; i<NPOINTS_TRIANGLE ;i++)
		{
			pointIndex = this->graph->getRefNode(nodeIndex)->getiPoint(i)-1;
			if (pointIndex >= 0)
			{
				// Insert current point and set as inserted.
				insertedPoints[pointIndex] = true;
				queue.enqueue(pointIndex);
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
				Logging::buildText(__FUNCTION__, __FILE__, "Inserted initial point ");
				Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
				Logging::write(true, Info);
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Skipped initial point ");
				Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
				Logging::write(true, Info);
#endif
			}
		}

		// Set loop condition.
		found = false;

		// Check point in queue until closest is found.
		do
		{
			// Get next point.
			pointIndex = queue.dequeue();
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
			Logging::buildText(__FUNCTION__, __FILE__, "Checking point ");
			Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
			Logging::write(true, Info);
#endif
			// Check if point inner to current Voronoi area.
			if (voronoi.isInnerToArea(p, pointIndex+1))
			{
				found = true;
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
				Logging::buildText(__FUNCTION__, __FILE__, "Is interior.");
				Logging::write(true, Info);
#endif
			}
			else
			{
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
				Logging::buildText(__FUNCTION__, __FILE__, "Is exterior.");
				Logging::write(true, Info);
#endif
				// Get edge departing from current point.
				currentEdgeIndex = this->dcel->getPointEdge(pointIndex)-1;
				currentEdgeIndex = this->dcel->getPrevious(currentEdgeIndex)-1;
				currentPointIndex = this->dcel->getOrigin(currentEdgeIndex)-1;

				// Set end of loop condition.
				firstPointIndex = currentPointIndex;

				do
				{
					// Insert only if not already checked.
					if (currentPointIndex >= 0)
					{
						if (!insertedPoints[currentPointIndex])
						{
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
							Logging::buildText(__FUNCTION__, __FILE__, "Inserting point ");
							Logging::buildText(__FUNCTION__, __FILE__, currentPointIndex+1);
							Logging::write(true, Info);
#endif
							// Insert current point and set as inserted.
							insertedPoints[currentPointIndex] = true;
							queue.enqueue(currentPointIndex);
						}
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
						else
						{
							Logging::buildText(__FUNCTION__, __FILE__, "Already checked point ");
							Logging::buildText(__FUNCTION__, __FILE__, currentPointIndex+1);
							Logging::write(true, Info);
						}
#endif
					}
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT
					else
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Current point is not real ");
						Logging::buildText(__FUNCTION__, __FILE__, currentPointIndex+1);
						Logging::write(true, Info);
					}
#endif
					// Get next edge.
					currentEdgeIndex = this->dcel->getTwin(currentEdgeIndex)-1;
					currentEdgeIndex = this->dcel->getPrevious(currentEdgeIndex)-1;
					currentPointIndex = this->dcel->getOrigin(currentEdgeIndex)-1;
				} while (currentPointIndex != firstPointIndex);
			}
		} while ((!found) && (queue.getNElements() > 0));

		if (found)
		{
			// Update output data.
			q = *this->dcel->getRefPoint(pointIndex);
			dist = p.distance(q);
		}
	}

	// Deallocate memory.
	delete[] insertedPoints;

	return(found);
}


// PENDING
bool Delaunay::findClosestPoint(Point<TYPE> &p, int nAnchors, Point<TYPE> &q, double &distance)
{
	bool 	found=false;		// Return value.
	int		pointIndex=0;		// Loop counter.
	int		id=0;				// Point identifier.
	Point<TYPE> currentPoint;	// Current point.
	TYPE 	dist=0.0;			// New distance.

	// Create seed.
	srand48((int) time(NULL));

	// Set current distance.
	distance = FLT_MAX;

	// Select closest anchor.
	for (pointIndex=0; pointIndex<nAnchors ;pointIndex++)
	{
		// Generate a random point.
		id = rand() % this->getDCEL()->getNVertex();
		currentPoint = *this->getDCEL()->getRefPoint(id);

#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT_ANCHORS
		Logging::buildText(__FUNCTION__, __FILE__, "Checking distance form point id ");
		Logging::buildText(__FUNCTION__, __FILE__, id);
		Logging::write(true, Info);
#endif

		// Compute distance to current anchor.
		dist = p.distance(currentPoint);
		if(dist < distance)
		{
			// Update output data.
			q = currentPoint;
			distance = dist;
#ifdef DEBUG_DELAUNAY_FIND_CLOSESTPOINT_ANCHORS
			Logging::buildText(__FUNCTION__, __FILE__, "Lower distance ");
			Logging::buildText(__FUNCTION__, __FILE__, distance);
			Logging::write(true, Info);
#endif
		}
	}

	// Update return value.
	found = true;

	return(found);
}

/***************************************************************************
* Name: 	findPath
* IN:		line		line that determines the path.
* OUT:		facesPath	list of faces in the path.
* RETURN:	true		if path found.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	determines the set of faces where the input line lays on.
***************************************************************************/
bool Delaunay::findPath(Line &line, Set<int> &facesPath)
{
	bool found=false;				// Return value.
	bool computePath=false;			// Both points in external face.
	int	 faceId=0;					// Initial face in the path.
	int	 finalFace=0;				// Final face in the path.
	int	 i=0;						// Loop counter.
	int	 nFacesToAdd=0;				// Loop upper bound.
	int	 edgeIndex=0;				// Edge index.
	Set<int> intersectEdges(2);		// Set of edges that intersect convex hull.
	Point<TYPE> origin, dest;		// Line extreme points.
	Set<int> extremeFaces(2);		// First and last faces in the path.

	// Get origin and destination points.
	origin = line.getOrigin();
	dest = line.getDest();

	// Get extreme point faces.
	if (this->findFace(origin, faceId) && this->findFace(dest, finalFace))
	{
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
		Logging::buildText(__FUNCTION__, __FILE__, "Faces are ");
		Logging::buildText(__FUNCTION__, __FILE__, faceId);
		Logging::buildText(__FUNCTION__, __FILE__, " and ");
		Logging::buildText(__FUNCTION__, __FILE__, finalFace);
		Logging::write(true, Info);
#endif
		// Add non external faces to set.
		if (!this->dcel->imaginaryFace(faceId))
		{
			extremeFaces.add(faceId);
			computePath = true;
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
			Logging::buildText(__FUNCTION__, __FILE__, "Initial face is real.");
			Logging::write(true, Info);
#endif
		}
		if (!this->dcel->imaginaryFace(finalFace))
		{
			extremeFaces.add(finalFace);
			computePath = true;
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
			Logging::buildText(__FUNCTION__, __FILE__, "Final face is real.");
			Logging::write(true, Info);
#endif
		}

		// Check if any of the faces is external to convex hull.
		if (extremeFaces.getNElements() != 2)
		{
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
			Logging::buildText(__FUNCTION__, __FILE__, "At least one of the faces is imaginary. Faces ids are ");
			Logging::buildText(__FUNCTION__, __FILE__, faceId);
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, finalFace);
			Logging::write(true, Info);
#endif
			// Check if convex hull has not been computed.
			if (!this->isConvexHullComputed())
			{
				this->convexHull();
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
				Logging::buildText(__FUNCTION__, __FILE__, "Computing convex hull.");
				Logging::write(true, Info);
#endif
			}

			// Line intersects convex hull.
			if (this->getConvexHull()->getIntersections(line, intersectEdges))
			{
				computePath = true;
				//this->getInitialFaces(line, intersectEdges, initialFace, finalFace);
				nFacesToAdd = intersectEdges.getNElements();
				for (i=0; i<nFacesToAdd ;i++)
				{
					edgeIndex = (*this->getConvexHullEdges()->at(*intersectEdges.at(i)))-1;
					faceId = this->dcel->getFace(this->dcel->getTwin(edgeIndex)-1);
					extremeFaces.add(faceId);
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
					Logging::buildText(__FUNCTION__, __FILE__, "Changing external face to ");
					Logging::buildText(__FUNCTION__, __FILE__, faceId);
					Logging::buildText(__FUNCTION__, __FILE__, " using edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write(true, Info);
#endif
				}
			}
			else
			{
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
				Logging::buildText(__FUNCTION__, __FILE__, "Convex hull is not intersected.");
				Logging::write(true, Info);
#endif
				found = true;
			}
		}

		// If both points are external -> no path to compute.
		if (computePath)
		{
			// Find path.
			found = this->getDCEL()->findPath(extremeFaces, line, facesPath);
		}
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Both points are external.");
			Logging::write(true, Info);
		}
#endif
	}
#ifdef DEBUG_DELAUNAY_FIND_TRIANG_PATH
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Points faces not found.");
		Logging::write(true, Error);
	}
#endif

	return(found);
}

/***************************************************************************
* Name: 	getInitialFaces
* IN:		line			line whose extreme points must be located
* 			edgesSet		set of edges that intersect convex hull.
* OUT:		NONE
* IN/OUT:	initialFace		first face into the DCEL convex hull
* 			finalFace		last face into the DCEL convex hull
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	at least one of the line extreme points is not into the
* 				convex hull. PENDING
***************************************************************************/
void Delaunay::getInitialFaces(Line &line, Set<int> &edgesSet, int &initialFace, int &finalFace)
{
	int			i=0;			// Loop counter.
	Point<TYPE>	point;			// Line extreme point.
	Set<int> 	edgesIndex;		// Set of edges index.

	//PENDING REMOVE this->dcel->print(std::cout);
	// Get real edges ID in DCEL.
	for (i=0; i<edgesSet.getNElements() ;i++)
	{
		edgesIndex.add((*this->getConvexHullEdges()->at(*edgesSet.at(i))) - 1);
#ifdef DEBUG_DELAUNAY_GETINITIALFACES
		Logging::buildText(__FUNCTION__, __FILE__, "Intersected edge index ");
		Logging::buildText(__FUNCTION__, __FILE__, *edgesIndex.at(i));
		Logging::write(true, Info);
#endif
	}

#ifdef DEBUG_DELAUNAY_GETINITIALFACES
	Logging::buildText(__FUNCTION__, __FILE__, "Initial faces are ");
	Logging::buildText(__FUNCTION__, __FILE__, initialFace);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, finalFace);
	Logging::write(true, Info);
#endif

	if (this->dcel->isBottomMostFace(initialFace))
	{
		if (this->dcel->getFace(*edgesIndex.at(0)) == finalFace)
		{
			initialFace = this->dcel->getFace(*edgesIndex.at(1));
		}
		else
		{
			initialFace = this->dcel->getFace(*edgesIndex.at(0));
		}
	}
	else if (this->dcel->isBottomMostFace(finalFace))
	{
		if (this->dcel->getFace(*edgesIndex.at(0)) == initialFace)
		{
			finalFace = this->dcel->getFace(*edgesIndex.at(1));
		}
		else
		{
			finalFace = this->dcel->getFace(*edgesIndex.at(0));
		}
	}
#ifdef DEBUG_DELAUNAY_GETINITIALFACES
	Logging::buildText(__FUNCTION__, __FILE__, "Corrected faces are ");
	Logging::buildText(__FUNCTION__, __FILE__, initialFace);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, finalFace);
	Logging::write(true, Info);
#endif

	// Check if origin extreme point is internal to the convex hull.
	point = line.getOrigin();
	if (!this->getConvexHull()->isInternal(point))
	{
#ifdef DEBUG_DELAUNAY_GETINITIALFACES
		Logging::buildText(__FUNCTION__, __FILE__, "Origin is external to convex hull");
		Logging::write(true, Info);
#endif
		this->getInternalFace(line, edgesIndex, initialFace);
	}

	// Check if origin extreme point is internal to the convex hull.
	point = line.getDest();
	if (!this->getConvexHull()->isInternal(point))
	{
#ifdef DEBUG_DELAUNAY_GETINITIALFACES
		Logging::buildText(__FUNCTION__, __FILE__, "Destination is external to convex hull");
		Logging::write(true, Info);
#endif
		this->getInternalFace(line, edgesIndex, finalFace);
	}
}


/***************************************************************************
* Name: 	getInternalFace
* IN:		edgesIndex		set of edges to check
* OUT:		NONE
* IN/OUT:	face			face to be updated
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	get the face that is in the convex hull that is a twin face
* 				of the external input "face".
***************************************************************************/
void Delaunay::getInternalFace(Line &line, Set<int> &edgesIndex, int &face)
{
	int		i=0;				// Loop counter.
	bool	found=false;		// Loop control flag.
	int		edgeIndex=0;		// Edge index.

	// Initialize loop variables.
	i=0;
	found = false;

	// Loop until all edges checked or face found.
	while ((i < edgesIndex.getNElements()) && (!found))
	{
		// Get next edge.
		edgeIndex = *edgesIndex.at(i);
#ifdef DEBUG_DELAUNAY_GET_INTERNAL_FACE
		Logging::buildText(__FUNCTION__, __FILE__, "Checking edge ");
		Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
		Logging::write(true, Info);
#endif

		// Check if edge face is equal to input face.
		if (this->dcel->getFace(edgeIndex) == face)
		{
			// Twin face is the internal face searched.
			found = true;
			face = this->dcel->getFace(this->dcel->getTwin(edgeIndex)-1);
#ifdef DEBUG_DELAUNAY_GET_INTERNAL_FACE
			Logging::buildText(__FUNCTION__, __FILE__, "Edge found and face is ");
			Logging::buildText(__FUNCTION__, __FILE__, face);
			Logging::write(true, Info);
#endif
		}
		i++;
	}
}

/***************************************************************************
* Name: 	findFace
* IN:		index		index of the point whose face must be located.
* OUT:		faceId		face id of the face that surrounds the point.
* RETURN:	true		if face found.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	allocates the graph to be used in incremental Delaunay.
***************************************************************************/
bool Delaunay::findFace(Point<TYPE> &point, int &faceId)
{
	bool 	found=false;		// Return value.
	int		nodeIndex=0;		// Index of the node assigned to face.

	// Locate node.
	found = this->locateNode(point, nodeIndex);
	if (found)
	{
		// Get face in node.
		faceId = this->graph->getRefNode(nodeIndex)->getFace();
		if (this->dcel->imaginaryFace(faceId))
		{
#ifdef DEBUG_DELAUNAY_FINDFACE
			Logging::buildText(__FUNCTION__, __FILE__, "Face found is ");
			Logging::buildText(__FUNCTION__, __FILE__, faceId);
			Logging::buildText(__FUNCTION__, __FILE__, " that is imaginary. Return 0.");
			Logging::write(true, Info);
#endif
			faceId = EXTERNAL_FACE;
		}
#ifdef DEBUG_DELAUNAY_FINDFACE
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Face found is ");
			Logging::buildText(__FUNCTION__, __FILE__, faceId);
			Logging::write(true, Info);
		}
#endif
	}

	return(found);
}

//------------------------------------------------------------------------
// Private functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	initializeGraph
* IN:		node		node whose area must be computed
* OUT:		NONE
* RETURN:	true		if graph allocated.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	allocates the graph to be used in incremental Delaunay.
***************************************************************************/
bool Delaunay::initializeGraph()
{
	int nNodes=0;		// # nodes to create.

	// PENDING. How many nodes to allocate. Depends on faces? points? generation type?
	nNodes = MAX(this->dcel->getNFaces(), this->dcel->getNVertex());

	// Check if graph already allocated.
	if (!this->isGraphAllocated())
	{
		// Allocate graph.
		try
		{
			this->graph = new Graph(nNodes);
			this->setGraphAllocated(true);
#ifdef DEBUG_GRAPH_INITIALIZEGRAPH
			Logging::buildText(__FUNCTION__, __FILE__, "Graph first allocated with # nodes=");
			Logging::buildText(__FUNCTION__, __FILE__, nNodes);
			Logging::write(true, Info);
#endif
		}
		catch (std::bad_alloc& ba)
		{
			std::cerr << "bad_alloc caught: " << ba.what() << endl;
		}
	}
	else
	{
		// Check if new graph size is higher than current.
		if (this->graph->getSize() < nNodes)
		{
#ifdef DEBUG_GRAPH_INITIALIZEGRAPH
			Logging::buildText(__FUNCTION__, __FILE__, "New size is higher.");
			Logging::write(true, Info);
#endif
			if (!this->graph->resize(nNodes*2, false))
			{
#ifdef DEBUG_GRAPH_INITIALIZEGRAPH
				Logging::buildText(__FUNCTION__, __FILE__, "Error resizing graph.");
				Logging::write(true, Info);
#endif
				this->setGraphAllocated(false);
			}
		}
		else
		{
#ifdef DEBUG_GRAPH_INITIALIZEGRAPH
			Logging::buildText(__FUNCTION__, __FILE__, "Reseting graph.");
			Logging::write(true, Info);
#endif
			this->graph->reset();
		}
	}

	return(this->isGraphAllocated());
}

/***************************************************************************
* Name: 	insertPoint
* IN:		index		index of the point to locate.
* OUT:		NONE
* RETURN:	true		if point inserted.
* 			false 		i.o.c.
* GLOBAL:	NONE
* Description: 	Locates the node where the point whose index is the input
* 				parameter and inserts the point in the triangulation.
***************************************************************************/
bool Delaunay::insertPoint(int index)
{
	bool	inserted=false;		// Return value.
	int		nodeIndex=0;		// Current node index.
	Point<TYPE> *point=NULL;   	// Pointer to points in DCEL.

	// Get new point to insert.
	point = this->dcel->getRefPoint(index);

	// Gets node where index point is located.
    if (this->locateNode(*point, nodeIndex))
    {
#ifdef DEBUG_DELAUNAY_INSERTPOINT
		Logging::buildText(__FUNCTION__, __FILE__, "Node is a leaf. Point is interior to node ");
		Logging::buildText(__FUNCTION__, __FILE__, nodeIndex+1);
		Logging::write(true, Info);
#endif

        // Check if point is strictly interior (not over an edge).
        if (this->isStrictlyInteriorToNode(*point, nodeIndex))
        {
#ifdef DEBUG_DELAUNAY_INSERTPOINT
        	Logging::buildText(__FUNCTION__, __FILE__, "Point is strictly interior");
			Logging::write(true, Info);
#endif
            // Split current node creating 3 triangles.
            this->splitNode(index, nodeIndex, 3);
        }
        // Point over an edge.
        else
        {
#ifdef DEBUG_DELAUNAY_INSERTPOINT
        	Logging::buildText(__FUNCTION__, __FILE__, "Point is over an edge");
			Logging::write(true, Info);
#endif
            // Split current node creating 4 triangles.
            this->splitNode(index, nodeIndex, 4);
        }
#ifdef DEBUG_DELAUNAY_INSERTPOINT
		Logging::buildText(__FUNCTION__, __FILE__, "Point inserted");
		Logging::write(true, Info);
#endif
		inserted = true;
    }

	return(inserted);
}


/***************************************************************************
* Name: 	locateNode
* IN:		index		index of the point to locate.
* OUT:		nodeIndex	node where point should be inserted.
* RETURN:	true		if node found
* 			false 		i.o.c.
* GLOBAL:	NONE
* Description: 	Locates the node where the point whose index is the first
* 				input parameter is located.
***************************************************************************/
bool Delaunay::locateNode(Point<TYPE> &point, int &nodeIndex)
{
	bool	locatedNode=false;	// Return value.
	int     i=0;                // Loop counter.
	int		nChildren=0;		// # children in current node.
	bool	error=false; 		// Fatal error flag.

#ifdef DEBUG_DELAUNAY_LOCATENODE
	Logging::buildText(__FUNCTION__, __FILE__, "Searching point " );
	Logging::buildText(__FUNCTION__, __FILE__, &point);
	Logging::write(true, Info);
#endif

    // Loop until triangle found or error in process.
	nodeIndex = 0;
    while ((!this->graph->isLeaf(nodeIndex)) && (!error))
    {
       	// PENDING REMOVE ASSIGNED FACE WHEN NODE IS INTERIOR.
        // Search triangle in children nodes.
        i = 0;
        locatedNode = false;
        nChildren = this->graph->getNChildren(nodeIndex);
        while ((!locatedNode) && (i < nChildren))
        {
#ifdef DEBUG_DELAUNAY_LOCATENODE
        	Logging::buildText(__FUNCTION__, __FILE__, "Analyzing ");
			Logging::buildText(__FUNCTION__, __FILE__, i+1);
			Logging::buildText(__FUNCTION__, __FILE__, "-child from node ");
			Logging::buildText(__FUNCTION__, __FILE__, nodeIndex);
			Logging::buildText(__FUNCTION__, __FILE__, " that is node ");
			Logging::buildText(__FUNCTION__, __FILE__, this->graph->getiChild(nodeIndex, i));
			Logging::buildText(__FUNCTION__, __FILE__, ". ");
			Logging::buildText(__FUNCTION__, __FILE__, this->graph->getRefNode(this->graph->getiChild(nodeIndex, i))->toStr());
			Logging::write(true, Info);
#endif

            // Check if point is interior to i-child node.
            if (this->isInteriorToNode(point, this->graph->getiChild(nodeIndex, i)))
            {
               	// Search in next children node.
                nodeIndex = this->graph->getiChild(nodeIndex, i);

                // End loop.
                locatedNode = true;
#ifdef DEBUG_DELAUNAY_LOCATENODE
				Logging::buildText(__FUNCTION__, __FILE__, "Point is inside node ");
				Logging::buildText(__FUNCTION__, __FILE__, nodeIndex);
				Logging::buildText(__FUNCTION__, __FILE__, ". ");
				Logging::buildText(__FUNCTION__, __FILE__, this->graph->getRefNode(nodeIndex)->toStr());
				Logging::write(true, Info);
#endif
            }
            // Next child.
            else
            {
                // Check if all children checked.
            	i++;
                if (i == nChildren)
                {
#ifdef DEBUG_DELAUNAY_LOCATENODE
                	// Print error message.
					Logging::buildText(__FUNCTION__, __FILE__, "Error: point at node index ");
					Logging::buildText(__FUNCTION__, __FILE__, nodeIndex);
					Logging::buildText(__FUNCTION__, __FILE__, " is not interior to any node. Coordinates");
					Logging::buildText(__FUNCTION__, __FILE__, &point);
					Logging::write(true, Error);
#endif
					// End main loop.
					error = true;
                }
            }
        }
    }

    return(this->graph->isLeaf(nodeIndex) && !error);
}


/***************************************************************************
* Name: 	isInteriorToNode
* IN:		point		reference to point to check
* 			nodeIndex	node to check if point is interior to
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	check if "point" is interior or is over an edge of the
* 				"nodeIndex" node. To determine if the point fulfills the
* 				conditions checks that there is no RIGHT turn between the
* 				input point and the points of every edge of the triangle.
***************************************************************************/
bool Delaunay::isInteriorToNode(Point<TYPE> &point, int nodeIndex)
{
	bool	isInterior=false;		// Return value.
	int		id1=0, id2=0, id3=0;	// IDs of vertex points.

	// Get an edge of the face associated to the node.
	this->graph->getVertices(nodeIndex, id1, id2, id3);

#ifdef DEBUG_IS_INTERIOR_TO_NODE
	Logging::buildText(__FUNCTION__, __FILE__, "Vertices are ");
	Logging::buildText(__FUNCTION__, __FILE__, id1);
	Logging::buildText(__FUNCTION__, __FILE__, ",");
	Logging::buildText(__FUNCTION__, __FILE__, id2);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, id3);
	Logging::write(true, Info);
#endif

	// Check if there is not a right turn.
	if (!(this->dcel->returnTurn(&point, id1, id2) == RIGHT_TURN) &&
	    !(this->dcel->returnTurn(&point, id2, id3) == RIGHT_TURN) &&
	    !(this->dcel->returnTurn(&point, id3, id1) == RIGHT_TURN))
	{
	    // Point is interior.
		isInterior = true;
	}

	return(isInterior);
}


/***************************************************************************
* Name: 	isInteriorToNode
* IN:		point		reference to point to check
* 			nodeIndex	node to check if point is interior to
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	check if "point" is interior to the
* 				"nodeIndex" node. To determine if the point fulfills the
* 				conditions checks that there are only LEFT turns between the
* 				input point and the points of every edge of the triangle.
***************************************************************************/
bool Delaunay::isStrictlyInteriorToNode(Point<TYPE> &point, int nodeIndex)
{
	bool isInterior=false;				// Return value.
	int	 id1=0, id2=0, id3=0;			// IDs of vertex points.

	// Get an edge of the face associated to the node.
	this->graph->getVertices(nodeIndex, id1, id2, id3);

#ifdef DEBUG_IS_STRICTLY_INTERIOR_TO_NODE
	Logging::buildText(__FUNCTION__, __FILE__, "Checking if node ");
	Logging::buildText(__FUNCTION__, __FILE__, nodeIndex+1);
	Logging::buildText(__FUNCTION__, __FILE__, " is strictly interior to ");
	Logging::buildText(__FUNCTION__, __FILE__, id1);
	Logging::buildText(__FUNCTION__, __FILE__, ",");
	Logging::buildText(__FUNCTION__, __FILE__, id2);
	Logging::buildText(__FUNCTION__, __FILE__, " and ");
	Logging::buildText(__FUNCTION__, __FILE__, id3);
	Logging::write(true, Info);

	if ((Debug::outOfBouds(id1, P_MINUS_2, this->dcel->getNVertex())) ||
		(Debug::outOfBouds(id1, P_MINUS_2, this->dcel->getNVertex())) ||
		(Debug::outOfBouds(id1, P_MINUS_2, this->dcel->getNVertex())))
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Point id out of bounds ");
		Logging::buildText(__FUNCTION__, __FILE__, id1);
		Logging::buildText(__FUNCTION__, __FILE__, ",");
		Logging::buildText(__FUNCTION__, __FILE__, id2);
		Logging::buildText(__FUNCTION__, __FILE__, " or ");
		Logging::buildText(__FUNCTION__, __FILE__, id3);
		Logging::write(true, Info);
		exit(0);
	}
#endif

	// Check if there is always a turn left.
	if ((this->dcel->returnTurn(&point, id1, id2) == LEFT_TURN) &&
		(this->dcel->returnTurn(&point, id2, id3) == LEFT_TURN) &&
		(this->dcel->returnTurn(&point, id3, id1) == LEFT_TURN))
	{
		// Point is interior.
		isInterior = true;
	}

	return(isInterior);
}

/***************************************************************************
* Name: 	splitNode
* IN:		pointIndex		point index that splits node
* 			nodeIndex		node to split
* 			nTriangles		# triangles after node is splitted
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	creates new nodes after the "pointIndex" point is inserted
* 				in the node. If the point is over an edge then 4 triangles
* 				must be created. Otherwise 3 triangles are created.
***************************************************************************/
void Delaunay::splitNode(int pointIndex, int nodeIndex, int nTriangles)
{
	int 	index=0;
	int		newNodeID=0;
    int     new_Edge_ID=0;			// Edge identifier.
    int     new_Face_ID=0;			// Face identifier.
    int     prev_Edge_ID=0;			// Stores previous edge id.
    int     next_Edge_ID=0;			// Stores next edge id.
    int     collinear_Edge_ID=0;	// Edge identifier of collinear edge.
    int		collinear_Index=0;		// Edge index of collinear edge.
	int		flipCandidates[2];		// Edges that must be cheked due to split operation.
	int		oldNode1=0;				// Old nodes id.
	int		oldNode2=0;				// Old nodes id.
    Face 	*ptrFace;				// Pointer to current face.
	Node 	*ptrNode;				// Pointer to current node.
	Node 	newNode[3];
	double	area[3];

    // Get identifiers of next edge and face to be created.
	new_Edge_ID = this->dcel->getNEdges() + 1;
	new_Face_ID = this->dcel->getNFaces();

    // Update edge departing from new point.
	this->dcel->updateVertex(new_Edge_ID, pointIndex);

    // Get information of node where the new triangles are created.
	ptrNode = this->graph->getRefNode(nodeIndex);

    // Get data of the face of the triangle to be splitted.
	ptrFace = dcel->getRefFace(ptrNode->getFace());

    // Check number of new triangles to create.
    if (nTriangles == 3)
    {
        // Save previous and next edges ID.
    	prev_Edge_ID = this->dcel->getPrevious(ptrFace->getEdge()-1);
    	next_Edge_ID = this->dcel->getNext(ptrFace->getEdge()-1);

        // Insert two new edges: new_Edge_ID and new_Edge_ID+1.
        this->dcel->addEdge(pointIndex+1, new_Edge_ID+5, new_Edge_ID+1, ptrFace->getEdge(), ptrNode->getFace());
        this->dcel->addEdge(this->dcel->getOrigin(next_Edge_ID-1), new_Edge_ID+2, ptrFace->getEdge(), new_Edge_ID, ptrNode->getFace());

        // Insert two new edges: new_Edge_ID+2 and new_Edge_ID+3.
        this->dcel->addEdge(pointIndex+1, new_Edge_ID+1, new_Edge_ID+3, next_Edge_ID, new_Face_ID);
        this->dcel->addEdge(this->dcel->getOrigin(prev_Edge_ID-1), new_Edge_ID+4, next_Edge_ID, new_Edge_ID+2, new_Face_ID);

        // Insert two new edges: new_Edge_ID+4 and new_Edge_ID+5.
        this->dcel->addEdge(pointIndex+1, new_Edge_ID+3, new_Edge_ID+5, prev_Edge_ID, new_Face_ID+1);
        this->dcel->addEdge(this->dcel->getOrigin(ptrFace->getEdge()-1), new_Edge_ID, prev_Edge_ID, new_Edge_ID+4, new_Face_ID+1);

        // Update existing edges.
        this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, new_Edge_ID, new_Edge_ID+1, NO_UPDATE, ptrFace->getEdge()-1);
        this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, new_Edge_ID+2, new_Edge_ID+3, new_Face_ID, next_Edge_ID-1);
        this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, new_Edge_ID+4, new_Edge_ID+5, new_Face_ID+1, prev_Edge_ID-1);

        // Insert two new faces.
        dcel->addFace(new_Edge_ID + 2);
        dcel->addFace(new_Edge_ID + 4);

        // Update leaf node.
        newNodeID = this->graph->getNElements();
        this->graph->getRefNode(nodeIndex)->setChildren(newNodeID, newNodeID+1, newNodeID+2);
        this->graph->update(nodeIndex, 3, ptrNode);

		// Insert three new nodes.
        newNode[0] = Node(	this->dcel->getOrigin(new_Edge_ID),
        					this->dcel->getOrigin(new_Edge_ID - 1),
							this->dcel->getOrigin(ptrFace->getEdge() - 1),
							ptrNode->getFace());
        area[0] = this->signedArea(&newNode[0]);
        newNode[1] = Node(	this->dcel->getOrigin(new_Edge_ID + 2),
        					this->dcel->getOrigin(new_Edge_ID + 1),
							this->dcel->getOrigin(next_Edge_ID - 1),
							new_Face_ID);
        area[1] = this->signedArea(&newNode[1]);
        newNode[2] = Node(	this->dcel->getOrigin(new_Edge_ID + 4),
        					this->dcel->getOrigin(new_Edge_ID + 3),
							this->dcel->getOrigin(prev_Edge_ID - 1),
							new_Face_ID + 1);
        area[2] = this->signedArea(&newNode[2]);
        if (area[0] > area[1])
        {
            // 2nd, 0th, 1st.
            if (area[2] > area[0])
            {
                this->graph->insert(newNode[2]);
                this->graph->insert(newNode[0]);
                this->graph->insert(newNode[1]);
            }
            // 0th, 2nd, 1st.
            else
            {
            	this->graph->insert(newNode[0]);
                //insert_Node(graph, &new_Node[0]);
                if (area[2] > area[1])
                {
                	this->graph->insert(newNode[2]);
                	this->graph->insert(newNode[1]);
                }
                // 0th, 1st, 2nd.
                else
                {
                	this->graph->insert(newNode[1]);
                	this->graph->insert(newNode[2]);
                }
            }
        }
        else
        {
            // 2nd, 1st, 0th.
            if (area[2] > area[1])
            {
            	this->graph->insert(newNode[2]);
            	this->graph->insert(newNode[1]);
            	this->graph->insert(newNode[0]);
            }
            else
            {
            	this->graph->insert(newNode[1]);
                if (area[2] > area[0])
                {
                	this->graph->insert(newNode[2]);
                	this->graph->insert(newNode[0]);
                }
                else
                {
                	this->graph->insert(newNode[0]);
                	this->graph->insert(newNode[2]);
                }
            }
        }
		// Check if edges must be flipped.
        this->checkEdge(ptrFace->getEdge());
		this->checkEdge(prev_Edge_ID);
		this->checkEdge(next_Edge_ID);
    }
    else
    {
        // Get edge identifier where new point is collinear.
    	collinear_Edge_ID = this->dcel->getCollinear(pointIndex, ptrFace->getEdge());

#ifdef DEBUG_SPLIT_NODE
		Logging::buildText(__FUNCTION__, __FILE__, "Point ");
		Logging::buildText(__FUNCTION__, __FILE__, pointIndex+1);
		Logging::buildText(__FUNCTION__, __FILE__, " is collinear to edge ");
		Logging::buildText(__FUNCTION__, __FILE__, collinear_Edge_ID);
		Logging::write(true, Info);
#endif
		if (collinear_Edge_ID != -1)
		{
			collinear_Index = collinear_Edge_ID - 1;

			// Save previous and next edges ID.
			prev_Edge_ID = this->dcel->getRefEdge(collinear_Index)->getPrevious();
			next_Edge_ID = this->dcel->getRefEdge(collinear_Index)->getNext();

#ifdef DEBUG_SPLIT_NODE
			Logging::buildText(__FUNCTION__, __FILE__, "Collinear edges are ");
			Logging::buildText(__FUNCTION__, __FILE__, collinear_Edge_ID);
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getTwin(collinear_Index));
			Logging::buildText(__FUNCTION__, __FILE__, ". Origins are ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getOrigin(collinear_Index));
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getOrigin(this->dcel->getTwin(collinear_Index) - 1));
			Logging::write(true, Info);
#endif

			// Store edges that must be checked after split of first triangle operation.
			flipCandidates[0] = next_Edge_ID;
			flipCandidates[1] = prev_Edge_ID;

			// Store nodes ID that are going to be updated.
			oldNode1 = this->graph->getNodeAssigned(this->dcel->getRefEdge(collinear_Index)->getFace());
			index = this->dcel->getRefEdge(collinear_Index)->getTwin() - 1;
			oldNode2 = this->graph->getNodeAssigned(this->dcel->getRefEdge(index)->getFace());

#ifdef DEBUG_SPLIT_NODE
			Logging::buildText(__FUNCTION__, __FILE__, "Nodes that share edge are ");
			Logging::buildText(__FUNCTION__, __FILE__, oldNode1);
			Logging::buildText(__FUNCTION__, __FILE__, " and ");
			Logging::buildText(__FUNCTION__, __FILE__, oldNode2);
			Logging::write(true, Info);
			Logging::buildText(__FUNCTION__, __FILE__, this->graph->getRefNode(oldNode1)->toStr());
			Logging::buildText(__FUNCTION__, __FILE__, this->graph->getRefNode(oldNode2)->toStr());
			Logging::write(true, Info);
#endif

			// Update current face with new edge: new_Edge_ID.
			this->dcel->addEdge(this->dcel->getRefEdge(prev_Edge_ID-1)->getOrigin(), new_Edge_ID+1, next_Edge_ID,collinear_Edge_ID, ptrNode->getFace());

			// Insert a new face with two new edges: new_Edge_ID+1 and new_Edge_ID+2.
			this->dcel->addEdge(pointIndex+1, new_Edge_ID, new_Edge_ID+2, prev_Edge_ID, new_Face_ID);
			this->dcel->addEdge(this->dcel->getRefEdge(collinear_Index)->getOrigin(), new_Edge_ID+3, prev_Edge_ID,new_Edge_ID+1, new_Face_ID);

			this->dcel->updateVertex(new_Edge_ID+1, pointIndex);
			this->dcel->updateFace(new_Edge_ID, ptrNode->getFace());

			// Update existing edges.
			this->dcel->updateEdge(pointIndex+1, NO_UPDATE, new_Edge_ID, NO_UPDATE, NO_UPDATE, collinear_Index);
			this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, NO_UPDATE, new_Edge_ID, NO_UPDATE, next_Edge_ID-1);
			this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, new_Edge_ID+1, new_Edge_ID+2, new_Face_ID, prev_Edge_ID-1);

			// Get node of current edge and update it.
			ptrNode = this->graph->getRefNode(oldNode1);
			ptrNode->setChildren(this->graph->getNElements(), this->graph->getNElements()+1, INVALID);
			this->graph->update(oldNode1, 2, ptrNode);

			// Insert two new nodes in first node splitted.
	        newNode[0] = Node(	this->dcel->getOrigin(this->dcel->getPrevious(collinear_Index)-1),
	        					this->dcel->getOrigin(collinear_Index),
								this->dcel->getOrigin(this->dcel->getNext(collinear_Index)-1),
								this->dcel->getFace(collinear_Index));
			this->graph->insert(newNode[0]);
#ifdef DEBUG_SPLIT_NODE
			Logging::buildText(__FUNCTION__, __FILE__, "Splitting 1st triangle ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefFace(newNode[0].getFace())->toStr());
			Logging::buildText(__FUNCTION__, __FILE__, "1st node is: ");
			Logging::buildText(__FUNCTION__, __FILE__, newNode[0].toStr());
			Logging::write(true, Info);
#endif
	        newNode[1] = Node(	this->dcel->getOrigin(this->dcel->getPrevious(collinear_Index)-1),
	        					this->dcel->getOrigin(this->dcel->getPrevious(this->dcel->getTwin(this->dcel->getPrevious(collinear_Index)-1)-1)-1),
								this->dcel->getOrigin(this->dcel->getTwin(this->dcel->getPrevious(collinear_Index)-1)-1),
								new_Face_ID);
			this->graph->insert(newNode[1]);

			// Insert new face.
			this->dcel->addFace(new_Edge_ID + 2);
#ifdef DEBUG_SPLIT_NODE
			Logging::buildText(__FUNCTION__, __FILE__, "Splitting 2nd triangle ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefFace(newNode[1].getFace())->toStr());
			Logging::buildText(__FUNCTION__, __FILE__, "Second node is: ");
			Logging::buildText(__FUNCTION__, __FILE__, newNode[1].toStr());
			Logging::write(true, Info);
#endif

			// Update twin face.
			collinear_Edge_ID = this->dcel->getTwin(collinear_Index);
			collinear_Index	 = collinear_Edge_ID-1;
			prev_Edge_ID	 = this->dcel->getPrevious(collinear_Index);
			next_Edge_ID	 = this->dcel->getNext(collinear_Index);

			// Insert a new face with two new edges: new_Edge_ID+3 and new_Edge_ID+4.
			this->dcel->addEdge(pointIndex+1, new_Edge_ID+2, new_Edge_ID+4, next_Edge_ID, new_Face_ID+1);
			this->dcel->addEdge(this->dcel->getOrigin(prev_Edge_ID-1), new_Edge_ID+5, next_Edge_ID, new_Edge_ID+3, new_Face_ID+1);

			// Update current face with new edge: new_Edge_ID+5.
			this->dcel->addEdge(pointIndex+1, new_Edge_ID+4, collinear_Edge_ID, prev_Edge_ID, this->dcel->getFace(collinear_Index));

			// Update existing edges.
			this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, new_Edge_ID+3, new_Edge_ID+4, new_Face_ID+1, next_Edge_ID-1);
			this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, NO_UPDATE, new_Edge_ID+5, NO_UPDATE, collinear_Index);
			this->dcel->updateEdge(NO_UPDATE, NO_UPDATE, new_Edge_ID+5, NO_UPDATE, NO_UPDATE, prev_Edge_ID-1);

			// Get node of twin edge and update it.
			ptrNode = this->graph->getRefNode(oldNode2);
			ptrNode->setChildren(this->graph->getNElements(), this->graph->getNElements()+1, INVALID);
			this->graph->update(oldNode2, 2, ptrNode);

			// Insert two new nodes in first node splitted.
	        newNode[0] = Node(	this->dcel->getOrigin(this->dcel->getPrevious(collinear_Index)-1),
	        					this->dcel->getOrigin(collinear_Index),
								this->dcel->getOrigin(this->dcel->getNext(collinear_Index)-1),
								this->dcel->getFace(collinear_Index));
			this->graph->insert(newNode[0]);

			// Update face.
			this->dcel->updateFace(collinear_Edge_ID, newNode[0].getFace());
#ifdef DEBUG_SPLIT_NODE
			Logging::buildText(__FUNCTION__, __FILE__, "Splitting first triangle ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefFace(newNode[0].getFace())->toStr());
			Logging::write(true, Info);
#endif
	        newNode[1] = Node(	this->dcel->getOrigin(this->dcel->getPrevious(next_Edge_ID-1)-1),
	        					this->dcel->getOrigin(next_Edge_ID-1),
								this->dcel->getOrigin(this->dcel->getNext(next_Edge_ID-1)-1),
								this->dcel->getFace(next_Edge_ID-1));
			this->graph->insert(newNode[1]);

			// Insert new face.
			this->dcel->addFace(new_Edge_ID + 4);
#ifdef DEBUG_SPLIT_NODE
			Logging::buildText(__FUNCTION__, __FILE__, "Splitting first triangle ");
			Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getRefFace(newNode[1].getFace())->toStr());
			Logging::write(true, Info);
#endif

			// Check candidates from first triangle.
			this->checkEdge(flipCandidates[0]);
			this->checkEdge(flipCandidates[1]);

			// Check candidates from second triangle.
			this->checkEdge(prev_Edge_ID);
			this->checkEdge(next_Edge_ID);
		}
#ifdef DEBUG_SPLIT_NODE
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Collinear edge is negative. Point index ");
			Logging::buildText(__FUNCTION__, __FILE__, pointIndex);
			Logging::buildText(__FUNCTION__, __FILE__, " and face edge ");
			Logging::buildText(__FUNCTION__, __FILE__, ptrFace->getEdge());
			Logging::write(true, Info);
		}
#endif
    }
}


/***************************************************************************
* Name: 	signedArea
* IN:		node		node whose area must be computed
* OUT:		NONE
* RETURN:	node area
* GLOBAL:	NONE
* Description: 	computes the area of the three points of the node. If any
* 				of the points is imaginary then area is 0.
***************************************************************************/
double Delaunay::signedArea(Node *node)
{
	double   area=0.0;           // Return value.

	// Check if any of the vertex is not real: P_MINUS_! or P_MINUS_2.
	if ((node->getiChild(0) < 0) ||
		(node->getiChild(1) < 0) ||
		(node->getiChild(2) < 0))
	{
		// Set area zero.
		area = 0.0;
	}
	else
	{
		// Compute signed area.
		area = this->dcel->signedArea(node->getiChild(0)-1,
									node->getiChild(1)-1,
									node->getiChild(2)-1);
	}

	return(area);
}
