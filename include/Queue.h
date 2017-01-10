/*
 * Queue.h
 *
 *  Created on: Nov 10, 2016
 *      Author: juan
 */

#ifndef INCLUDE_QUEUE_H_
#define INCLUDE_QUEUE_H_

#include "defines.h"
#include "Logging.h"
#include <string.h>

#define DEFAULT_QUEUE_SIZE	20

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_QUEUE_CONSTRUCTOR_DEFAULT
//#define DEBUG_QUEUE_DESTRUCTOR
//#define DEBUG_QUEUE_CONSTRUCTOR
//#define DEBUG_QUEUE_INSERT
//#define DEBUG_QUEUE_GET
//#define DEBUG_QUEUE_AT
//#define DEBUG_QUEUE_RESIZE
#endif

//*****************************************************************************
//                           	QUEUE CLASS
//*****************************************************************************
template <class A_Type>
class Queue
{
	//***********************************************************
	// ATTRIBUTES
	//***********************************************************
	int			in;				// Next free position.
	int			out;			// Next busy position.
	int			size;			// Max # elements in queue.
	int			nElements;		// Current # elements in queue.
	A_Type 		*data;			// Elements array.

public:

	//***********************************************************
	// Constructor/Destructor.
	//***********************************************************
	Queue();
	Queue(int s);
	~Queue();

	//***********************************************************
	// Interface.
	//***********************************************************
	void 	enqueue(A_Type value);
	A_Type 	dequeue();
	A_Type 	at(int index);
	int 	getNElements();
	int 	getSize();
	void 	reset();
	void	resize( int size, bool copy);
	void	print(std::ostream& out);
};

// Constructor.
template <class A_Type> Queue<A_Type>::Queue()
{
	// Set size and # elements in queue.
	this->size = 0;
	this->nElements = 0;
	this->in = 0;
	this->out = 0;

	// Allocate array.
	this->data = NULL;
#ifdef DEBUG_QUEUE_CONSTRUCTOR_DEFAULT
	Logging::buildText(__FUNCTION__, __FILE__, "Creating default queue.");
	Logging::write( true, Info);
#endif
}

// Constructor.
template <class A_Type> Queue<A_Type>::Queue(int s)
{
	// Set size and # elements in queue.
	this->size = s;
	this->nElements = 0;
	this->in = 0;
	this->out = 0;

	// Allocate array.
	this->data = new A_Type[this->size];
#ifdef DEBUG_QUEUE_CONSTRUCTOR
	Logging::buildText(__FUNCTION__, __FILE__, "Creating queue with length ");
	Logging::buildText(__FUNCTION__, __FILE__, this->size);
	Logging::write( true, Info);
#endif
}

// Destructor.
template <class A_Type> Queue<A_Type>::~Queue()
{
#ifdef DEBUG_QUEUE_DESTRUCTOR
	Logging::buildText(__FUNCTION__, __FILE__, "Default queue destructor.");
	Logging::write( true, Info);
#endif
	// Deallocate array.
	delete[] this->data;
}

// Add a new element at top of the queue.
template <class A_Type> void Queue<A_Type>::enqueue(A_Type value)
{
	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (this->nElements == this->size)
	{
#ifdef DEBUG_QUEUE_INSERT
		Logging::buildText(__FUNCTION__, __FILE__, "Resizing queue.");
		Logging::write( true, Info);
#endif
		this->resize( INVALID, true);
	}

#ifdef DEBUG_QUEUE_INSERT
	Logging::buildText(__FUNCTION__, __FILE__, "Inserting element at position ");
	Logging::buildText(__FUNCTION__, __FILE__, this->in);
	Logging::buildText(__FUNCTION__, __FILE__, " of ");
	Logging::buildText(__FUNCTION__, __FILE__, this->size);
	Logging::write( true, Info);
#endif

	// Update buffer attributes.
	this->data[this->in] = value;
	this->in++;
	this->in = this->in % this->size;
	this->nElements++;
}

