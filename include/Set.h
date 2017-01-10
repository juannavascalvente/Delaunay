/*
 * Set.h
 *
 *  Created on: Jul 5, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_SET_H_
#define INCLUDE_SET_H_

#include "defines.h"
#include "Logging.h"
#include <string.h>
#include <errno.h>
#include <iostream>
#include <fstream>
using namespace std;

#define DEFAULT_SIZE		10

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_RESIZE_SET
//#define DEBUG_SET
//#define DEBUG_ADD_SET
#endif

//****************************************************************************
//                           	SET CLASS
//****************************************************************************
template <class ElementType>
class Set
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int				nElements;		// # elements in the set.
	int				size;			// Allocated size of set.
	ElementType		*data;			// Set elements vector.

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Set();
	Set( int size);
	~Set();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool isEmpty() { return(this->nElements == 0); };
	bool isFull() { return(this->nElements == this->size); };
	void add(ElementType element);
	ElementType *at( int index);

	int getSize();
	int getNElements();

	void resize( int size, bool copy);
	void reset();

	int	 read( const char *fileName);
	int	 write( const char *fileName);
	void print();
	int	 random( int nPoints);
	void shake();
};

template <class ElementType> Set<ElementType>::Set()
{
	// Initialize size attributes.
	this->size = 0;
	this->nElements = 0;

	// Allocate data.
	this->data = NULL;
}

template <class ElementType> Set<ElementType>::Set( int size)
{
	// Initialize size attributes.
	this->size = size;
	this->nElements = 0;

	// Allocate data.
	this->data = new ElementType[size];
}

template <class ElementType> Set<ElementType>::~Set()
{
	// Reset attributes.
	this->size = 0;
	this->nElements = 0;

	// Deallocate data.
	if (this->data != NULL)
	{
		delete[] this->data;
	}
}

template <class ElementType> void Set<ElementType>::add(ElementType element)
{
	// Check if set is full.
	if (this->isFull())
	{
#ifdef DEBUG_ADD_SET
		Logging::buildText( __FILE__, __FUNCTION__,"Resizing set.");
		Logging::write( true, Info);
#endif
		// Resize vector.
		this->resize( INVALID, true);
	}

#ifdef DEBUG_ADD_SET
	Logging::buildText( __FILE__, __FUNCTION__, "Adding element to set.");
	Logging::write( true, Info);
#endif
	// Insert element.
	this->data[this->nElements] = element;
	this->nElements++;

#ifdef DEBUG_ADD_SET
	Logging::buildText( __FILE__, __FUNCTION__, "Element added. # points is ");
	Logging::buildText( __FILE__, __FUNCTION__, this->nElements);
	Logging::write( true, Info);
#endif
}

template <class ElementType> ElementType* Set<ElementType>::at( int index)
{
	ElementType* ret;		// Return value.

	// Check if index out of bounds.
	if (index < this->nElements)
	{
		ret = &this->data[index];
	}
	else
	{
		// PENDING what exception to throw?.
		throw;
	}

	return(ret);
}

template <class ElementType> int Set<ElementType>::getSize()
{
	return(this->size);
}

template <class ElementType> int Set<ElementType>::getNElements()
{
	return(this->nElements);
}

template <class ElementType> void Set<ElementType>::resize( int size, bool copy)
{
	ElementType	*tmp;		// Temporary vector.

	try
	{
		// Double set size.
		if (size == INVALID)
		{
#ifdef DEBUG_RESIZE_SET
			Logging::buildText( __FILE__, __FUNCTION__, "Doubling size from ");
			Logging::buildText( __FILE__, __FUNCTION__, this->size);
#endif
			if (this->size == 0)
			{
				this->size = DEFAULT_SIZE;
			}
			else
			{
				this->size = this->size*2;
			}
#ifdef DEBUG_RESIZE_SET
			Logging::buildText( __FILE__, __FUNCTION__, " to ");
			Logging::buildText( __FILE__, __FUNCTION__, this->size);
			Logging::write( true, Info);
#endif
		}
		else
		{
#ifdef DEBUG_RESIZE_SET
			Logging::buildText( __FILE__, __FUNCTION__, "Setting new size from ");
			Logging::buildText( __FILE__, __FUNCTION__, this->size);
			Logging::buildText( __FILE__, __FUNCTION__, " to ");
			Logging::buildText( __FILE__, __FUNCTION__, size);
			Logging::write( true, Info);
#endif
			this->size = size;
		}

		// Allocate new set.
		tmp = new ElementType[this->size];

		// Check if copy required.
		if (copy)
		{
			// Copy current set elements.
			memcpy( tmp, this->data, sizeof(ElementType)*this->nElements);
#ifdef DEBUG_RESIZE_SET
			Logging::buildText( __FILE__, __FUNCTION__, "Number of elements to copy ");
			Logging::buildText( __FILE__, __FUNCTION__, this->nElements);
			Logging::write( true, Info);
#endif
		}

		// Deallocate previous data.
		delete[] this->data;
		this->data = tmp;
#ifdef DEBUG_RESIZE_SET
		Logging::buildText( __FILE__, __FUNCTION__, "New size is ");
		Logging::buildText( __FILE__, __FUNCTION__, this->size);
		Logging::buildText( __FILE__, __FUNCTION__, " and number of elements is ");
		Logging::buildText( __FILE__, __FUNCTION__, this->nElements);
		Logging::write( true, Info);
#endif
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

template <class ElementType> void Set<ElementType>::reset()
{
	this->nElements = 0;
}

template <class ElementType> int Set<ElementType>::read( const char *fileName)
{
	int		i=0;			// Loop counter.
    int     ret=SUCCESS;  	// Return value.
    int     nPoints=0;    	// Number of points of set.
    ifstream inFile;    	// Input file.

    // Open file.
    inFile.open( fileName, std::ifstream::in);

    // Check file is open.
    if (!inFile.is_open())
	{
	    // Error opening input file.
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening input file ");
		Logging::buildText(__FUNCTION__, __FILE__, fileName);
		Logging::write( true, Error);
		ret = FAILURE;
	}
	else
	{
	    // Read number of points.
		inFile >> nPoints;

#ifdef DEBUG_READ_FLAT_FILE
	    printf("# points %d\n", number_Points);
#endif
			// Resize set.
	    if (this->size < nPoints)
	    {
	    	this->resize( nPoints, false);
	    }

	    // Read set of points.
	    for (i=0; i<nPoints ;i++)
        {
	    	data->read( inFile);
        }

        // Close input file.
	    inFile.close();
	}

	return(ret);
}

template <class ElementType> int Set<ElementType>::write( const char *fileName)
{
	int		i=0;			// Loop counter.
    int     ret=SUCCESS;  	// Return value.
    ifstream outFile;    	// Output file.

    // Open file.
    outFile.open( fileName, std::ifstream::out);

    // Check file is open.
    if (!outFile.is_open())
	{
	    // Error opening input file.
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening input file ");
		Logging::buildText(__FUNCTION__, __FILE__, fileName);
		Logging::write( true, Error);
    	ret = FAILURE;
	}
	else
	{
	    // Write number of points.
		outFile << this->nElements;

#ifdef DEBUG_READ_FLAT_FILE
	    	printf("# points %d\n", number_Points);
#endif
	    // Read set of points.
	    for (i=0; i<this->nElements ;i++)
        {
	    	data->write( outFile);
        }

        // Close output file.
	    outFile.close();
	}

	return(ret);
}

template <class ElementType> void Set<ElementType>::print()
{
	int	i=0;			// Loop counter.

    // Print elements.
    for (i=0; i<this->nElements ;i++)
    {
    	std::cout << data[i] << std::endl;
    }
}

template <class ElementType> int Set<ElementType>::random( int nPoints)
{
	int	i=0;			// Loop counter.
	int ret=SUCCESS;  	// Return value.

    // Generate random set of elements.
    for (i=0; i<this->nElements ;i++)
    {
    	data[this->nElements].random();
    	this->nElements++;
    }

	return(ret);
}

// PENDING: REMOVE TO POINTS.
template <class ElementType> void Set<ElementType>::shake( )
{
	int	i=0;			// Loop counter.

    // Shake points.
    for (i=0; i<this->nElements ;i++)
    {
    	data[this->nElements].shake();
    }
}

#endif /* INCLUDE_SET_H_ */
