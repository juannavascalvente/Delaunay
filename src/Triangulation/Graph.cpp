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



/***************************************************************************
* Name: 	print
* IN:		out			output stream.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	print graph data.
***************************************************************************/
void Graph::print(std::ostream& out)
{
    int     i=0;            // Loop counter.

    // Print graph length and size.
    out << "Graph # elements: " << this->getSize() << std::endl;
    out << "Graph size: " << this->getSize() << std::endl;

    // Print graph nodes.
    for (i=0; i<this->getSize() ;i++)
    {
    	this->vNodes.at(i).print(out);
    }

	// Print face-node relations.
	for (i=0; i<this->getSize() ;i++)
	{
		out << vFaceNode.at(i) << endl;
	}
}

/***************************************************************************
* Name: 	read
* IN:		fileName		file name to read data from.
* OUT:		NONE
* RETURN:	false			if error writing data.
* 			true			i.o.c.
* GLOBAL:	NONE
* Description: 	read the graph data from "fileName" file.
***************************************************************************/
bool Graph::read(const string& fileName)
{
	bool 	read=true;	// Return value.
	ifstream ifs;		// Input file.
	int		i=0;		// Loop counter.
	int		size=0;		// Graph size.
	Node	node;

#ifdef DEBUG_GRAPH_READ
	Logging::buildText(__FUNCTION__, __FILE__, "Opening file: ");
	Logging::buildText(__FUNCTION__, __FILE__, fileName);
	Logging::write(true, Info);
#endif

	// Open file.
	ifs.open(fileName.c_str(), ios::in);

	// Check file is opened.
	if (ifs.is_open())
	{
		// Read # nodes.
		ifs >> size;

		// Nodes main loop.
		for (i=0; i<this->getSize() ;i++)
		{
			node.read(ifs);
			this->insert(node);
		}

		// Read face-node relations.
		for (i=0; i<this->getSize() ;i++)
		{
		    int iFaceId;
			ifs >> iFaceId;
			this->vFaceNode.push_back(iFaceId);
		}

		// Close file.
		ifs.close();
#ifdef DEBUG_GRAPH_READ
		this->print(std::cout);
#endif
	}
	else
	{
#ifdef DEBUG_GRAPH_READ
	Logging::buildText(__FUNCTION__, __FILE__, "Error opening input file ");
	Logging::buildText(__FUNCTION__, __FILE__, fileName.c_str());
	Logging::write(true, Info);
#endif
		read = false;
	}

#ifdef DEBUG_GRAPH_READ
	Logging::buildText(__FUNCTION__, __FILE__, "File read successfully.");
	Logging::write(true, Info);
#endif

	return(read);
}


/***************************************************************************
* Name: 	write
* IN:		fileName		file name to write data to.
* OUT:		NONE
* RETURN:	false			if error writing data.
* 			true			i.o.c.
* GLOBAL:	NONE
* Description: 	writes the graph data to "fileName" file.
***************************************************************************/
bool Graph::write(const string& fileName)
{
	bool success=true;	// Return value.
	ofstream ofs;		// Input file.
	int	i=0;			// Loop counter.

	// Open file.
	ofs.open(fileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Write # nodes.
		ofs << this->getSize() << endl;

		// Nodes main loop.
		for (i=0; i<this->getSize() ;i++)
		{
			this->vNodes.at(i).write(ofs);
			ofs << endl;
		}

		// Read face-node relations.
		for (i=0; i<this->getSize() ;i++)
		{
			ofs << this->vFaceNode.at(i) << endl;
		}

		// Close file.
		ofs.close();
	}
	else
	{
#ifdef DEBUG_GRAPH_WRITE
	Logging::buildText(__FUNCTION__, __FILE__, "Error opening output file ");
	Logging::buildText(__FUNCTION__, __FILE__, fileName.c_str());
	Logging::write(true, Info);
#endif
		success = false;
	}

	return(success);
}
