/*
 * Stack.h
 *
 *  Created on: Jul 22, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_STACK_H_
#define INCLUDE_STACK_H_

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_PEAK_STACK
//#define DEBUG_ELEMENT_AT_STACK
//#define DEBUG_STACK_RESIZE
//#define DEBUG_UPDATE_PEAK_STACK
//#define DEBUG_PUSH_STACK
//#define DEBUG_POP_STACK
#endif

#include "string.h"

//*****************************************************************************
//                           	STACK CLASS
//*****************************************************************************
template <class A_Type>
class Stack
{
	//***********************************************************
	// ATTRIBUTES
	//***********************************************************
	int			size;			// Max # elements in stack.
	int			nElements;		// Current # elements in stack.
	A_Type 		*data;			// Elements array.

	//***********************************************************
	// MEMBERS
	//***********************************************************
	public:

	//***********************************************************
	// Constructor/Destructor.
	//***********************************************************
	Stack(int s);
	~Stack();

	//***********************************************************
	// Interface.
	//***********************************************************
	A_Type 	peak();
	A_Type 	elementAt( int index);
	void 	updatePeak(A_Type value);
	void 	push(A_Type value);
	A_Type 	pop();
	int 	getLength();
	int 	getSize();
	void 	reset();

	void	resize( int size, bool copy);
};

// Stack class definition.
// Constructor.
template <class A_Type> Stack<A_Type>::Stack(int s)
{
	// Set size and # elements in stack.
	this->size = s;
	this->nElements = 0;

	// Allocate array.
	this->data = new A_Type[s];
}

// Destructor.
template <class A_Type> Stack<A_Type>::~Stack()
{
	// Deallocate array.
	delete[] this->data;
}

// Return peak value.
template <class A_Type> A_Type Stack<A_Type>::peak()
{
	A_Type ret;			// Return value.

	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (this->nElements > 0)
	{
		// Get peak element.
		ret = this->data[this->nElements-1];
	}
#ifdef DEBUG_PEAK_STACK
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Trying to update peak but stack is empty.");
		Logging::write( true, Error);
	}
#endif

	return(ret);
}

// Return element at position index.
template <class A_Type> A_Type Stack<A_Type>::elementAt(int index)
{
	A_Type ret;			// Return value.

	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (index <= this->nElements)
	{
		// Get peak element.
		ret = this->data[this->nElements - index];
	}
#ifdef DEBUG_ELEMENT_AT_STACK
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Accessing to index ");
		Logging::buildText(__FUNCTION__, __FILE__, index);
		Logging::buildText(__FUNCTION__, __FILE__, " but stack has ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
		Logging::write( true, Error);
	}
#endif

	return(ret);
}

// Set peak value without inserting a new value.
template <class A_Type> void Stack<A_Type>::updatePeak(A_Type value)
{
	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (this->nElements > 0)
	{
		// Set peak value.
		this->data[this->nElements-1] = value;
	}
#ifdef DEBUG_UPDATE_PEAK_STACK
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Trying to update peak but stack is empty.");
		Logging::write( true, Error);
	}
#endif
}

// Add a new element at top of the stack.
template <class A_Type> void Stack<A_Type>::push(A_Type value)
{
	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (this->nElements == this->size)
	{
		this->resize( INVALID, true);
	}

	// Insert a new element in the top of the stack.
	this->data[this->nElements] = value;
	this->nElements++;
#ifdef DEBUG_PUSH_STACK
	Logging::buildText(__FUNCTION__, __FILE__, "Pushed element. # elements is : ");
	Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
	Logging::write( true, Info);
#endif
}

// Return peak element and remove it from the stack.
template <class A_Type> A_Type Stack<A_Type>::pop()
{
	A_Type ret;		// Return value.

	// PENDING WHAT TO IF OUT OF BOUNDS.
	if (this->nElements > 0)
	{
		// Decrease number of elements.
		this->nElements--;

		// Get current value.
		ret = this->data[this->nElements];
#ifdef DEBUG_POP_STACK
		Logging::buildText(__FUNCTION__, __FILE__, "Pop element. # elements is : ");
		Logging::buildText(__FUNCTION__, __FILE__, this->nElements);
		Logging::write( true, Info);
#endif
	}
#ifdef DEBUG_POP_STACK
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Trying to pop element but stack is empty.");
		Logging::write( true, Error);
	}
#endif

	return(ret);
}

// Return nElements value.
template <class A_Type> int Stack<A_Type>::getLength()
{
	// Return current # elements in stack.
	return(this->nElements);
}

// Return stack size.
template <class A_Type> int Stack<A_Type>::getSize()
{
	// Return stack size.
	return(this->size);
}

// Set stack as empty.
template <class A_Type> void Stack<A_Type>::reset()
{
	// Reset data of the stack (without resizing).
	this->nElements = 0;
	memset( this->data, 0, sizeof(A_Type)*this->size);
}


// Set stack as empty.
template <class A_Type> void Stack<A_Type>::resize( int size, bool copy)
{
	try
	{
		A_Type 	*tmp;

		// Check if current content must be copied.
		if (copy)
		{
			tmp = this->data;
		}
		// If not -> reset # elements.
		else
		{
			this->nElements = 0;
		}

		// New size unknown -> allocate twice current space.
		if (size == INVALID)
		{
			this->size = this->size*2;
		}
		else
		{
			this->size = size;
		}

		// PENDING What to do if size is negative?
		// Check if new size is negative.
		if (this->size < 0)
		{
			Logging::buildText(__FUNCTION__, __FILE__, "New stack size is negative: ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Error);
		}
		else
		{
			// Allocate new memory.
			tmp = new A_Type[this->size];

			// copy current data.
			if (copy)
			{
				// Copy current data.
				memcpy( tmp, this->data, this->nElements*sizeof(A_Type));
			}

			// Deallocate data.
			if (this->data != NULL)
			{
				delete[] this->data;
			}

			// Reference new data.
			this->data = tmp;

#ifdef DEBUG_STACK_RESIZE
			Logging::buildText(__FUNCTION__, __FILE__, "Resizing stack. New size is ");
			Logging::buildText(__FUNCTION__, __FILE__, this->size);
			Logging::write( true, Info);
#endif
		}
	}
	catch (bad_alloc &ex)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory. New stack size is ");
		Logging::buildText(__FUNCTION__, __FILE__, this->size);
		Logging::write( true, Error);
	}
	catch (exception &ex)
	{
		std::cout << ex.what();
	}
}

#endif /* INCLUDE_STACK_H_ */
