#ifndef DELAUNAY_DELAUNAYLIB_H
#define DELAUNAY_DELAUNAYLIB_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "Dcel.h"
#include "Delaunay.h"
#include "Voronoi.h"

/***********************************************************************************************************************
* API function declarations
***********************************************************************************************************************/
bool starTriangulation(const vector<TYPE> &vPoints, Dcel &dcel);


bool delaunay(const vector<TYPE> &vPoints, Delaunay &delaunay);


bool voronoi(const vector<TYPE> &vPoints, Voronoi &voronoi);


bool convexHull(const Dcel &dcel, vector<TYPE> &vPoints);



#endif //DELAUNAY_DELAUNAYLIB_H
