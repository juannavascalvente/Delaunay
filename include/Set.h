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

#ifdef _OPENMP
#include <omp.h>
#endif
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
template <class T>
class Set
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int	nElements;		// # elements in the set.
	int	size;			// Allocated size of set.
	T	*data;			// Set elements vector.

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Set();
	Set(int size);
	~Set();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool isEmpty() { return(this->nElements == 0); };
	bool isFull() { return(this->nElements == this->size); };
	void add(T element);
	void update(int index, T element);
	T *at(int index);

	int getSize();
	int getNElements();

	void resize(int size, bool copy);
	void reset();

	bool read(string fileName);
	bool write(string fileName);
	void print();
	int	 random(int nPoints);
	void shake();

	int highestIndex();
	T highest();
	bool  operator==(Set<T> &other);
};

template <class T> Set<T>::Set()
{
	// Initialize size attributes.
	this->size = 0;
	this->nElements = 0;

	// Allocate data.
	this->data = NULL;
}

template <class T> Set<T>::Set(int size)
{
	// Initialize size attributes.
	this->size = size;
	this->nElements = 0;

	// Allocate data.
	this->data = new T[size];
}

template <class T> Set<T>::~Set()
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

template <class T> void Set<T>::add(T element)
{
	// Check if set is full.
	if (this->isFull())
	{
#ifdef DEBUG_ADD_SET
		Logging::buildText(__FILE__, __FUNCTION__,"Resizing set.");
		Logging::write(true, Info);
#endif
		// Resize vector.
		this->resize(INVALID, true);
	}

#ifdef DEBUG_ADD_SET
	Logging::buildText(__FILE__, __FUNCTION__, "Adding element to set ");
	Logging::buildText(__FILE__, __FUNCTION__, this->nElements+1);
	Logging::buildText(__FILE__, __FUNCTION__, "/");
	Logging::buildText(__FILE__, __FUNCTION__, this->size);
	Logging::write(true, Info);
#endif
	// Insert element.
	this->data[this->nElements] = element;
	this->nElements++;
}

template <class T> void Set<T>::update(int index, T element)
{
	// Check if index out of bounds.
	if (index < this->nElements)
	{
		this->data[index] = element;
	}
	else
	{
		// PENDING what exception to throw?.
		throw;
	}
}


