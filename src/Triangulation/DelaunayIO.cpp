//
// Created by delaunay on 23/3/20.
//
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"
#include "DcelWriter.h"
#include "Delaunay.h"
#include "DelaunayIO.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DelaunayIO::read(const string &fileName, const string &graphFileName, Delaunay &delaunay)
{
    bool	read;		// Return value.

    // Read DCEL data.
    read = DcelReader::read(fileName, false, *delaunay.dcel);
    if (read)
    {
        // Initialize graph.
        delaunay.initializeGraph();

        // Read graph data.
        read = delaunay.graph->read(graphFileName);
        if (read)
        {
            delaunay.setGraphAllocated(true);
            delaunay.setConvexHullComputed(false);
        }
    }

    return read;
}


bool DelaunayIO::write(const string &strFileName, const string &strGraphFileName, const Delaunay &delaunay)
{
    bool	success;		// Return value.

    // Write DCEL data.
    success = DcelWriter::write(strFileName, false, *delaunay.dcel);
    if (success)
    {
        // Write graph data if graph exists.
        if (delaunay.graph->getNElements() > 0)
        {
            success = delaunay.graph->write(strGraphFileName);
        }
    }

    return success;
}