// Get first element inserted.
template <class A_Type> A_Type Queue<A_Type>::dequeue()
{
	A_Type value;		// Return value.

	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (this->nElements > 0)
	{
#ifdef DEBUG_QUEUE_GET
		Logging::buildText(__FUNCTION__, __FILE__, "Get element from position: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->out);
		Logging::buildText(__FUNCTION__, __FILE__, " of ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
		Logging::write( true, Info);
#endif
		// Get element.
		value = this->data[this->out];

		// Update buffer attributes.
		this->out++;
		this->out = this->out % this->size;
		this->nElements--;
	}
#ifdef DEBUG_QUEUE_GET
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Trying to get element but queue is empty.");
		Logging::write( true, Error);
	}
#endif

	return(value);
}

// Return element at indeex position.
template <class A_Type> A_Type Queue<A_Type>::at(int index)
{
	A_Type value;		// Return value.
	int	pos=0;			// Position to get value.

	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (index < this->nElements)
	{
		pos = (this->out + index) % this->size;
#ifdef DEBUG_QUEUE_AT
		Logging::buildText(__FUNCTION__, __FILE__, "Get element at position ");
		Logging::buildText(__FUNCTION__, __FILE__, pos);
		Logging::buildText(__FUNCTION__, __FILE__, " of ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Info);
#endif
		// Get element.
		value = this->data[pos];
	}
#ifdef DEBUG_QUEUE_AT
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Trying to get element at index ");
		Logging::buildText(__FUNCTION__, __FILE__, index);
		Logging::buildText(__FUNCTION__, __FILE__, " but it has ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
		Logging::write( true, Error);
	}
#endif

	return(value);
}

// Return nElements value.
template <class A_Type> int Queue<A_Type>::getNElements()
{
	// Return current # elements in queue.
	return(this->nElements);
}

// Return queue size.
template <class A_Type> int Queue<A_Type>::getSize()
{
	// Return queue size.
	return(this->size);
}

// Set queue as empty.
template <class A_Type> void Queue<A_Type>::reset()
{
	// Reset data of the queue (without resizing).
	this->in = 0;
	this->out = 0;
	this->nElements = 0;
	memset( this->data, 0, sizeof(A_Type)*this->size);
}

// Resize queue to size.
template <class A_Type> void Queue<A_Type>::resize( int size, bool copy)
{
	int	newSize=0;		// New queue size.

	try
	{
		A_Type 	*tmp;

		// New size unknown -> allocate twice current space.
		if (size == INVALID)
		{
			newSize = this->size*2;
		}
		else
		{
			newSize = size;
		}

		// Allocate new memory.
		tmp = new A_Type[newSize];

#ifdef DEBUG_QUEUE_RESIZE
		Logging::buildText(__FUNCTION__, __FILE__, "New size is ");
		Logging::buildText(__FUNCTION__, __FILE__, newSize);
		Logging::write( true, Info);
#endif
		// PENDING What to do if size is negative?
		// Check if new size is negative.
		if (newSize <= 0)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "New queue size is not positive: ");
			Logging::buildText(__FUNCTION__, __FILE__, newSize);
			Logging::write( true, Error);
		}
		else
		{
			// copy current data.
			if (copy)
			{
#ifdef DEBUG_QUEUE_RESIZE
				Logging::buildText(__FUNCTION__, __FILE__, "Copying data.");
				Logging::write( true, Info);
#endif
				// Copy current data.
				memcpy( tmp, this->data, this->size*sizeof(A_Type));
				this->in = (this->out + this->nElements) % newSize;
			}
			else
			{
				this->nElements = 0;
				this->in = 0;
				this->out = 0;
			}

			// Deallocate data.
			if (this->data != NULL)
			{
				delete[] this->data;
			}

			// Reference new data.
			this->data = tmp;

			// Set new size.
			this->size = newSize;
#ifdef DEBUG_QUEUE_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Resizing queue. New size is ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Info);
#endif
		}
	}
	catch (std::bad_alloc &ex)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory. New queue size is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Error);
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what();
	}
}

template <class A_Type> void Queue<A_Type>::print(std::ostream& out)
{
	int	i=0;			// Loop counter.
	int	pos=0;			// Position in array.

	// Print all data.
	pos = this->out;
	for (i=0; i<this->getNElements(); i++)
	{
		out << this->data[pos] << std::endl;
		pos++;
		pos = pos % this->size;
	}
}

#endif /* INCLUDE_QUEUE_H_ */
