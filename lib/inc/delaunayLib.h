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
/**
 * @fn              getStarTriangulation
 * @brief           Returns the dcel associated to a star triangulation
 *
 * @param vPoints   (IN) Triangulation number of points
 * @param dcel      (OUT) Dcel associated to star triangulation
 * @return          true if triangulation built successfully
 *                  false otherwise
 */
bool getStarTriangulation(const vector<TYPE> &vPoints, Dcel &dcel);


/**
 * @fn              getDelaunay
 * @brief           Returns the dcel associated to a Delaunay triangulation
 *
 * @param vPoints   (IN) Triangulation number of points
 * @param dcel      (OUT) Dcel associated to Delunay triangulation
 * @return          true if triangulation built successfully
 *                  false otherwise
 */
bool getDelaunay(const vector<TYPE> &vPoints, Dcel &dcel);


bool getVoronoi(const vector<TYPE> &vPoints, Dcel &dcel);


bool getConvexHull(const Dcel &dcel, vector<TYPE> &vPoints);



#endif //DELAUNAY_DELAUNAYLIB_H
