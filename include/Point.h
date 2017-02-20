/*
 * Point.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef POINT_H_
#define POINT_H_

#include "defines.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include <math.h>

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_POINT_CHECK_TURN
#endif

//****************************************************************************
// 								DEFINES
//****************************************************************************
#define 	COLLINEAR_THRESHOLD		0.0

// Window size
#define MAX_X_COORD				10000
#define MAX_Y_COORD				10000

//****************************************************************************
// 							Data types definition
//****************************************************************************
// Type of turn of three points line.
enum Turn_T {COLLINEAR=1, LEFT_TURN=2, RIGHT_TURN=3};

//****************************************************************************
//                           	POINT CLASS
//****************************************************************************
template <class A_Type>
class Point
{
	//***********************************************************
	// ATTRIBUTES
	//***********************************************************
	A_Type		x;		// X coordinate
	A_Type		y;		// Y coordinate

	//***********************************************************
	// MEMBERS
	//***********************************************************
	public:

	//***********************************************************
	// Constructor/Destructor.
	//***********************************************************
	Point() { this->x = 0.0; this->y = 0.0;}
	Point(A_Type x, A_Type y) { this->x = x; this->y = y;}

	// Set/Get operations.
	inline void 	setX(const A_Type x) { this->x = x;};
	inline void 	setY(const A_Type y) { this->y = y;};
	inline A_Type 	getX() {return(this->x);};
	inline A_Type 	getY() {return(this->y);};
	inline void 	setOrigin() { this->x = 0.0; this->y = 0.0;};

	//***********************************************************
	// Interface.
	//***********************************************************
	// PENGIND IS THIS CODE USED???
	// Comparison interface.
	/*
	int	        higher_Point(struct Point_T *p1, struct Point_T *p2, int (*f)(struct Point_T *, struct Point_T *));
	int			lower_X(struct Point_T *p1, struct Point_T *p2);
	int			higher_X(struct Point_T *p1, struct Point_T *p2);
	int			higher_Y(struct Point_T *p1, struct Point_T *p2);
	int         lexicographic_Higher(struct Point_T *p1, struct Point_T *p2);

	// Location interface.
	//bool in_Circle(const Point *p1, const Point * p2, const Point * p3);
	bool		interior_Triangle(struct Point_T *p1, struct Point_T *p2, struct Point_T *p3, struct Point_T *q);
	*/

	// Distance.
	A_Type distance(const Point &p);
	double signedArea(const Point &p, const Point &q);

	// Location interface.
	bool has_Extreme_Coordinates(void);
	enum Turn_T	check_Turn(Point<A_Type> &p, Point<A_Type> &q);
	static bool lowerY(Point<TYPE> *p, Point<TYPE> *q);
	static bool lexicographicHigher(Point<TYPE> *p, Point<TYPE> *q);
	static bool higher_Point(Point<TYPE> *p1, Point<TYPE> *p2, bool (*f)(Point<TYPE> *, Point<TYPE> *));
	static bool inCircle(Point<TYPE> *p1, Point<TYPE> *p2, Point<TYPE> *q1, Point<TYPE> *q2);
	static void middlePoint(Point<TYPE> *p, Point<TYPE> *q, Point<TYPE> *middle);

	void random();
	void shake();

	// I/O interface
	void print(std::ostream& out);
	string toStr();
	void printFormatted(std::ostream& out);
	void read(ifstream &ifs);
	void write(ofstream &ofs);

	// Operators.
	inline Point& 	operator+(const Point& q){ this->x += q.x; this->y += q.y; return(*this); }
	inline Point& 	operator-(const Point& q){ this->x -= q.x; this->y -= q.y; return(*this); }
	inline Point& 	operator*(const TYPE value){ this->x *= value; this->y *= value; return(*this); }
	inline Point& 	operator/(const TYPE value){ this->x /= value; this->y /= value; return(*this); }
	inline bool 	operator==(const Point& q){ return ((this->x == q->x) && (this->y == q->y)); }
	inline bool 	operator!=(const Point& q){ return ((this->x != q->x) || (this->y != q->y)); }
	inline Point& 	operator=(const Point& other ) { this->x = other.x; this->y = other.y; return(*this);}
	friend istream &operator>>(istream &in, Point &p) { in >> p.x; in >> p.y; return(in); };
	friend ostream &operator<<(ostream &out, Point &p) { out << p.x; out << " "; out << p.y; return(out); };

	inline void shift(TYPE deltaX, TYPE deltaY) { this->x += deltaX; this->y += deltaY; };
};

