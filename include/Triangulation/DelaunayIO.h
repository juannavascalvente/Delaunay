#ifndef DELAUNAY_DELAUNAYIO_H
#define DELAUNAY_DELAUNAYIO_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Node.h"


/***********************************************************************************************************************
* Class definition
***********************************************************************************************************************/
class DelaunayIO
{
public:
    /**
     * @fn      read
     * @brief   Reads Delaunay triangulation incremental data
     *
     * @param strDcelFileName   (IN)    Dcel file name
     * @param strGraphFileName  (IN)    Location graph file name
     * @param delaunay          (OUT)   Delaunay data
     * @return                  true if read successfully
     *                          false otherwise
     */
    static bool read(const string &strDcelFileName, const string &strGraphFileName, Delaunay &delaunay);


    /**
     * @fn      write
     * @brief   Writes Delaunay triangulation incremental data
     *
     * @param strDcelFileName   (IN)    Dcel file name
     * @param strGraphFileName  (IN)    Location graph file name
     * @param delaunay          (IN)    Delaunay data
     * @return                  true if written successfully
     *                          false otherwise
     */
    static bool write(const string &strDcelFileName, const string &strGraphFileName, Delaunay &delaunay);
};


#endif //DELAUNAY_DELAUNAYIO_H
