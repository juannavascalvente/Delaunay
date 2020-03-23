//
// Created by delaunay on 23/3/20.
//

#ifndef DELAUNAY_VORONOIIO_H
#define DELAUNAY_VORONOIIO_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/


/***********************************************************************************************************************
* Class definition
***********************************************************************************************************************/
class VoronoiIO
{
public:
    /***************************************************************************
    * Name: 	print
    * IN:		out			output stream.
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	print voronoi data.
    ***************************************************************************/
    static void print(const Voronoi &voronoi, std::ostream &out);

    /***************************************************************************
    * Name: 	read
    * IN:		fileName		file name to read data from.
    * OUT:		NONE
    * RETURN:	false			if error writing data.
    * 			true			i.o.c.
    * GLOBAL:	NONE
    * Description: 	read the voronoi data from "fileName" file.
    ***************************************************************************/
    static bool read(const string &strFileName, Voronoi &voronoi);
    
    /***************************************************************************
    * Name: 	write
    * IN:		fileName		file name to write data to.
    * OUT:		NONE
    * RETURN:	false			if error writing data.
    * 			true			i.o.c.
    * GLOBAL:	NONE
    * Description: 	writes the voronoi data to "fileName" file.
    ***************************************************************************/
    static bool write(const string &strFileName, const Voronoi &voronoi);
};


#endif //DELAUNAY_VORONOIIO_H
