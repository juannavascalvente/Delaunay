#ifndef DELAUNAY_FIGUREIO_H
#define DELAUNAY_FIGUREIO_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Polygon.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class FigureIO
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn      read
     * @brief   Reads polygon from file
     *
     * @param   strFileName (IN) Input file name
     * @param   out         (OUT) Polygon read from file
     * @return  true if read successfully
     *          false otherwise
     */
    static bool read(const string &strFileName, Polygon &out);

    /**
     * @fn      read
     * @brief   Writes polygon to a file
     *
     * @param   strFileName (IN) Input file name
     * @param   in          (IN) Polygon to write
     * @return  true if written successfully
     *          false otherwise
     */
    static bool write(const string &strFileName, const Polygon &in);

    /**
     * @fn      print
     * @brief   Prints polygon to a stream
     *
     * @param   polygon (IN)    Polygon to print
     * @param   out     (OUT)   Stream to print polygon
     */
    static void print(const Polygon &polygon, std::ostream& out);
};


#endif //DELAUNAY_FIGUREIO_H
