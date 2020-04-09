/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "Graph.h"
#include "Logging.h"

#include <iostream>
using namespace std;

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_GRAPH
//#define DEBUG_GRAPH_RESIZE
//#define DEBUG_GRAPH_RESET
//#define DEBUG_UPDATE_NODE
//#define DEBUG_INSERT_NODE
//#define DEBUG_GRAPH_READ
//#define DEBUG_GRAPH_WRITE
#endif


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	insert
* IN:		node		node to insert
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	inserts a node in the last position of the nodes array.
***************************************************************************/
void Graph::insert(Node &node)
{
    // Copy node data.
    this->vNodes.push_back(node);

    // Face is negative -> reading node from file with default node data.
    if (node.getFace() > 0)
    {
        if (node.getFace() >= this->vFaceNode.size())
        {
            this->vFaceNode.resize(node.getFace()*2, 0);
        }

        this->vFaceNode.at(node.getFace()) = this->vNodes.size() - 1;
    }
}


int Graph::getNodeAssigned(int face)
{
    if (face >= this->vFaceNode.size())
    {
        cout << "Error" << endl;
        return 0;
    }

    return this->vFaceNode.at(face);
};

/***************************************************************************
* Name: 	update
* IN:		index		node index to update
* 			node		pointer to node data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	updates the node information of node at position "index".
***************************************************************************/
void Graph::update(int index, Node *node)
{
	try
	{
		// Update values of node at position index.
		this->vNodes.at(index) = (*node);
	}
	catch (exception &ex)
	{
		ex.what();
	}
}


/***************************************************************************
* Name: getVertices
* IN:	graph			graph data
* 		node_Index		node index
* OUT:		index1		first vertex id
* 			index2		second vertex id
* 			index3		third vertex id
* IN/OUT:	N/A
* RETURN:	N/A
* Description: 	returns the three vertex id of the "node_Index" node.
***************************************************************************/
void Graph::getVertices(int node_Index, int &index1, int &index2, int &index3)
{
	// Get point vertexes of node at node_Index position.
	index1 = this->vNodes.at(node_Index).getiPoint(0);
	index2 = this->vNodes.at(node_Index).getiPoint(1);
	index3 = this->vNodes.at(node_Index).getiPoint(2);
}


/***************************************************************************
* Name: 	reset
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	reset the graph variables.
***************************************************************************/
void Graph::reset()
{
	// Reset vectors
	this->vNodes.clear();
    this->vFaceNode.clear();
}
