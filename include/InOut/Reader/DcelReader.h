#ifndef DELAUNAY_DCELREADER_H
#define DELAUNAY_DCELREADER_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"

#include <string>


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
    static bool readFlat(const string &fileName, Dcel &dcel);

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
     * @param   dcel      (OUT) Dcel to update
     * @return  true if read successfully
     *          false otherwise
     */
    static bool read(const string &fileName, Dcel &dcel);
};

#endif //DELAUNAY_DCELREADER_H
