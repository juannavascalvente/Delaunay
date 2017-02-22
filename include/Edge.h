/*
 * Edge.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <iostream>
using namespace std;

/****************************************************************************
//	 						EDGE CLASS DEFITNION
****************************************************************************/
class Edge
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int	origin;
	int	twin;
	int	previous;
	int	next;
	int	face;
public:

	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Edge();
	Edge(int origin, int twin, int prev, int next, int face);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void set(int origin, int twin, int	previous, int next, int face);
	void reset();

	// Get / Set.
	inline void setOrigin(int value) { this->origin = value; };
	inline void setTwin(int value) { this->twin = value; };
	inline void setPrevious(int value) { this->previous = value; };
	inline void setNext(int value) { this->next = value; };
	inline void setFace(int value) { this->face = value; };

	inline int getOrigin() { return(this->origin); };
	inline int getTwin() { return(this->twin); };
	inline int getPrevious() { return(this->previous); };
	inline int getNext() { return(this->next); };
	inline int getFace() { return(this->face); };

	// Operators.
	friend istream& operator>>(istream &in, Edge &edge)
	{
		// Read edge data.
		in >> edge.origin;
		in >> edge.twin;
		in >> edge.previous;
		in >> edge.next;
		in >> edge.face;
		return(in);
	}
	friend ostream& operator<<(ostream &out, Edge &edge)
	{
		// Write edge data.
		out << edge.origin << " ";
		out << edge.twin << " ";
		out << edge.previous << " ";
		out << edge.next << " ";
		out << edge.face << " " << std::endl;
		return(out);
	}
	// Operators.
	Edge& operator=(Edge other)
	{
		this->origin = other.origin;
		this->twin = other.twin;
		this->previous = other.previous;
		this->next = other.next;
		this->face = other.face;
		return(*this);
	};
	bool  operator==(const Edge &other) const
	{
		return ((this->origin == other.origin) &&
				(this->twin == other.twin) &&
				(this->previous == other.previous) &&
				(this->next == other.next) &&
				(this->face == other.face) &&
				(this->origin == other.origin));
	}

	// I/O operations.
	void print(std::ostream& out) const;
	string toStr();
};


#endif /* EDGE_H_ */
