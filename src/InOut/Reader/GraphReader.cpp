/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "GraphReader.h"

#include <fstream>
using namespace std;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool GraphReader::read(const string& fileName, bool isBinary, Graph &graph)
{
    bool isSuccess;		// Return value.

    // Check if read binary data.
    if (isBinary)
    {
        isSuccess = GraphReader::readBinary(fileName, graph);
    }
    else
    {
        isSuccess = GraphReader::readFlat(fileName, graph);
    }

    return isSuccess;
}


bool GraphReader::readFlat(const string& strFileName, Graph &graph)
{
    bool 	isSuccess=false;	        // Return value.

    try
    {
        // Initialize output
        graph.reset();

        // Open file.
        ifstream ifs(strFileName.c_str(), ios::in);

        // Check file is opened.
        if (ifs.is_open())
        {
            // Read # nodes.
            size_t szNumElements=0;
            ifs >> szNumElements;

            // Nodes main loop.
            Node	node;
            for (size_t i=0; i<szNumElements ;i++)
            {
                node.read(ifs);
                graph.insert(node);
            }

            // Close file.
            ifs.close();

            isSuccess = true;
        }
    }
    catch (std::ifstream::failure &e)
    {
        std::cerr << "Exception opening/reading/closing file " << strFileName << endl;
    }

    return isSuccess;
}