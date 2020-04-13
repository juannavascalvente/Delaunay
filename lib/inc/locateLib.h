#ifndef DELAUNAY_LOCATELIB_H
#define DELAUNAY_LOCATELIB_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "Point.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Public struct definitions
***********************************************************************************************************************/
typedef vector<Point<TYPE>> FacePoints;


/***********************************************************************************************************************
* API function declarations
***********************************************************************************************************************/
/**
 * @fn      getClosestPoint
 * @brief   Given a set of points, it returns the closest point to an input point
 *
 * @param   p       (IN) Point whose closest point in the set is returned
 * @param   vPoints (IN) Set of points where closest point belongs
 * @param   closest (OUT) Closest point
 * @return  true if point found
 *          false otherwise
 */
bool getClosestPoint(Point<TYPE> &p, vector<Point<TYPE>> &vPoints, Point<TYPE> &closest);


/**
 * @fn      get2ClosestPoints
 * @brief   Given a set of points, it returns the 2 closest points
 *
 * @param   vPoints   (IN) Set of points where 2 closest points belongs
 * @param   p         (OUT) First point
 * @param   q         (OUT) Second point
 * @return  true if points found
 *          false otherwise
 */
bool get2ClosestPoints(vector<Point<TYPE>> &vPoints, Point<TYPE> &p, Point<TYPE> &q);


/**
 * @fn      getPointFace
 * @brief   Given a set of points and a point, returns the three points that surround the input point
 * @details If point is external to points set, then function returns true and output vector is empty
 *
 * @param   p           (IN) Point whose face is returned
 * @param   vPoints     (IN) Set of points
 * @param   facePoints  (OUT) Vector that contains points that surround input point
 * @return  true if face found
 *          false otherwise
 */
bool getPointFace(Point<TYPE> &p, vector<Point<TYPE>> &vPoints, FacePoints &facePoints);


#endif //DELAUNAY_LOCATELIB_H
