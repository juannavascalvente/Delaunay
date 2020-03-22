//
// Created by delaunay on 21/3/20.
//

#ifndef DELAUNAY_DCELWRITER_H
#define DELAUNAY_DCELWRITER_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <string>
#include "Dcel.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelWriter
{
public:
    /***************************************************************************
    * Name: 	write
    * IN:		fileName		name of the file to read.
    * 			isBinary		indicates if data is in binary format.
    * OUT:		NONE
    * RETURN:	true			if written
    * 			false			i.o.c.
    * GLOBAL:	NONE
    * Description:	writes the dcel data to a file in plain or binary format
    * 				depending on "isBinary" format.
    ***************************************************************************/
    static bool write(const string &fileName, bool isBinary, const Dcel &dcel);

    /***************************************************************************
    * Name: 	write
    * IN:		fileName		name of the file to write.
    * OUT:		NONE
    * RETURN:	true			if file written.
    * 			false			i.o.c.
    * GLOBAL:	NONE
    * Description:	writes the dcel data to a file.
    ***************************************************************************/
    static bool write(const string &fileName, const Dcel &dcel);

    /***************************************************************************
    * Name: 	writeBinary
    * IN:		fileName		output file name.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	writes DCEL data to output stream in binary format.
    ***************************************************************************/
    static bool writeBinary(const string &fileName, const Dcel &dcel);

    /***************************************************************************
    * Name: 	writePoints
    * IN:		fileName	file name to write data into.
    * 			nPoints		# points to write.
    * OUT:		NONE
    * RETURN:	true		if written
    * 			false		i.o.c.
    * GLOBAL:	NONE
    * Description: 	writes the set of points in DCEL separated by spaces to
    * 				"fileName". If nPoints is INVALID then write all DCEL
    * 				points. If not then write only "nPoints" points.
    ***************************************************************************/
    static bool writePoints(const string &fileName, int nPoints, const Dcel &dcel);

    /***************************************************************************
    * Name: 	print
    * IN:		out			output stream.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	prints DCEL data to output stream.
    ***************************************************************************/
    static void print(std::ostream& out, const Dcel &dcel);
};

#endif //DELAUNAY_DCELWRITER_H
