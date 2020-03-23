/*
 * Gabriel.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: juan
 */

#include "Gabriel/Gabriel.h"
#include "Line.h"
#include "Logging.h"
#include "Point.h"
#include <stdio.h>
#include <string.h>

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_GABRIEL_CONSTRUCTOR
//#define DEBUG_GABRIEL_BUILD
//#define DEBUG_GABRIEL_VALIDATE_EDGES
//#define DEBUG_GABRIEL_RESIZE
//#define DEBUG_GABRIEL_READ
#endif

//------------------------------------------------------------------------
// Constructors / Destructor.
//------------------------------------------------------------------------
Gabriel::Gabriel()
{
	this->valid = false;
	this->size = 0;
	this->dcel = NULL;
	this->voronoi = NULL;
	this->validEdge = NULL;
}

Gabriel::Gabriel( Dcel *dcel, Voronoi *voronoi)
{
	try
	{
		this->dcel = dcel;
		this->voronoi = voronoi;
		this->valid = true;
		this->size = this->dcel->getNEdges();
		this->validEdge = new bool[this->size];
		memset( this->validEdge, 0, sizeof(bool)*this->size);
#ifdef DEBUG_GABRIEL_CONSTRUCTOR
		Logging::buildText(__FUNCTION__, __FILE__, "Gabriel graph length: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Info);
#endif
	}
	catch (std::bad_alloc &ex)
	{
		this->valid = false;
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory.");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Error);
	}
	catch (std::exception &ex)
	{
		this->valid = false;
		std::cout << ex.what();
	}
}

Gabriel::~Gabriel()
{
	if (this->valid)
	{
		delete[] this->validEdge;
		this->valid = false;
	}
}

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	init
* IN:		dcel			DCEL triangulation data.
* 			voronoi			Voronoi data.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description:	allocates the Gabriel gprah data vector.
***************************************************************************/
void Gabriel::init( Dcel *dcel, Voronoi *voronoi)
{
	try
	{
		this->dcel = dcel;
		this->voronoi = voronoi;
		this->resize(dcel->getNEdges(), false);
#ifdef DEBUG_GABRIEL_INIT
		Logging::buildText(__FUNCTION__, __FILE__, "Creating Gabriel graph length: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Info);
#endif
	}
	catch (std::bad_alloc &ex)
	{
		this->valid = false;
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory.");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Error);
	}
	catch (std::exception &ex)
	{
		this->valid = false;
		std::cout << ex.what();
	}
}


/***************************************************************************
* Name: 	build
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description:	builds the Gabriel graph using the DCEl attribute as the
* 				triangulation to check. For every edge in the triangulation
* 				it checks if the circle whose diameter is that edge
* 				surrounds any other point of the triangulation.
***************************************************************************/
bool Gabriel::build()
{
    int     edgeIndex=0;		// Loop counter.
    int		nEdges=0;			// Upper bound limit.
    Point<TYPE> *p1, *p2;		// Edge origin and destination points.
    Point<TYPE> q1, q2;		// Edge origin and destination points.
    int     face1=0, face2=0; 	// Faces that share edge.
    Line 	line1, line2;

    // Check Gabriel graph allocated.
    if (this->valid)
    {
		// Check all edges.
		nEdges = this->getSize();
#ifdef DEBUG_GABRIEL_BUILD
		Logging::buildText(__FUNCTION__, __FILE__, "Building Gabriel graph. # edges: ");
		Logging::buildText(__FUNCTION__, __FILE__, nEdges);
		Logging::write( true, Info);
#endif
		for (edgeIndex=0; edgeIndex<nEdges ;edgeIndex++)
		{
#ifdef DEBUG_GABRIEL_BUILD
			Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, "/");
			Logging::buildText(__FUNCTION__, __FILE__, nEdges);
			Logging::write( true, Info);
#endif
			// Skip edges higher than its twin -> already checked.
			if (edgeIndex < this->dcel->getTwin(edgeIndex))
			{
				// Skip imaginary edges.
				if (!this->dcel->hasNegativeVertex(edgeIndex+1))
				{
#ifdef DEBUG_GABRIEL_BUILD
					Logging::buildText(__FUNCTION__, __FILE__, "Checking intersection for edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write( true, Info);
#endif
	                // Get faces that share edge.
					face1 = this->dcel->getFace( edgeIndex);
					face2 = this->dcel->getFace( this->dcel->getTwin(edgeIndex)-1);
	                //face1 = dcel->edges[edge_Index].face;
	                //face2 = dcel->edges[dcel->edges[edge_Index].twin_Edge-1].face;

					// Get origin and destination point of triangulation segment.
					p1 = this->dcel->getRefPoint( this->dcel->getOrigin( edgeIndex)-1);
					p2 = this->dcel->getRefPoint( this->dcel->getOrigin( this->dcel->getTwin(edgeIndex)-1)-1);
					//p1 = dcel->vertex[dcel->edges[edge_Index].origin_Vertex-1].vertex;
					//p2 = dcel->vertex[dcel->edges[dcel->edges[edge_Index].twin_Edge-1].origin_Vertex-1].vertex;

					// Get origin and destination point of triangulation segment.
					this->voronoi->getCentre( face1-1, q1);
					this->voronoi->getCentre( face2-1, q2);
					//get_Voronoi_Centre( voronoi_Set, face1-1, &q1);
					//get_Voronoi_Centre( voronoi_Set, face2-1, &q2);

					// Create lines to check intersection.
					line1 = Line( *p1, *p2);
					line2 = Line( q1, q2);

					// Check if both segment intersect.
					if (line1.intersect( line2))
					//if (segment_Intersect( &p1, &p2, &q1, &q2))
					{
#ifdef DEBUG_GABRIEL_BUILD
						Logging::buildText(__FUNCTION__, __FILE__, "Adding edge ");
						Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
						Logging::write( true, Info);
#endif
						// Set edge and its twin as valid Gabriel edges.
						this->validateEdges( edgeIndex);
						//draw_Edge( dcel, 2.0, RED, edge_Index);
					}
#ifdef DEBUG_GABRIEL_BUILD
					else
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Not intersection for edge ");
						Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
						Logging::write( true, Info);
					}
#endif
				}
#ifdef DEBUG_GABRIEL_BUILD
				else
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Non real edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write( true, Info);
				}
#endif
			}
#ifdef DEBUG_GABRIEL_BUILD
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Skipping edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, " because twin edge already checked ");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getTwin(edgeIndex));
				Logging::write( true, Info);
			}