template <class T> T* Set<T>::at(int index)
{
	T* ret;		// Return value.

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

template <class T> int Set<T>::getSize()
{
	return(this->size);
}

template <class T> int Set<T>::getNElements()
{
	return(this->nElements);
}

template <class T> void Set<T>::resize(int size, bool copy)
{
	T *tmp; // Temporary vector.

	try
	{
		// Double set size.
		if (size == INVALID)
		{
#ifdef DEBUG_RESIZE_SET
			Logging::buildText(__FILE__, __FUNCTION__, "Doubling size from ");
			Logging::buildText(__FILE__, __FUNCTION__, this->size);
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
			Logging::buildText(__FILE__, __FUNCTION__, " to ");
			Logging::buildText(__FILE__, __FUNCTION__, this->size);
			Logging::write(true, Info);
#endif
		}
		else
		{
#ifdef DEBUG_RESIZE_SET
			Logging::buildText(__FILE__, __FUNCTION__, "Setting new size from ");
			Logging::buildText(__FILE__, __FUNCTION__, this->size);
			Logging::buildText(__FILE__, __FUNCTION__, " to ");
			Logging::buildText(__FILE__, __FUNCTION__, size);
			Logging::write(true, Info);
#endif
			this->size = size;
		}

		// Allocate new set.
		tmp = new T[this->size];

		// Check if copy required.
		if (copy)
		{
			// Copy current set elements.
			memcpy(tmp, this->data, sizeof(T)*this->nElements);
#ifdef DEBUG_RESIZE_SET
			Logging::buildText(__FILE__, __FUNCTION__, "Number of elements to copy ");
			Logging::buildText(__FILE__, __FUNCTION__, this->nElements);
			Logging::write(true, Info);
#endif
		}

		// Deallocate previous data.
		delete[] this->data;
		this->data = tmp;
#ifdef DEBUG_RESIZE_SET
		Logging::buildText(__FILE__, __FUNCTION__, "New size is ");
		Logging::buildText(__FILE__, __FUNCTION__, this->size);
		Logging::buildText(__FILE__, __FUNCTION__, " and number of elements is ");
		Logging::buildText(__FILE__, __FUNCTION__, this->nElements);
		Logging::write(true, Info);
#endif
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

template <class T> void Set<T>::reset()
{
	this->nElements = 0;
}

template <class T> bool Set<T>::read(string fileName)
{
    bool success=true;  	// Return value.
    ifstream ifs;    		// Input file.
    T	data;

    // Open file.
    ifs.open(fileName.c_str(), std::ifstream::in);

    // Check file is open.
    if (!ifs.is_open())
	{
	    // Error opening input file.
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening input file ");
		Logging::buildText(__FUNCTION__, __FILE__, fileName);
		Logging::write(true, Error);
		success = false;
	}
	else
	{
	    // Read elements from file.
		while (!ifs.eof())
		{
			ifs >> data;
			if (!ifs.eof())
			{
				this->add(data);
			}
		}

        // Close input file.
	    ifs.close();
	}

	return(success);
}

template <class T> bool Set<T>::write(string fileName)
{
	int		i=0;			// Loop counter.
	bool success=true;  	// Return value.
    ofstream ofs;    		// Output file.

    // Open file.
    ofs.open(fileName.c_str(), std::ifstream::out);

    // Check file is open.
    if (!ofs.is_open())
	{
	    // Error opening input file.
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening output file ");
		Logging::buildText(__FUNCTION__, __FILE__, fileName);
		Logging::write(true, Error);
		success = false;
	}
	else
	{
	    // Write set of points.
	    for (i=0; i<this->nElements ;i++)
        {
	    	ofs << this->data[i] << " ";
        }

        // Close output file.
	    ofs.close();
	}

	return(success);
}

template <class T> void Set<T>::print()
{
	int	i=0;			// Loop counter.

    // Print elements.
    for (i=0; i<this->nElements ;i++)
    {
    	cout << data[i] << " ";
    }
    cout << endl;
}

template <class T> int Set<T>::random(int nPoints)
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
template <class T> void Set<T>::shake()
{
	int	i=0;			// Loop counter.

    // Shake points.
    for (i=0; i<this->nElements ;i++)
    {
    	data[this->nElements].shake();
    }
}

template <class T> int Set<T>::highestIndex()
{
	int	i=0;				// Loop counter.
	int	index=0;			// Highest value index.

	// Main loop.
    for (i=1; i<this->nElements ;i++)
    {
		// Check if new element is higher than current.
		if (this->data[i] > this->data[index])
		{
			// Update current highest value.
			index = i;
		}
    }

	return(index);
}

template <class T> T Set<T>::highest()
{
	int	i=0;				// Loop counter.
	int highestValue=0;

	// Main loop.
	highestValue = this->data[0];
	#pragma omp parallel for default(none) private(i) reduction(max: highestValue)
    for (i=1; i<this->nElements ;i++)
    {
		// Check if new element is higher than current.
		if (this->data[i] > highestValue)
		{
			// Update current highest value.
			highestValue = this->data[i];
		}
    }

	return(highestValue);
}

/***************************************************************************
* Name: 	==
* IN:		v			vertex to compare to
* OUT:		NONE
* RETURN:	true 		if both vertex are equal
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	checks ifboth vertex are equals.
***************************************************************************/
template <class T> bool Set<T>::operator==(Set<T> &other)
{
	bool equal=false;		// Return value.
	int	i=0;				// Loop counter.

	if (this->getNElements() == other.getNElements())
	{
		equal = true;
		i = 0;
		while ((i<this->getNElements()) && equal)
		{
			equal = (*this->at(i) == *other.at(i));
			i++;
		}
	}

	return (equal);
}

#endif /* INCLUDE_SET_H_ */
