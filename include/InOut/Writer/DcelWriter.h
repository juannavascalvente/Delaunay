#ifndef DELAUNAY_DCELWRITER_H
#define DELAUNAY_DCELWRITER_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"

#include <string>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelWriter
{
    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
    static bool writeFlat(const string &fileName, const DcelModel &dcel);
    static bool writeBinary(const string &fileName, const DcelModel &dcel);

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn          write
     * @brief       Writes Dcel to file
     *
     * @param fileName  (IN) File name
     * @param dcel      (IN) Dcel data to write
     * @param isBinary  (IN) Flag to write data in binary format or plain text
     * @return          true if file written
     *                  false otherwise
     */
    static bool write(const string &fileName, const DcelModel &dcel, bool isBinary=false);

    /**
     * @fn          print
     * @brief       Prints dcel formart to stream
     *
     * @param dcel  (IN)        Dcel Model to print
     * @param out   (IN/OUT)    Output stream
     */
    static void print(const DcelModel &dcel, std::ostream &out);
};

#endif //DELAUNAY_DCELWRITER_H