/*****************************************************************************
 * Name: 		distance
 * Input: 		point p
 * Description: computes the Euclidean distance from p to q.
 * Output: 		Euclidean distance from point p to point q
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> A_Type Point<A_Type>::distance(const Point &p)
{
	 A_Type	dist=0.0;			// Return value.

	 // Compute Euclidean distance.
	dist = sqrt(pow(this->x - p.x, 2) + pow(this->y - p.y, 2));

	return(dist);
}

/*****************************************************************************
 * Name: 		signedArea
 * Input: 		Point p, Point q
 * Description: computes the signed area enclosed by 3 points (self, p, q)
 * Output: 		signed area enclosed by 3 points (self, p, q)
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> double Point<A_Type>::signedArea(const Point &p, const Point &q)
{
	double area=0.0;	// Return value.

	// Compute signed area.
	area = - (((double) p.x)*((double) this->y)) + (((double) q.x)*((double) this->y))
		   + (((double) this->x)*((double) p.y)) - (((double) q.x)*((double) p.y))
		   - (((double) this->x)*((double) q.y)) + (((double) p.x)*((double) q.y));

	return(area);
}

/*****************************************************************************
 * Name: 		has_Extreme_Coordinates
 * Input: 		NONE
 * Description: Checks if points is out of maximum coordinates.
 * Output: 	True if out of bounds. False otherwise.
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> bool Point<A_Type>::has_Extreme_Coordinates(void)
{
	bool	hasExtreme=false;			// Return value.

	// Check if point out of bounds.
	if ((this->x >= MAX_X_COORD) || (this->y >= MAX_Y_COORD) ||
		(this->x <= -MAX_X_COORD) || (this->y <= -MAX_Y_COORD))
	{
		hasExtreme = true;
	}

	return(hasExtreme);
}

/*****************************************************************************
 * Name: 		check_Turn
 * Input: 		NONE
 * Description: Checks if points is out of maximum coordinates.
 * Output: 	True if out of bounds. False otherwise.
 * Complexity:	O(1)
*****************************************************************************/
// PENDING CHANGE PARAMETERS TO BE reference.
template <class A_Type> enum Turn_T Point<A_Type>::check_Turn(Point<A_Type> &p, Point<A_Type> &q)
{
	double          area=0.0;       // Signed area.
	enum Turn_T     turn;           // Return value.

	// Compute signed area of the triangle formed by p1, p2 and p3.
	//area = signed_Area(p1, p2, p3);
	area = this->signedArea(p, q);

#ifdef DEBUG_POINT_CHECK_TURN
	std::cout << "Area is " << area << std::endl;
#endif

	// Higher than zero -> turn left.
	if (area > COLLINEAR_THRESHOLD)
	{
#ifdef DEBUG_POINT_CHECK_TURN
		std::cout << "LEFT" << std::endl;
#endif
		turn = LEFT_TURN;
	}
	// Lower than zero -> turn right.
	else if (area < -COLLINEAR_THRESHOLD)
	{
#ifdef DEBUG_POINT_CHECK_TURN
		std::cout << "RIGHT" << std::endl;
#endif
		turn = RIGHT_TURN;
	}
	// If area is close to zero then points are collinear.
	else
	{
#ifdef DEBUG_POINT_CHECK_TURN
		std::cout << "COLLINEAR" << std::endl;
#endif
		turn = COLLINEAR;
	}

	return(turn);
}

/*****************************************************************************
 * Name: 		random
 * Input: 		NONE
 * Description: Generate random x and y coordinates.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::random(void)
{
	// Generate random coordinates.
	this->x = (A_Type) drand48()*MAX_X_COORD;
	this->y = (A_Type) drand48()*MAX_Y_COORD;
}

/*****************************************************************************
 * Name: 		shake
 * Input: 		NONE
 * Description: Shake random x and y coordinates.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::shake(void)
{
	A_Type delta=0.0;

	// Generate random coordinates.
	delta = (double)(rand() % 10);
	delta = delta / 10.0;
	this->x = this->x + (A_Type) delta;
	delta = (double)(rand() % 10);
	delta = delta / 10.0;
	this->y = this->y + (A_Type) delta;
}

/*****************************************************************************
 * Name: 		print
 * Input: 		NONE
 * Description: print x and y coordinates.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::print(std::ostream& out)
{
	// Print coordinates.
	out << this->x << " " << this->y << " ";
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	point information as a string
* GLOBAL:	NONE
* Description: 	concat the point information as a string
***************************************************************************/
template <class A_Type> string Point<A_Type>::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print(oss);
	text = oss.str();

	return(text);
}