#endif
		}
    }

    return(this->valid);
}


//------------------------------------------------------------------------
// Private functions.
//------------------------------------------------------------------------
/***************************************************************************
* Name: 	validateEdges
* IN:		edgeIndex			edge index to set to true
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	sets to true the input edge and its twin.
***************************************************************************/
void Gabriel::validateEdges( int edgeIndex)
{
#ifdef DEBUG_GABRIEL_VALIDATE_EDGES
	Logging::buildText(__FUNCTION__, __FILE__, "Setting edge ");
	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
	Logging::buildText(__FUNCTION__, __FILE__, " and its twin ");
	Logging::buildText(__FUNCTION__, __FILE__, this->dcel->getTwin( edgeIndex)-1);
	Logging::write( true, Info);
#endif
	this->set( edgeIndex);
	this->set( this->dcel->getTwin( edgeIndex)-1);
}

/***************************************************************************
* Name: 	resize
* IN:		size			new size
* 			copy 			flag to copy current data to new vector.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	sets to true the input edge and its twin.
***************************************************************************/
bool Gabriel::resize( int size, bool copy)
{
	this->valid = false;

	try
	{
		// Temporary buffers.
		bool 	*tmp;

		// Check if current content must be copied.
		if (copy)
		{
#ifdef DEBUG_GABRIEL_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Copying existing data");
			Logging::write( true, Info);
#endif
			tmp = this->validEdge;
		}

		// New size unknown -> allocate twice current space.
		if (size == INVALID)
		{
#ifdef DEBUG_GABRIEL_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Doubling size to ");
			Logging::write( true, Info);
#endif
			this->size = this->size*2;
		}
		else
		{
#ifdef DEBUG_GABRIEL_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Input size is ");
#endif
			this->size = size;
		}

		// PENDING What to do if size is negative?
		// Check if new size is negative.
		if (this->size < 0)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "New Gabriel size is negative: ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Error);
		}
		else
		{
			// Allocate new memory.
			tmp = new bool[this->size];
			memset( tmp, 0, sizeof(bool)*this->size);

			// copy current data.
			if (copy)
			{
				// Copy current data.
				memcpy( tmp, this->validEdge, this->size*sizeof(bool));
			}

			// Deallocate data.
			delete[] this->validEdge;

			// Reference new data.
			this->validEdge = tmp;

#ifdef DEBUG_GABRIEL_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Resizing Gabriel. New size is ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Info);
#endif
			this->valid = true;
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

	return(this->valid);
}


