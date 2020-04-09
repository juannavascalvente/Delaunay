/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "GraphWriter.h"

#include <fstream>
using namespace std;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool GraphWriter::write(const string &strFileName, bool isBinary, Graph &graph)
{
    bool isSuccess;		// Return value.

    // Check if write binary data.
    if (isBinary)
    {
        isSuccess = GraphWriter::writeBinary(strFileName, graph);
    }
    else
    {
        isSuccess = GraphWriter::writeFlat(strFileName, graph);
    }

    return isSuccess;
}


void GraphWriter::print(std::ostream& out, Graph &graph)
{
    // Print graph length and size.
    out << "Graph # elements: " << graph.getSize() << std::endl;

    // Print graph nodes.
    for (size_t i=0; i<graph.getSize() ;i++)
    {
        graph.vNodes.at(i).print(out);
    }

    // Print face-node relations.
    for (size_t i=0; i<graph.getSize() ;i++)
    {
        out << graph.vFaceNode.at(i) << endl;
    }
}

/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/**
 * @fn                  writeFlat
 * @brief               Writes graph to output file in flat format
 *
 * @param strFileName   (IN)    File name to write to
 * @param graph         (IN)    Graph to write to file
 * @return              true if written successfully
 *                      false otherwise
 */
bool GraphWriter::writeFlat(const string &strFileName, Graph &graph)
{
    bool success=false;	// Return value.

    try
    {
        // Open file.
        ofstream ofs(strFileName.c_str(), ios::out);

        // Check file is opened.
        if (ofs.is_open())
        {
            // Write # nodes.
            ofs << graph.getSize() << endl;

            // Nodes main loop.
            for (size_t i=0; i<graph.getSize() ;i++)
            {
                graph.vNodes.at(i).write(ofs);
                ofs << endl;
            }

            // Read face-node relations.
            for (int i : graph.vFaceNode)
            {
                ofs << i << endl;
            }

            // Close file.
            ofs.close();
            success = true;
        }
    }
    catch (ofstream::failure &e)
    {
        cerr << "Exception opening/reading/closing file " << strFileName << endl;
    }

    return success;
}


/**
 * @fn                  writeBinary
 * @brief               Writes graph to output file in binary format
 *
 * @param strFileName   (IN)    File name to write to
 * @param graph         (IN)    Graph to write to file
 * @return              true if written successfully
 *                      false otherwise
 */
bool GraphWriter::writeBinary(const string &strFileName, Graph &graph)
{
    return false;
}
