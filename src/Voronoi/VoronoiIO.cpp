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


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void VoronoiIO::print(const Voronoi &voronoi, std::ostream &out)
{
    if (voronoi.valid)
    {
        DcelWriter::print(out, voronoi.voronoi);
    }
}


bool VoronoiIO::read(const string &strFileName, Voronoi &voronoi)
{
    // Read voronoi DCEL file.
    voronoi.valid = DcelReader::read(strFileName, false, voronoi.voronoi);

    return(voronoi.valid);
}


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
