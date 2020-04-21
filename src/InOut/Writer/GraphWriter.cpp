/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "GraphWriter.h"

#include "FileExtensionChecker.h"
#include "Logging.h"
#include "NodeIO.h"

#include <fstream>
using namespace std;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool GraphWriter::write(const string &strFileName, const Graph &graph)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Check if write binary data.
        if (FileExtensionChecker::isBinary(strFileName))
        {
            isSuccess = GraphWriter::writeBinary(strFileName, graph);
        }
        else
        {
            isSuccess = GraphWriter::writeFlat(strFileName, graph);
        }
    }
    catch (const ofstream::failure& e)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, strFileName);
        Logging::write(true, Error);
    }
    catch (bad_alloc &ex)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory");
        Logging::write( true, Error);
    }
    catch (exception &ex)
    {
        std::cout << ex.what();
    }

    return isSuccess;
}


void GraphWriter::print(const Graph &graph, std::ostream &out)
{
    // Print graph length and size.
    out << "Graph # elements: " << graph.getSize() << std::endl;

    // Print graph nodes.
    for (size_t i=0; i<graph.getSize() ;i++)
    {
        Node node = graph.vNodes.at(i);
        NodeIO::print(node, out);
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
bool GraphWriter::writeFlat(const string &strFileName, const Graph &graph)
{
    bool isSuccess=false;	// Return value.

    // Open file.
    ofstream ofs(strFileName.c_str(), ios::out);
    if (ofs.is_open())
    {
        // Nodes main loop.
        ofs << graph.vNodes.size() << endl;
        for (size_t i=0; i<graph.getSize() ;i++)
        {
            Node node = graph.vNodes.at(i);
            NodeIO::write(node, ofs);
            ofs << endl;
        }

        // Read face-node relations.
        for (int i : graph.vFaceNode)
        {
            ofs << i << endl;
        }

        // Close file.
        ofs.close();
        isSuccess = true;
    }

    return isSuccess;
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
bool GraphWriter::writeBinary(const string &strFileName, const Graph &graph)
{
    bool isSuccess=false;	// Return value.

    // Open file.
    ofstream ofs(strFileName.c_str(), ios::out | ios::binary);
    if (ofs.is_open())
    {
        // Computing amount of data to write.
        unsigned long size = sizeof(size_t) + sizeof(Node)*graph.vNodes.size() +
                             sizeof(size_t) + sizeof(int)*graph.vFaceNode.size();

        // Allocate buffer.
        size_t szCnt=0;
        char *buffer = new char[size];

        // Insert nodes array length
        size_t szValue = graph.vNodes.size();
        memcpy(&buffer[szCnt], &szValue, sizeof(szValue));
        szCnt += sizeof(szValue);

        // Insert nodes array data
        int iValue;
        for (auto node : graph.vNodes)
        {
            // Add number of children
            iValue = node.getNChildren();
            memcpy(&buffer[szCnt], &iValue, sizeof(iValue));
            szCnt += sizeof(iValue);

            // Add children
            for (size_t szIdx=0; szIdx<node.getNChildren() ; szIdx++)
            {
                iValue = node.getiChild(szIdx);
                memcpy(&buffer[szCnt], &iValue, sizeof(iValue));
                szCnt += sizeof(iValue);
            }

            // Add points indexes
            for (size_t szIdx=0; szIdx<NODE_POINTS ; szIdx++)
            {
                iValue = node.getiPoint(szIdx);
                memcpy(&buffer[szCnt], &iValue, sizeof(iValue));
                szCnt += sizeof(iValue);
            }

            // Add face
            iValue = node.getFace();
            memcpy(&buffer[szCnt], &iValue, sizeof(iValue));
            szCnt += sizeof(iValue);
        }

        // Insert nodes array length
        szValue = graph.vFaceNode.size();
        memcpy(&buffer[szCnt], &szValue, sizeof(szValue));
        szCnt += sizeof(szValue);

        // Copy faces array data
        for (auto faceNode : graph.vFaceNode)
        {
            // Add number of children
            memcpy(&buffer[szCnt], &faceNode, sizeof(faceNode));
            szCnt += sizeof(faceNode);
        }

        // Write data.
        ofs.write(buffer, sizeof(char)*szCnt);

        // Close file.
        ofs.close();

        // Deallocate data.
        delete[] buffer;
        isSuccess = true;
    }

    return isSuccess;
}
