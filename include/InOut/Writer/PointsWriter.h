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
    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
    static bool writeFlat(const string &fileName, vector<Point<TYPE>> &vPoints);
    static bool writeBinary(const string &fileName, vector<Point<TYPE>> &vPoints);

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn              write
     * @brief           Writes set of points to file
     * @details         Plain format is:    X(1), Y(1), X(2), Y(2), ........ , X(n-1), Y(n-1), X(n), Y(n)
     *                  Binary format is:    X(1)Y(1)X(2)Y(2)........X(n-1)Y(n-1)X(n)Y(n)
     *                  The size of each element will depend on the definition of TYPE macro
     *
     * @param fileName  (IN) File name
     * @param vPoints   (IN) Vector of points to write
     * @param isBinary  (IN) Flag that stands for binary output format or clear text
     * @return          true if file written
     *                  false otherwise
     */
    static bool write(const string &fileName, vector<Point<TYPE>> &vPoints, bool isBinary=false);
};


#endif //DELAUNAY_POINTSWRITER_H
