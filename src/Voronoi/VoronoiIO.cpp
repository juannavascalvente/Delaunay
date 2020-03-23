//
// Created by delaunay on 23/3/20.
//
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"
#include "DcelWriter.h"
#include "Voronoi.h"
#include "VoronoiIO.h"


/***************************************************************************
* Name: 	print
* IN:		out			output stream.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	print voronoi data.
***************************************************************************/
void VoronoiIO::print(const Voronoi &voronoi, std::ostream &out)
{
    if (voronoi.valid)
    {
        DcelWriter::print(out, voronoi.voronoi);
    }
}

/***************************************************************************
* Name: 	read
* IN:		fileName		file name to read data from.
* OUT:		NONE
* RETURN:	false			if error writing data.
* 			true			i.o.c.
* GLOBAL:	NONE
* Description: 	read the voronoi data from "fileName" file.
***************************************************************************/
bool VoronoiIO::read(const string &strFileName, Voronoi &voronoi)
{
    // Read voronoi DCEL file.
    voronoi.valid = DcelReader::read(strFileName, false, voronoi.voronoi);

    return(voronoi.valid);
}

/***************************************************************************
* Name: 	write
* IN:		fileName		file name to write data to.
* OUT:		NONE
* RETURN:	false			if error writing data.
* 			true			i.o.c.
* GLOBAL:	NONE
* Description: 	writes the voronoi data to "fileName" file.
***************************************************************************/
bool VoronoiIO::write(const string &strFileName, const Voronoi &voronoi)
{
    bool write;		// Return value.

    // Write data to file.
    write = voronoi.valid;
    if (voronoi.valid)
    {
        write = DcelWriter::write(strFileName, false, voronoi.voronoi);
    }

    return(write);
}
