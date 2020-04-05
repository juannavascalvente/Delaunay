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
    DcelWriter::print(out, voronoi.dcel);
}


bool VoronoiIO::read(const string &strFileName, Voronoi &voronoi)
{
    // Read voronoi DCEL file.
    bool isSuccess = DcelReader::read(strFileName, false, voronoi.dcel);

    return isSuccess;
}


bool VoronoiIO::write(const string &strFileName, const Voronoi &voronoi)
{
    bool isSuccess = DcelWriter::write(strFileName, false, voronoi.dcel);

    return isSuccess;
}
