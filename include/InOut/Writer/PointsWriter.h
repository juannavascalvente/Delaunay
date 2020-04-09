#ifndef DELAUNAY_POINTSWRITER_H
#define DELAUNAY_POINTSWRITER_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class PointsWriter
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn              write
     * @brief           Writes set of points to file
     *
     * @param fileName  (IN) File name
     * @param vPoints   (IN) Vector of points to write
     * @return          true if file written
     *                  false otherwise
     */
    static bool write(const string &fileName, vector<Point<TYPE>> &vPoints);
};


#endif //DELAUNAY_POINTSWRITER_H
