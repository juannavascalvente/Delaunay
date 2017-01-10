/*
 * Graph.cpp
 *
 *  Created on: Jul 11, 2016
 *      Author: jnavas
 */

#include "defines.h"
#include "Graph.h"
#include "Logging.h"

#include <string.h>
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

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Graph::Graph()
{
	// Initialize variables.
	this->size = 0;
	this->nElements = 0;
	this->nodes = NULL;
	this->face_Node_Assign = NULL;
	this->allocated = false;
}


Graph::Graph(int size)
{
	try
	{
		// Initialize variables.
		this->size = size;
		this->nElements = 0;

		// Allocate memory initializing data.
		this->nodes = new Node[size];
		this->face_Node_Assign = new int[size];
		memset( this->face_Node_Assign, 0, sizeof(int)*size);

		this->allocated = true;
	}
	catch (bad_alloc &ex)
	{
		this->allocated = false;
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory. # nodes to allocate ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Error);
	}
	catch (exception &ex)
	{
		this->allocated = false;
		std::cout << ex.what();
	}
}

Graph::~Graph()
{
	// Deallocate data.
	if (this->allocated)
	{
		this->allocated = false;
		delete[] this->nodes;
		delete[] this->face_Node_Assign;
	}
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	insert
* IN:		node		node to insert
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	inserts a node in the last position of the nodes array.
***************************************************************************/
void Graph::insert( Node &node)
{
    // Check if array is full.
    if (this->isFull())
    {
    	this->resize( INVALID, true);
#ifdef DEBUG_INSERT_NODE
    	Logging::buildText(__FUNCTION__, __FILE__, "Graph is full -> resizing.");
    	Logging::write( true, Info);
#endif
    }

    // Copy node data.
    this->nodes[this->nElements] = node;

    // Face is negative -> reading node from file with default node data.
    if (node.getFace() > 0)
    {
        this->face_Node_Assign[node.getFace()] = this->nElements;
    }

#ifdef DEBUG_INSERT_NODE
	Logging::buildText(__FUNCTION__, __FILE__, "Inserting new node at position ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
	Logging::buildText(__FUNCTION__, __FILE__, ". ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nodes[this->nElements].toStr());
	Logging::buildText(__FUNCTION__, __FILE__, ".\nFace ");
	Logging::buildText(__FUNCTION__, __FILE__, node.getFace());
	Logging::buildText(__FUNCTION__, __FILE__, " assigned to node ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
	Logging::write( true, Info);
#endif

	// Increase number of elements.
    this->nElements++;
}


/***************************************************************************
* Name: 	update
* IN:		index		node index to update
* 			nChildren	# children to set
* 			node		pointer to node data
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	updates the node information of node at position "index".
***************************************************************************/
void Graph::update( int index, int nChildren, Node *node)
{
	try
	{
		// Update values of node at position index.
		this->nodes[index] = (*node);

#ifdef DEBUG_UPDATE_NODE
		Logging::buildText( __FUNCTION__, __FILE__, "Updating node ");
		Logging::buildText( __FUNCTION__, __FILE__, index);
		Logging::buildText( __FUNCTION__, __FILE__, ".");
		Logging::buildText( __FUNCTION__, __FILE__, this->nodes[index].toStr());
		Logging::write( true, Info);
#endif
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
void Graph::getVertices( int node_Index, int &index1, int &index2, int &index3)
{
	// Get point vertexes of node at node_Index position.
	index1 = this->nodes[node_Index].getiPoint(0);
	index2 = this->nodes[node_Index].getiPoint(1);
	index3 = this->nodes[node_Index].getiPoint(2);
}


/***************************************************************************
* Name: 	reset
* IN:		size		new graph size.
* OUT:		NONE
* RETURN:	true		if resized ok
* 			false		otherwise
* GLOBAL:	NONE
* Description: 	allocates a new graph whose size is "size". If "size" is
* 				INVALID then the new graph is twice the previous graph. If
* 				"copy" is true then data from previous graph is copied to
* 				the new graph.
***************************************************************************/
bool Graph::resize( int size, bool copy)
{
	this->allocated = false;

	try
	{
		// Temporary buffers.
		Node 	*tmpNodes;
		int 	*tmpAssigned;

		// Check if current content must be copied.
		if (copy)
		{
#ifdef DEBUG_GRAPH_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Copying existing data");
			Logging::write( true, Info);
#endif
			tmpNodes = this->nodes;
			tmpAssigned = this->face_Node_Assign;
		}
		// If not -> reset # elements.
		else
		{
			this->nElements = 0;
		}

		// New size unknown -> allocate twice current space.
		if (size == INVALID)
		{
#ifdef DEBUG_GRAPH_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Doubling size to ");
#endif
			this->size = this->size*2;
		}
		else
		{
#ifdef DEBUG_GRAPH_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Input size is ");
#endif
			this->size = size;
		}
#ifdef DEBUG_GRAPH_RESIZE
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Info);
#endif

		// PENDING What to do if size is negative?
		// Check if new size is negative.
		if (this->size < 0)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "New graph size is negative: ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Error);
		}
		else
		{
			// Allocate new memory.
			tmpNodes = new Node[this->size];
			tmpAssigned = new int[this->size];
			memset( tmpAssigned, 0, sizeof(int)*this->size);

			// copy current data.
			if (copy)
			{
				// Copy current data.
				memcpy( tmpNodes, this->nodes, this->nElements*sizeof(Node));
				memcpy( tmpAssigned, this->face_Node_Assign, this->nElements*sizeof(int));
			}

			// Deallocate data.
			delete[] this->nodes;
			delete[] this->face_Node_Assign;

			// Reference new data.
			this->nodes = tmpNodes;
			this->face_Node_Assign = tmpAssigned;

#ifdef DEBUG_GRAPH_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Resizing graph. New size is ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Info);
#endif
			this->allocated = true;
		}
	}
	catch (bad_alloc &ex)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory. New # nodes is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Error);
	}
	catch (exception &ex)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Exception.");
		Logging::write( true, Error);
		std::cout << ex.what();
	}

	return(this->allocated);
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
	// Reset variables.
	this->nElements = 0;
	memset( this->nodes, 0, sizeof(Node)*this->size);
	memset( this->face_Node_Assign, 0, sizeof(int)*this->size);
#ifdef DEBUG_GRAPH_RESET
	Logging::buildText(__FUNCTION__, __FILE__, "Reseting graph");
	Logging::write( true, Info);
#endif
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
    out << "Graph # elements: " << this->getNElements() << std::endl;
    out << "Graph size: " << this->getSize() << std::endl;

    // Print graph nodes.
    for (i=0; i<this->getSize() ;i++)
    {
    	this->nodes[i].print( out);
    }

	// Print face-node relations.
	for (i=0; i<this->getSize() ;i++)
	{
		out << face_Node_Assign[i] << endl;
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
bool Graph::read(string fileName)
{
	bool 	read=true;	// Return value.
	ifstream ifs;		// Input file.
	int		i=0;		// Loop counter.
	int		size=0;		// Graph size.
	Node	node;

#ifdef DEBUG_GRAPH_READ
	Logging::buildText( __FUNCTION__, __FILE__, "Opening file: ");
	Logging::buildText( __FUNCTION__, __FILE__, fileName);
	Logging::write( true, Info);
#endif

	// Open file.
	ifs.open( fileName.c_str(), ios::in);

	// Check file is opened.
	if (ifs.is_open())
	{
		// Read # nodes.
		ifs >> size;

		// Initialize graph.
		this->resize( size, false);

		// Nodes main loop.
		for (i=0; i<this->getSize() ;i++)
		{
			node.read( ifs);
			this->insert( node);
		}

		// Read face-node relations.
		for (i=0; i<this->getSize() ;i++)
		{
			ifs >> face_Node_Assign[i];
		}

		// Set data to valid.
		this->allocated = true;

		// Close file.
		ifs.close();
#ifdef DEBUG_GRAPH_READ
		this->print( std::cout);
#endif
	}
	else
	{
#ifdef DEBUG_GRAPH_READ
	Logging::buildText(__FUNCTION__, __FILE__, "Error opening input file ");
	Logging::buildText(__FUNCTION__, __FILE__, fileName.c_str());
	Logging::write( true, Info);
#endif
		read = false;
	}

#ifdef DEBUG_GRAPH_READ
	Logging::buildText( __FUNCTION__, __FILE__, "File read successfully.");
	Logging::write( true, Info);
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
bool Graph::write(string fileName)
{
	bool success=true;	// Return value.
	ofstream ofs;		// Input file.
	int	i=0;			// Loop counter.

	// Open file.
	ofs.open( fileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Write # nodes.
		ofs << this->getSize() << endl;

		// Nodes main loop.
		for (i=0; i<this->getSize() ;i++)
		{
			this->nodes[i].write( ofs);
			ofs << endl;
		}

		// Read face-node relations.
		for (i=0; i<this->getSize() ;i++)
		{
			ofs << this->face_Node_Assign[i] << endl;
		}

		// Close file.
		ofs.close();
	}
	else
	{
#ifdef DEBUG_GRAPH_WRITE
	Logging::buildText(__FUNCTION__, __FILE__, "Error opening output file ");
	Logging::buildText(__FUNCTION__, __FILE__, fileName.c_str());
	Logging::write( true, Info);
#endif
		success = false;
	}

	return(success);
}
