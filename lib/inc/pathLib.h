#ifndef DELAUNAY_PATHLIB_H
#define DELAUNAY_PATHLIB_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"


/***********************************************************************************************************************
* API function declarations
***********************************************************************************************************************/
/**
 * @fn                  getTriangulationPath
 * @brief               Returns the triangles path of a given set between two input points
 * @details             Output vector contains a face every three points
 *
 * @param o             (IN) Origin path point
 * @param d             (IN) Destination path point
 * @param vPointsIn     (IN) Set of points that is used to compute triangulation
 * @param vPointsOut    (OUT) Set of points that contains a face every three points
 * @return              true if path computed
 *                      false otherwise
 */
bool getTriangulationPath(Point<TYPE> &o, Point<TYPE> &d, vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut);


/**
 * @fn                  getTriangulationPath
 * @brief               Returns the triangles path of a given set between two input points
 * @details             Output vector contains one point for evey Voronoi area (Voronoi area center)
 *
 * @param o             (IN) Origin path point
 * @param d             (IN) Destination path point
 * @param vPointsIn     (IN) Set of points that is used to compute Voronoi
 * @param vPointsOut    (OUT) Set of points that contains one Voronoi face every point
 * @return              true if path computed
 *                      false otherwise
 */
bool getVoronoiPath(Point<TYPE> &o, Point<TYPE> &d, vector<Point<TYPE>> &vPointsIn, vector<Point<TYPE>> &vPointsOut);


#endif //DELAUNAY_PATHLIB_H
