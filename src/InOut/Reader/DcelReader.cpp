/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"

#include "Dcel.h"
#include "defines.h"
#include "FileExtensionChecker.h"
#include "Logging.h"
#include "Point.h"

#include <fstream>
#include <sys/stat.h>
#include <cstring>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelReader::read(const string &fileName, Dcel &dcel)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Check if read binary data.
        if (FileExtensionChecker::isBinary(fileName))
        {
            isSuccess = DcelReader::readBinary(fileName, dcel);
        }
        else
        {
            isSuccess = DcelReader::readFlat(fileName, dcel);
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
 * @fn      readFlat
 * @brief   read dcel from file in flat format
 *
 * @param   fileName    (IN) File name
 * @param   dcel        (OUT) Dcel structure
 * @return  true if read successfully
 *          false otherwise
 */
bool DcelReader::readFlat(const string &fileName, Dcel &dcel)
{
    bool isSuccess=false;		// Return value.

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
                    isSuccess = true;
                }
                else
                {
                    Logging::buildText(__FUNCTION__, __FILE__, "Number of faces is NOT positive: ");
                    Logging::buildText(__FUNCTION__, __FILE__, szLength);
                    Logging::write(true, Error);
                }
            }
            else
            {
                Logging::buildText(__FUNCTION__, __FILE__, "Number of edges is NOT positive: ");
                Logging::buildText(__FUNCTION__, __FILE__, szLength);
                Logging::write(true, Error);
            }
        }
    }

    return isSuccess;
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
    bool isSuccess=false;				// Return value.

    // Open file.
    ifstream ifs(fileName.c_str(), ios::in | ios::binary);
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
            char intElement[sizeof(int)];
            TYPE x, y;
            int originEdge;
            for (size_t i=0; i<szLength ; i++)
            {
                memcpy(&x, &arr_cBuffer[szPtr], sizeof(x));
                szPtr += sizeof(TYPE);

                memcpy(&y, &arr_cBuffer[szPtr], sizeof(y));
                szPtr += sizeof(TYPE);

                memcpy(&originEdge, &arr_cBuffer[szPtr], sizeof(originEdge));
                szPtr += sizeof(originEdge);

                // Add vertex
                Point<TYPE> p(x, y);
                Vertex v(originEdge, p);
                dcel.addVertex(v);
            }

            // Get number of edges
            memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(size_t));
            szPtr += sizeof(size_t);
            int origin, twin, previous, next, face;

            // Get edges array
            for (size_t i=0; i<szLength ; i++)
            {
                memcpy(&origin, &arr_cBuffer[szPtr], sizeof(origin));
                szPtr += sizeof(origin);
                memcpy(&twin, &arr_cBuffer[szPtr], sizeof(twin));
                szPtr += sizeof(twin);
                memcpy(&previous, &arr_cBuffer[szPtr], sizeof(previous));
                szPtr += sizeof(previous);
                memcpy(&next, &arr_cBuffer[szPtr], sizeof(next));
                szPtr += sizeof(next);
                memcpy(&face, &arr_cBuffer[szPtr], sizeof(face));
                szPtr += sizeof(face);

                // Add edge
                Edge e(origin, twin, previous, next, face);
                dcel.addEdge(e);
            }

            // Get number of faces
            memcpy(&szLength, &arr_cBuffer[szPtr], sizeof(size_t));
            szPtr += sizeof(size_t);

            // Get edges array
            int edge;
            for (size_t i=0; i<szLength ; i++)
            {
                memcpy(&edge, &arr_cBuffer[szPtr], sizeof(edge));
                szPtr += sizeof(edge);

                // Add face
                Face f(edge);
                dcel.addFace(f);
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
