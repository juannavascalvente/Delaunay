#ifndef DELAUNAY_POINTSREADER_H
#define DELAUNAY_POINTSREADER_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class PointsReader
{
    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
    static bool readFlat(const string &strFileName, vector<Point<TYPE>> &vPoints);
    static bool readBinary(const string &strFileName, vector<Point<TYPE>> &vPoints);

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn      read
     * @brief   read the set of points from a flat file or a DCEL file depending on the "fromFlatFile· flag
     *
     * @param   strFileName     (IN) File name
     * @param   fromFlatFile    (IN) Flag that stands for binary format
     * @param   vPoints         (OUT) Points set
     * @return
     */
    static bool read(const string &strFileName, vector<Point<TYPE>> &vPoints);
};


#endif //DELAUNAY_POINTSREADER_H
