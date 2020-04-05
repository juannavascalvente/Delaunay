/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelModel.h"
#include "Logging.h"



/***********************************************************************************************************************
* Public function definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	addVertex
* IN:		p			new point coordinates
* 			edge		new edge identifier.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new point to the vertex array.
***************************************************************************/
void DcelModel::addVertex(const Point<TYPE> *p, const int edge)
{
    Vertex v(edge, *p);
    vVertex.push_back(v);
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
void DcelModel::updateVertex(int edge_ID, int index)
{
    // Check if index is out of bounds.
    if (index < this->vVertex.size())
    {
        // Update vertex edge.
        this->vVertex.at(index).setOrigin(edge_ID);
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
        Logging::buildRange(__FUNCTION__, __FILE__, 0, this->vVertex.size());
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
void DcelModel::updateVertex(Point<TYPE> *p, int index)
{
    // Check if index is out of bounds.
    if (index < this->vVertex.size())
    {
        // Update vertex edge.
        this->vVertex.at(index).setPoint(p);
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
        Logging::buildRange(__FUNCTION__, __FILE__, 0, this->vVertex.size());
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
void DcelModel::addEdge(int origin, int twin, int previous, int next, int face)
{
    Edge e(origin, twin, previous, next, face);
    this->vEdges.push_back(e);
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
void DcelModel::updateEdge(int origin, int twin, int previous, int next, int face, int index)
{
    // Check if faces vector is full.
    if (index < this->vEdges.size())
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
            this->vEdges.at(index).setOrigin(origin);
        }

        // Check if twin edge field must be updated.
        if (twin != NO_UPDATE)
        {
            this->vEdges.at(index).setTwin(twin);
        }

        // Check if previous edge field must be updated.
        if (previous != NO_UPDATE)
        {
            this->vEdges.at(index).setPrevious(previous);
        }

        // Check if next edge field must be updated.
        if (next != NO_UPDATE)
        {
            this->vEdges.at(index).setNext(next);
        }

        // Check if face field must be updated.
        if (face != NO_UPDATE)
        {
            this->vEdges.at(index).setFace(face);
        }
    }
    else
    {
        // PENDING: EXCEPTION?
        Logging::buildText(__FUNCTION__, __FILE__, "Updating edge with index ");
        Logging::buildText(__FUNCTION__, __FILE__, index);
        Logging::buildText(__FUNCTION__, __FILE__, " out of bounds.");
        Logging::buildRange(__FUNCTION__, __FILE__, 0, this->vEdges.size());
        Logging::write(true, Error);
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
void DcelModel::addFace(const int edge)
{
    // Add face
    Face face(edge);
    this->vFaces.push_back(face);
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
void DcelModel::updateFace(int edge_ID, int index)
{
    // JNC PENDING ALL INDEX ACCESSES TO THROW EXCEPTION.
    try
    {
        // Update edge id of face at index-th position.
        this->vFaces.at(index).setEdge(edge_ID);
    }
    catch (exception &ex)
    {
        ex.what();
    }
}


/***************************************************************************
* Name: 	swapVertex
* IN:		N/A
* OUT:		N/A
* IN/OUT:	N/A
* RETURN:	N/A
* Description: swaps vertex positions in array.
***************************************************************************/
void DcelModel::swapVertex(int index1, int index2)
{
    Vertex vertex;	// Temporary variable.

    if ((index1 < this->getNumVertex()) || (index2 < this->getNumVertex()))
    {
        // Swap vertices.
        vertex = this->vVertex.at(index1);
        this->vVertex.at(index1) = this->vVertex.at(index2);
        this->vVertex.at(index2) = vertex;
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


void DcelModel::getEdgePoints(int edgeIndex, Point<TYPE> &origin, Point<TYPE> &dest)
{
#ifdef DEBUG_OUTOFBOUNDS_EXCEPTION
    if (edgeIndex >= this->getNumEdges())
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


void DcelModel::getFaceVertices(int faceIndex, int *ids)
{
    int		edgeIndex=0;			// Edge index.

    // Get index edge from face.
    edgeIndex = this->getFaceEdge(faceIndex) - 1;

    // Get face vertices.
    ids[0] = this->getOrigin(edgeIndex);
    ids[1] = this->getOrigin(this->getNext(edgeIndex)-1);
    ids[2] = this->getOrigin(this->getPrevious(edgeIndex)-1);
}


void DcelModel::getFacePoints(int faceIndex, Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r)
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


void DcelModel::reset()
{
    // Reset counters.
    this->vVertex.clear();
    this->vEdges.clear();
    this->vFaces.clear();
}


bool DcelModel::operator==(const DcelModel& other) const
{
    bool isEqual;	    // Return value.

    isEqual =  ((this->vVertex == other.vVertex) &&
                (this->vEdges == other.vEdges) &&
                (this->vFaces == other.vFaces));

    return(isEqual);
}
