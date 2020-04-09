#ifndef DELAUNAY_DCELREADER_H
#define DELAUNAY_DCELREADER_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <string>
#include "Dcel.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelReader
{

    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
    static bool readVertexSet(ifstream &ifs, Dcel &dcel);
    static bool readBinary(const string &fileName, Dcel &dcel);
    static bool readDcel(const string &fileName, Dcel &dcel);

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn          read
     * @brief       reads the dcel data from a file in plain or binary format
     * 				depending on "isBinary" format.
     *
     * @param   fileName  (IN) File name
     * @param   isBinary  (IN) Binary format flag
     * @param   dcel      (OUT) Dcel to update
     * @return  true if read successfully
     *          false otherwise
     */
    static bool read(const string &fileName, bool isBinary, Dcel &dcel);
};

#endif //DELAUNAY_DCELREADER_H
