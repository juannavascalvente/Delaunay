//
// Created by delaunay on 23/3/20.
//

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
    /***************************************************************************
    * Name: 	read
    * IN:		fileName		file name to read from.
    * 			graphFileName	incremental graph filename.
    * OUT:		NONE
    * RETURN:	true			if read
    * 			false			i.o.c.
    * GLOBAL:	NONE
    * Description: 	reads the DCEL and the graph data from two files, first
    * 				named "fileName" and the graph from the "filaNameGraph.txt".
    ***************************************************************************/
    static bool read(const string &fileName, const string &graphFileName, Delaunay &delaunay);

    /***************************************************************************
    * Name: 	write
    * IN:		fileName	file name to write to.
    * 			graphFileName	incremental graph filename.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	writes the DCEL and the graph data to two files, first
    * 				named "fileName" and the graph to the "filaNameGraph.txt".
    ***************************************************************************/
    static bool write(const string &strFileName, const string &strGraphFileName, const Delaunay &delaunay);
};


#endif //DELAUNAY_DELAUNAYIO_H
