#ifndef DELAUNAY_TRIANGULATIONLIB_H
#define DELAUNAY_TRIANGULATIONLIB_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "Dcel.h"


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
bool getStarTriangulation(vector<Point<TYPE>> &vPoints, Dcel &dcel);


/**
 * @fn              getDelaunay
 * @brief           Returns the dcel associated to a Delaunay triangulation
 *
 * @param vPoints   (IN) Triangulation points
 * @param dcel      (OUT) Delunay triangulation in DCEL format
 * @return          true if triangulation built successfully
 *                  false otherwise
 */
bool getDelaunay(vector<Point<TYPE>> &vPoints, Dcel &dcel);


#endif //DELAUNAY_TRIANGULATIONLIB_H