/*****************************************************************************
 * Name: 		printFormatted
 * Input: 		NONE
 * Description: print x and y coordinates between parenthesis.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::printFormatted(std::ostream& out)
{
	// Print coordinates.
	out << "(" << this->x << "," << this->y << ")";
}

/*****************************************************************************
 * Name: 		read
 * Input: 		ifs		input file stream.
 * Description: reads x and y coordinates.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::read(ifstream &ifs)
{
	A_Type value;

	try
	{
		// Read X and Y coordinate.
		ifs >> value;
		//this->setX(value);
		ifs >> value;
		//this->setY(value);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

/*****************************************************************************
 * Name: 		write
 * Input: 		ofs		output file stream.
 * Description: writes x and y coordinates.
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::write(ofstream &ofs)
{
	try
	{
		// Read X and Y coordinate.
		ofs << this->x << " " << this->y;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

/*****************************************************************************
 * Name: 		lowerY
 * Input: 		p		first point
 * 				q 		second point
 * Output:
 * Return:		true if p has lower Y coordinate than q
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> bool Point<A_Type>::lowerY(Point<TYPE> *p, Point<TYPE> *q)
{
    bool lower=false;       // Return value.

    // Check if Y coordinate is lower.
    if (p->getY() < q->getY())
    {
    	lower = true;
    }

    return(lower);
}

/*****************************************************************************
 * Name: 		lexicographic_Higher
 * Input: 		NONE
 * Output:
 * Return:		true if
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> bool Point<A_Type>::lexicographicHigher(Point<TYPE> *p, Point<TYPE> *q)
{
    bool higher=false;       // Return value.

    // Check if Y coordinate is higher.
    if (p->getY() > q->getY())
    {
        higher = true;
    }
    else
    {
        // If Y coordinate is equal then check X coordinate.
        if ((p->getY() == q->getY()) &&
            (p->getX() > q->getX()))
        {
            higher = true;
        }
    }

    return(higher);
}

/*****************************************************************************
 * Name: 		lexicographic_Higher
 * Input: 		NONE
 * Output:
 * Return:		true if
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> bool Point<A_Type>::higher_Point(Point<TYPE> *p1,
					Point<TYPE> *p2, bool (*f)(Point<TYPE> *, Point<TYPE> *))
{
    return((*f)(p1, p2));
}

/*****************************************************************************
 * Name: 		inCircle
 * Input: 		p1			pointer to 1st point that defines circle
 * 				p2			pointer to 2nd point that defines circle
 * 				p3			pointer to 3rd point that defines circle
 * 				q			pointer to point to check
 * Output:
 * Return:		true if "q" is in circle defined by p1-p2-p3
 * 				false otherwise
 * Output: 		NONE
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> bool Point<A_Type>::inCircle(Point<TYPE> *p1,
						Point<TYPE> *p2, Point<TYPE> *p3, Point<TYPE> *q)
{
	bool inCircle=false;	// Return value.
	TYPE value=0.0;			// Determinant value.
	TYPE temp[9];			// Intermediate values.

	// Compute Ax - Dx, Ay - Dy and (Ax-Dx)² + (Ay-Dy)²
	temp[0] = (p1->x - q->x);
	temp[1] = (p1->y - q->y);
	temp[2] = (POINT_T) (pow((p1->x - q->x), 2) + pow((p1->y - q->y), 2));

	// Compute Bx - Dx, By - Dy and (Bx-Dx)² + (By-Dy)²
	temp[3] = (p2->x - q->x);
	temp[4] = (p2->y - q->y);
	temp[5] = (POINT_T) (pow((p2->x - q->x), 2) + pow((p2->y - q->y), 2));

	// Compute Cx - Dx, Cy - Dy and (Cx-Dx)² + (Cy-Dy)²
	temp[6] = (p3->x - q->x);
	temp[7] = (p3->y - q->y);
	temp[8] = (POINT_T) (pow((p3->x - q->x), 2) + pow((p3->y - q->y), 2));

	// Compute determinant.
	value = (temp[0]*temp[4]*temp[8]) + (temp[1]*temp[5]*temp[6]) +
			(temp[2]*temp[3]*temp[7]) - (temp[2]*temp[4]*temp[6]) -
			(temp[5]*temp[7]*temp[0]) -	(temp[8]*temp[1]*temp[3]);

	// If positive then point "q" belongs to p1-p2-p3 circumference.
	if (value > 0.0)
	{
		inCircle = true;
	}

	return(inCircle);
}

/*****************************************************************************
 * Name: 		middlePoint
 * Input: 		p			pointer to 1st point
 * 				q			pointer to 2nd point
 * Output:		middle		pointer to middle point
 * Return:		NONE
 * Output: 		NONE
 * Description:	Computes the middle point of the segment formed by p-q
 * Complexity:	O(1)
*****************************************************************************/
template <class A_Type> void Point<A_Type>::middlePoint(Point<TYPE> *p,
										Point<TYPE> *q, Point<TYPE> *middle)
{
    // Compute middle point of edge.
    middle->x = (p->x + q->x) / (TYPE) 2.0;
    middle->y = (p->y + q->y) / (TYPE) 2.0;
}

#endif /* POINT_H_ */
