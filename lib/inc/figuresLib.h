#ifndef DELAUNAY_FIGURESLIB_H
#define DELAUNAY_FIGURESLIB_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "Dcel.h"


/***********************************************************************************************************************
* API function declarations
***********************************************************************************************************************/
/**
 * @fn              getDelaunay
 * @brief           Returns the dcel associated to a Delaunay triangulation
 *
 * @param vPoints   (IN) Set of points
 * @param dcelOut   (OUT) Voronoi in DCEL format
 * @return          true if Voronoi built successfully
 *                  false otherwise
 */
bool getVoronoi(vector<Point<TYPE>> &vPoints, Dcel &dcelOut);


/**
 * @fn                  getConvexHull
 * @brief               Returns the convex hull of a given set of points
 *
 * @param vPointsIn     (IN) Set of points whose convex hull is returned
 * @param vPointsOut    (OUT) Convex hull set of points
 * @return              true if convex hull built successfully
 *                      false otherwise
 */
bool getConvexHull(vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut);

#endif //DELAUNAY_FIGURESLIB_H
