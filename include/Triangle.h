/*
 * Triangle.h
 *
 *  Created on: Jul 16, 2016
 *      Author: juan
 */

#ifndef INCLUDE_TRIANGLE_H_
#define INCLUDE_TRIANGLE_H_

#include "Point.h"

/****************************************************************************
// 						TRIANLGE CLASS DEFITNION
****************************************************************************/
class Triangle
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	Point<TYPE>	vertex[NPOINTS_TRIANGLE];
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Triangle(Point <TYPE> p, Point <TYPE> q, Point <TYPE> r);
	Triangle(TYPE x1, TYPE y1, TYPE x2, TYPE y2, TYPE x3, TYPE y3);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	double 	area();
	double 	perimeter();
	void	centroid(Point<TYPE> &centroid);
	static void	centroid(Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r,
														Point<TYPE> &centroid);

	void	print();
};

#endif /* INCLUDE_TRIANGLE_H_ */
