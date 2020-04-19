/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "GraphReader.h"

#include "FileExtensionChecker.h"
#include "Logging.h"
#include "NodeIO.h"

#include <fstream>
#include <sys/stat.h>
using namespace std;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool GraphReader::read(const string &fileName, Graph &graph)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Initialize output
        graph.reset();

        // Check if read binary data.
        if (FileExtensionChecker::isBinary(fileName))
        {
            isSuccess = GraphReader::readBinary(fileName, graph);
        }
        else
        {
            isSuccess = GraphReader::readFlat(fileName, graph);
        }
    }
    catch (const ofstream::failure& e)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
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


bool GraphReader::readFlat(const string& strFileName, Graph &graph)
{
    bool 	isSuccess=false;	        // Return value.

    // Open file.
    ifstream ifs(strFileName.c_str(), ios::in);
    if (ifs.is_open())
    {
        // Read # nodes.
        size_t szNumElements=0;
        ifs >> szNumElements;

        // Nodes main loop.
        Node	node;
        for (size_t i=0; i<szNumElements ;i++)
        {
            NodeIO::read(node, ifs);
            graph.insert(node);
        }

        // Close file.
        ifs.close();
        isSuccess = true;
    }

    return isSuccess;
}

bool GraphReader::readBinary(const string &strFileName, Graph &graph)
{
    bool 	isSuccess=false;	        // Return value.

    // Open file.
    ifstream ifs(strFileName.c_str(), ios::in | ios::binary);
    if (ifs.is_open())
    {
        // Get file size
        struct stat results = {};
        stat(strFileName.c_str(), &results);

        // Allocate buffer
        char *arr_cBuffer = new char[results.st_size];

        // Read data from file
        if (!ifs.read (arr_cBuffer, results.st_size))
        {
            Logging::buildText(__FUNCTION__, __FILE__, "Not enough data read from file: ");
            Logging::buildText(__FUNCTION__, __FILE__, strFileName);
            Logging::buildText(__FUNCTION__, __FILE__, "Data read is: ");
            Logging::buildText(__FUNCTION__, __FILE__, (size_t) ifs.gcount());
            Logging::write(true, Error);
            delete[] arr_cBuffer;
        }
        else
        {
            // Extract data
            size_t szLength;
            size_t szPtr=0;

            // Get number of nodes
            memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(szLength));
            szPtr += sizeof(szLength);

            // Get nodes array data
            int iNumChildren;
            int iChild1, iChild2, iChild3;
            int iP1, iP2, iP3;
            int iFaceId;
            for (size_t szIdx=0; szIdx < szLength ; szIdx++)
            {
                // Get number of children
                memcpy(&iNumChildren, &arr_cBuffer[szPtr], sizeof(iNumChildren));
                szPtr += sizeof(iNumChildren);

                // Get children
                iChild1 = INVALID;
                iChild2 = INVALID;
                iChild3 = INVALID;
                if (iNumChildren >= 2)
                {
                    memcpy(&iChild1, &arr_cBuffer[szPtr], sizeof(iChild1));
                    szPtr += sizeof(iChild1);
                    memcpy(&iChild2, &arr_cBuffer[szPtr], sizeof(iChild2));
                    szPtr += sizeof(iChild2);
                    if (iNumChildren == 3)
                    {
                        memcpy(&iChild3, &arr_cBuffer[szPtr], sizeof(iChild3));
                        szPtr += sizeof(iChild3);
                    }
                }

                // Get points
                memcpy(&iP1, &arr_cBuffer[szPtr], sizeof(iP1));
                szPtr += sizeof(iP1);
                memcpy(&iP2, &arr_cBuffer[szPtr], sizeof(iP2));
                szPtr += sizeof(iP2);
                memcpy(&iP3, &arr_cBuffer[szPtr], sizeof(iP3));
                szPtr += sizeof(iP3);

                // Get face id
                memcpy(&iFaceId, &arr_cBuffer[szPtr], sizeof(iFaceId));
                szPtr += sizeof(iFaceId);

                // Add node
                if (iNumChildren == 0)
                {
                    vector<int> vPoints = {iP1, iP2, iP3};
                    Node node(vPoints, iFaceId);
                    graph.insert(node);
                }
                else if (iNumChildren == 2)
                {
                    vector<int> vPoints = {iP1, iP2, iP3};
                    vector<int> vChildren = {iChild1, iChild2};
                    Node node(vPoints, vChildren, iFaceId);
                    graph.insert(node);
                }
                else
                {
                    vector<int> vPoints = {iP1, iP2, iP3};
                    vector<int> vChildren = {iChild1, iChild2, iChild3};
                    Node node(vPoints, vChildren, iFaceId);
                    graph.insert(node);
                }
            }

            // Get number of nodes faces
            memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(size_t));
            szPtr += sizeof(size_t);

            // Get edges array
            int iValue;
            for (size_t szIdx=0; szIdx < szLength ; szIdx++)
            {
                memcpy(&iValue, &arr_cBuffer[szPtr], sizeof(iValue));
                szPtr += sizeof(iValue);

                // Update i face node value
                graph.setNodeAssigned(szIdx, iValue);
            }
        }

        // Deallocate data.
        delete[] arr_cBuffer;

        // Close file.
        ifs.close();
        isSuccess = true;
    }

    return isSuccess;
}
