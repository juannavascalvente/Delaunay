//
// Created by delaunay on 21/3/20.
//

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
private:
    static bool readVertexSet(ifstream &ifs, Dcel &dcel);
    static bool readFlatPoints(const string &fileName, Dcel &dcel);
    static bool readDcelPoints(const string &fileName, Dcel &dcel);
    static bool readBinary(const string &fileName, Dcel &dcel);
    static bool readDcel(const string &fileName, Dcel &dcel);
public:
    /***************************************************************************
    * Name: 	read
    * IN:		fileName		name of the file to read.
    * 			isBinary		indicates if data is in binary format.
    * OUT:		NONE
    * RETURN:	true			if read
    * 			false			i.o.c.
    * GLOBAL:	NONE
    * Description:	reads the dcel data from a file in plain or binary format
    * 				depending on "isBinary" format.
    ***************************************************************************/
    static bool read(const string &fileName, bool isBinary, Dcel &dcel);

    /***************************************************************************
    * Name: 	readPoints
    * IN:		fileName	file name to write data into.
    * OUT:		NONE
    * RETURN:	true 		if file read.
    * 			false		i.o.c.
    * GLOBAL:	NONE
    * Description: 	read the set of points from a flat file or a DCEL file
    * 				depending on the "fromFlatFile· flag.
    ***************************************************************************/
    static bool readPoints(const string &fileName, bool fromFlatFile, Dcel &dcel);
};

#endif //DELAUNAY_DCELREADER_H
