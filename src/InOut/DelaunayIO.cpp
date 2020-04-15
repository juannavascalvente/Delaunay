/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"
#include "DcelWriter.h"
#include "Delaunay.h"
#include "DelaunayIO.h"
#include "GraphReader.h"
#include "GraphWriter.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DelaunayIO::read(const string &strDcelFileName, const string &strGraphFileName, Delaunay &delaunay)
{
    bool	isSuccess;		// Return value.

    // Initialize output
    delaunay.reset();

    // Read DCEL data.
    isSuccess = DcelReader::read(strDcelFileName, delaunay.dcel);
    if (isSuccess)
    {
        delaunay.setValid(true);

        // Initialize graph.
        delaunay.initGraph();

        // Read graph data.
        isSuccess = GraphReader::read(strGraphFileName, false, delaunay.graph);
    }

    return isSuccess;
}


bool DelaunayIO::write(const string &strDcelFileName, const string &strGraphFileName, Delaunay &delaunay)
{
    bool	isSuccess;		// Return value.

    // Write DCEL data.
    isSuccess = DcelWriter::write(strDcelFileName, delaunay.dcel);
    if (isSuccess)
    {
        // Write graph data if graph exists.
        if (delaunay.graph.getSize() > 0)
        {
            isSuccess = GraphWriter::write(strGraphFileName, false, delaunay.graph);
        }
    }

    return isSuccess;
}
