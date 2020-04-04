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
 * @param vPoints   (IN) Set of points
 * @param dcel      (OUT) Star triangulation in DCEL format
 * @return          true if triangulation built successfully
 *                  false otherwise
 */
bool getStarTriangulation(const vector<TYPE> &vPoints, Dcel &dcel);


/**
 * @fn              getDelaunay
 * @brief           Returns the dcel associated to a Delaunay triangulation
 *
 * @param vPoints   (IN) Triangulation points
 * @param dcel      (OUT) Delunay triangulation in DCEL format
 * @return          true if triangulation built successfully
 *                  false otherwise
 */
bool getDelaunay(const vector<TYPE> &vPoints, Dcel &dcel);


/**
 * @fn              getDelaunay
 * @brief           Returns the dcel associated to a Delaunay triangulation
 *
 * @param vPoints   (IN) Set of points
 * @param dcelOut   (OUT) Voronoi in DCEL format
 * @return          true if Voronoi built successfully
 *                  false otherwise
 */
bool getVoronoi(const vector<Point<TYPE>> &vPoints, Dcel &dcelOut);


/**
 * @fn                  getConvexHull
 * @brief               Returns the convex hull of a given set of points
 *
 * @param vPointsIn     (IN) Set of points whose convex hull is returned
 * @param vPointsOut    (OUT) Convex hull set of points
 * @return              true if convex hull built successfully
 *                      false otherwise
 */
bool getConvexHull(const vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut);


#endif //DELAUNAY_DELAUNAYLIB_H
