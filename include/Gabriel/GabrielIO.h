//
// Created by delaunay on 23/3/20.
//

#ifndef DELAUNAY_GABRIELIO_H
#define DELAUNAY_GABRIELIO_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Gabriel.h"

#include <ostream>
using namespace std;


/***********************************************************************************************************************
* Class definition
***********************************************************************************************************************/
class GabrielIO
{
public:
    /***************************************************************************
    * Name: 	print
    * IN:		NONE
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description:	prints the Gabriel graph data to an output stream.
    ***************************************************************************/
    static void print(std::ostream &out, const Gabriel &gabriel);

    /***************************************************************************
    * Name: 	read
    * IN:		fileName		input file name.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description:	reads the Gabriel graph data from fileName file.
    ***************************************************************************/
    static bool read(const string &strFileName, Gabriel &gabriel);

    /***************************************************************************
    * Name: 	write
    * IN:		fileName		output file name.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description:	writes the Gabriel graph data to the output fileName file.
    ***************************************************************************/
    static bool write(const string &strFileName, const Gabriel &gabriel);

    /***************************************************************************
    * Name: 	readBinary
    * IN:		fileName		input file name.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description:	reads the Gabriel graph data from fileName file in
    * 				binary format.
    ***************************************************************************/
    static bool readBinary(const string &strFileName, Gabriel &gabriel);

    /***************************************************************************
    * Name: 	writeBinary
    * IN:		fileName		output file name.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description:	writes the Gabriel graph data to the output fileName file
    * 				in binary format.
    ***************************************************************************/
    static bool writeBinary(const string &strFileName, const Gabriel &gabriel);
};


#endif //DELAUNAY_GABRIELIO_H
