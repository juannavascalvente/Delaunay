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

    // Initialize output
    delaunay.reset();

    // Read DCEL data.
    read = DcelReader::read(fileName, false, delaunay.dcel);
    if (read)
    {
        delaunay.setValid(true);

        // Initialize graph.
        delaunay.initGraph();

        // Read graph data.
        read = delaunay.graph.read(graphFileName);
    }

    return read;
}


bool DelaunayIO::write(const string &strFileName, const string &strGraphFileName, Delaunay &delaunay)
{
    bool	success;		// Return value.

    // Write DCEL data.
    success = DcelWriter::write(strFileName, delaunay.dcel, false);
    if (success)
    {
        // Write graph data if graph exists.
        if (delaunay.graph.getSize() > 0)
        {
            success = delaunay.graph.write(strGraphFileName);
        }
    }

    return success;
}
