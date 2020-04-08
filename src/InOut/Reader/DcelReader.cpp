/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"
#include "Dcel.h"
#include "defines.h"
#include "Logging.h"
#include "Point.h"

#include <fstream>
#include <sys/stat.h>
#include <cstring>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelReader::read(const string &fileName, bool isBinary, Dcel &dcel)
{
    bool read;		// Return value.

    // Check if read binary data.
    if (isBinary)
    {
        read = DcelReader::readBinary(fileName, dcel);
    }
    else
    {
        read = DcelReader::readDcel(fileName, dcel);
    }

    return(read);
}


/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/**
 * @fn      readVertexSet
 * @brief   read vertex set from file in flat format
 *
 * @param   ifs     (IN) input stream
 * @param   dcel    (OUT) Dcel data
 * @return  true if read successfully
 *          false otherwise
 */
bool DcelReader::readVertexSet(ifstream &ifs, Dcel &dcel)
{
    bool isSuccess=true;	// Return value.
    size_t szNumElements=0;	// # elements to read.

    // Get # points to read.
    ifs >> szNumElements;
    if (szNumElements > 0)
    {
        // Points loop.
        Vertex 	vertex;		// Vertex to read.
        for (size_t i=0; i<szNumElements ;i++)
        {
            ifs >> vertex;
            dcel.addVertex(vertex);
        }
    }
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Number of points is NOT positive: ");
        Logging::buildText(__FUNCTION__, __FILE__, szNumElements);
        Logging::write(true, Error);
        isSuccess = false;
    }

    return isSuccess;
}


/**
 * @fn      readDcel
 * @brief   read dcel from file in flat format
 *
 * @param   fileName    (IN) File name
 * @param   dcel        (OUT) Dcel structure
 * @return  true if read successfully
 *          false otherwise
 */
bool DcelReader::readDcel(const string &fileName, Dcel &dcel)
{
    bool read=true;		// Return value.

    // Open file.
    ifstream ifs(fileName.c_str(), ios::in);
    if (ifs.is_open())
    {
        // Read points
        if (DcelReader::readVertexSet(ifs, dcel))
        {
            // Get # edges to read
            size_t szLength=0;
            ifs >> szLength;
            if (szLength > 0)
            {
                Edge 	edge;		// Edge to read.
                for (size_t i=0; i<szLength ;i++)
                {
                    ifs >> edge;
                    dcel.addEdge(edge);
                }

                // Get # faces to read
                ifs >> szLength;
                if (szLength > 0)
                {
                    Face 	face;		// Face to read.
                    for (size_t i=0; i<szLength ;i++)
                    {
                        ifs >> face;
                        dcel.addFace(face.getEdge());
                    }

                    // Close file.
                    ifs.close();
                }
                else
                {
                    Logging::buildText(__FUNCTION__, __FILE__, "Number of faces is NOT positive: ");
                    Logging::buildText(__FUNCTION__, __FILE__, szLength);
                    Logging::write(true, Error);
                    read = false;
                }
            }
            else
            {
                Logging::buildText(__FUNCTION__, __FILE__, "Number of edges is NOT positive: ");
                Logging::buildText(__FUNCTION__, __FILE__, szLength);
                Logging::write(true, Error);
                read = false;
            }
        }
    }
        // Error opening file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        read = false;
    }

    return(read);
}


/**
 * @fn      readBinary
 * @brief   read dcel from file in binary format
 *
 * @param   fileName    (IN) File name
 * @param   dcel        (OUT) Dcel structure
 * @return  true if read successfully
 *          false otherwise
 */
bool DcelReader::readBinary(const string &fileName, Dcel &dcel)
{
    bool isSuccess=true;				// Return value.

    try
    {
        // Open file.
        ifstream ifs(fileName.c_str(), std::ifstream::binary);
        if (ifs.is_open())
        {
            // Get file size
            struct stat results = {};
            stat(fileName.c_str(), &results);

            // Allocate buffer
            char *arr_cBuffer = new char[results.st_size];

            // Read data from file
            if (!ifs.read (arr_cBuffer, results.st_size))
            {
                Logging::buildText(__FUNCTION__, __FILE__, "Not enough data read from file: ");
                Logging::buildText(__FUNCTION__, __FILE__, fileName);
                Logging::buildText(__FUNCTION__, __FILE__, "Data read is: ");
                Logging::buildText(__FUNCTION__, __FILE__, (size_t) ifs.gcount());
                Logging::write(true, Error);
                isSuccess = false;
                delete[] arr_cBuffer;
            }
            else
            {
                // Extract data
                size_t szLength;
                size_t szPtr=0;

                // Get number of vertex
                memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(size_t));
                szPtr += sizeof(size_t);

                // Get vertex array
                size_t szElementSize = sizeof(TYPE)*2 + sizeof(int);
                char *element = new char[szElementSize];
                for (size_t i=0; i<szLength ; i++)
                {
                    memcpy(&element, &arr_cBuffer[szPtr], szElementSize);
                    szPtr += szElementSize;

                    // Extract point
                    std::string fs(element);
                    TYPE x = std::stof(fs);
                    fs = element[sizeof(TYPE)];
                    TYPE y = std::stof(fs);

                    // Extract edge ID
                    int originEdge = (int) element[sizeof(TYPE)*2];

                    // Add vertex
                    Point<TYPE> p(x, y);
                    Vertex v(originEdge, p);
                    dcel.addVertex(v);
                }
                delete[] element;

                // Get number of edges
                memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(size_t));
                szPtr += sizeof(size_t);

                // Get edges array
                szElementSize = sizeof(int)*5;
                element = new char[szElementSize];
                for (size_t i=0; i<szLength ; i++)
                {
                    memcpy(&element, &arr_cBuffer[szPtr], szElementSize);
                    szPtr += szElementSize;

                    // Extract edge fields
                    int	origin = (int) element[0];
                    int	twin = (int) element[sizeof(int)];
                    int	previous = (int) element[sizeof(int)*2];
                    int	next = (int) element[sizeof(int)*3];
                    int	face = (int) element[sizeof(int)*4];

                    // Add edge
                    Edge e(origin, twin, previous, next, face);
                    dcel.addEdge(e);
                }
                delete[] element;

                // Get number of faces
                memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(size_t));
                szPtr += sizeof(size_t);

                // Get edges array
                szElementSize = sizeof(int);
                element = new char[szElementSize];
                for (size_t i=0; i<szLength ; i++)
                {
                    memcpy(&element, &arr_cBuffer[szPtr], szElementSize);
                    szPtr += szElementSize;

                    // Extract face fields
                    int	edge = (int) element[0];

                    // Add face
                    Face f(edge);
                    dcel.addFace(f);
                }
                delete[] element;
            }

            // Deallocate data.
            delete[] arr_cBuffer;

            // Close file.
            ifs.close();
        }
        // Error opening file.
        else
        {
            Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
            Logging::buildText(__FUNCTION__, __FILE__, fileName);
            Logging::write(true, Error);
            isSuccess = false;
        }

    }
    catch (std::ifstream::failure &e)
    {
        std::cerr << "Exception opening/reading/closing file" << endl;
    }

    return isSuccess;
}
