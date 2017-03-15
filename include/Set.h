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

#include <omp.h>
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
	Set() : nElements(0), size(DEFAULT_SIZE)
			{this->data = new T[size];};
	Set(int size) : nElements(0), size(size)
			{this->data = new T[size];};
	~Set();

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	bool isEmpty() { return(this->nElements == 0); };
	bool isFull() { return(this->nElements == this->size); };
	void add(T element);
	void update(int index, T element);
	T 	 *at(int index);

	int  getSize();
	int  getNElements();

	void resize(int size, bool copy);
	void reset();

	bool read(string fileName);
	bool write(string fileName);
	void print();
	void random(int nPoints);
	void shake();

	int getIndexHighest();
	T 	getHighest();

	bool  	operator==(Set<T> &other);
	Set<T>& operator++(int inc);
};

template <class T> Set<T>::~Set()
{
	// Deallocate data.
	if (this->data != NULL)
	{
		delete[] this->data;
	}
}

/***************************************************************************
* Name: 	add
* IN:		element			element to add
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds a new element to the set.
***************************************************************************/
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

/***************************************************************************
* Name: 	update
* IN:		index			index to update.
* 			element			element to updat
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Updates the element at position index with element data.
***************************************************************************/
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

/***************************************************************************
* Name: 	at
* IN:		index			index of the element to recover
* OUT:		NONE
* RETURN:	returns a reference to the element at index position
* GLOBAL:	NONE
* Description: 	returns a reference to the element at index position
***************************************************************************/
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

/***************************************************************************
* Name: 	getNElements
* IN:		NONE
* OUT:		NONE
* RETURN:	the size of the set
* GLOBAL:	NONE
* Description: 	returns the size of elements of the set
***************************************************************************/
template <class T> int Set<T>::getSize()
{
	return(this->size);
}

/***************************************************************************
* Name: 	getNElements
* IN:		NONE
* OUT:		NONE
* RETURN:	the number of elements of the set
* GLOBAL:	NONE
* Description: 	returns the number of elements of the set
***************************************************************************/
template <class T> int Set<T>::getNElements()
{
	return(this->nElements);
}

/***************************************************************************
* Name: 	resize
* IN:		size			new size
* 			copy			flag to copy current data to new location
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	resizes the set and copy the existing data to the new
* 				location if the input copy parameter is true. The new size
* 				will be the size input parameter, but if this value is
* 				-1 then the size is doubled.
***************************************************************************/
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

/***************************************************************************
* Name: 	read
* IN:		fileName		input file name
* OUT:		NONE
* RETURN:	true			if file written
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	reads the set of elements from file named "fileName".
***************************************************************************/
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

/***************************************************************************
* Name: 	write
* IN:		fileName		output file name
* OUT:		NONE
* RETURN:	true			if file written
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	writes the set of elements to an output file named "fileName".
***************************************************************************/
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

/***************************************************************************
* Name: 	print
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints all the elements of the set.
***************************************************************************/
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

/***************************************************************************
* Name: 	random
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	generates a random set of nPoints.
***************************************************************************/
template <class T> void Set<T>::random(int nPoints)
{
	int	i=0;			// Loop counter.

    // Generate random set of elements.
	#pragma omp parallel for default(none) private(i) shared(nPoints)
    for (i=0; i<nPoints ;i++)
    {
    	data[i].random();
    }
}

/***************************************************************************
* Name: 	shake
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	shake all the points off the set adding random delta values
* 				to all the points of the set.
***************************************************************************/
template <class T> void Set<T>::shake()
{
	int	i=0;			// Loop counter.
	int nElements=0;	// Loop upper bound.

    // Shake points.
	nElements = this->getNElements();
	#pragma omp parallel for default(none) private(i)
    for (i=0; i<nElements ;i++)
    {
    	data[this->nElements].shake();
    }
}

/***************************************************************************
* Name: 	getIndexHighest
* IN:		NONE
* OUT:		NONE
* RETURN:	index of the highest element in the set.
* GLOBAL:	NONE
* Description: 	returns index of the highest element of the set
***************************************************************************/
template <class T> int Set<T>::getIndexHighest()
{
	int i=0;			// Loop counter.
	int nElements=0;	// Loop upper bound.
	int *indexes;
	int bestIndex=0;	// Return value.
	int	nThreads=0;

	nThreads = omp_get_max_threads();
	indexes = new int[nThreads];
	memset(indexes, 0, sizeof(int)*nThreads);

	// Main loop.
	nElements = this->getNElements();
	#pragma omp parallel for default(none) private(i) shared(nElements, indexes)
	for (i=0; i<nElements ;i++)
	{
		// Check if new element is higher than current.
		if (this->data[i] > this->data[indexes[omp_get_thread_num()]])
		{
			// Update current highest value.
			indexes[omp_get_thread_num()] = i;
		}
	}

	bestIndex = indexes[0];
	for (i=1; i<nThreads ;i++)
	{
		if (this->data[bestIndex] < this->data[indexes[i]])
		{
			bestIndex = indexes[i];
		}
	}

	// Deallocate memory.
	delete[] indexes;

	return(bestIndex);
}

/***************************************************************************
* Name: 	getHighest
* IN:		NONE
* OUT:		NONE
* RETURN:	highest element in the set.
* GLOBAL:	NONE
* Description: 	returns the highest element of the set
***************************************************************************/
template <class T> T Set<T>::getHighest()
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
* Description: 	checks if both sets are equals.
***************************************************************************/
template <class T> bool Set<T>::operator==(Set<T> &other)
{
	bool equal=false;		// Return value.
	int	i=0;				// Loop counter.
	int nElements=0;		// Loop upper bound.

	if (this->getNElements() == other.getNElements())
	{
		equal = true;
		nElements = this->getNElements();
		#pragma omp parallel for default(none), private (i), shared(nElements, other), reduction(&:equal)
		for (i=0; i<nElements ;i++)
		{
			equal = (*this->at(i) == *other.at(i));
			i++;
		}
	}

	return (equal);
}

/***************************************************************************
* Name: 	++
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	adds 1 to all elements of the set.
***************************************************************************/
template <class T> Set<T>& Set<T>::operator++(int inc)
{
	int	i=0;				// Loop counter.
	int nElements=0;		// Loop upper bound.

	nElements = this->getNElements();
	#pragma omp parallel for default(none), private (i), shared(nElements)
	for (i=0; i<nElements ;i++)
	{
		this->data[i]++;
	}

    return *this;
}

#endif /* INCLUDE_SET_H_ */
