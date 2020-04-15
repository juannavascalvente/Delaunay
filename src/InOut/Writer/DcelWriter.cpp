/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelWriter.h"
#include "FileExtensionChecker.h"
#include "Logging.h"

#include <cstring>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelWriter::write(const string &fileName, const DcelModel &dcel)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Check if write binary data.
        if (FileExtensionChecker::isBinary(fileName))
        {
            isSuccess = DcelWriter::writeBinary(fileName, dcel);
        }
        else
        {
            isSuccess = DcelWriter::writeFlat(fileName, dcel);
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


void DcelWriter::print(const DcelModel &dcel, std::ostream &out)
{
    // Points loop.
    std::cout << dcel.getNumVertex() << std::endl;
    for (auto vertex : dcel.vVertex)
    {
        cout << vertex;
    }


    // Edges loop.
    std::cout << dcel.getNumEdges() << std::endl;
    for (auto edge : dcel.vEdges)
    {
        cout << edge;
    }

    cout << dcel.getNumFaces() << std::endl;
    for (auto face : dcel.vFaces)
    {
        cout << face;
    }
}


/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/**
 * @fn          writeFlat
 * @brief       Writes Dcel to file in flat format
 *
 * @param fileName  (IN) File name
 * @param dcel      (IN) Dcel data to write
 * @return          true if file written
 *                  false otherwise
 */
bool DcelWriter::writeFlat(const string &fileName, const DcelModel &dcel)
{
    bool isSuccess;     // Return value.

    // Open file.
    ofstream ofs(fileName.c_str(), ios::out);
    if (ofs.is_open())
    {
        // Write # points and points
        ofs << dcel.getNumVertex() << std::endl;
        for (auto point : dcel.vVertex)
        {
            ofs << point;
        }

        // Write # edges and edges
        ofs << dcel.getNumEdges() << std::endl;
        for (auto edge : dcel.vEdges)
        {
            ofs << edge;
        }

        // Write # faces and faces
        ofs << dcel.getNumFaces() << std::endl;
        for (auto face : dcel.vFaces)
        {
            ofs << face << std::endl;
        }

        // Close file.
        ofs.close();
        isSuccess = true;
    }
    // Error opening file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
    }

    return isSuccess;
}


/**
 * @fn          writeBinary
 * @brief       Writes Dcel to file in binary format
 *
 * @param fileName  (IN) File name
 * @param dcel      (IN) Dcel data to write
 * @return          true if file written
 *                  false otherwise
 */
bool DcelWriter::writeBinary(const string &fileName, const DcelModel &dcel)
{
    bool isSuccess=false;	    // Return value.

    // Open file.
    ofstream ofs(fileName.c_str(), ios::out | ios::binary);
    if (ofs.is_open())
    {
        // Computing amount of data to write.
        unsigned long size = sizeof(size_t)*3 + sizeof(Vertex)*dcel.getNumVertex() +
                             sizeof(Edge)*dcel.getNumEdges() +
                             sizeof(Face)*dcel.getNumFaces();

        // Allocate buffer.
        char *buffer = new char[size];

        // Common fields.
        char *ptr = buffer;

        // Copy vertex data.
        size_t szValue = dcel.getNumVertex();
        memcpy(ptr, &szValue, sizeof(szValue));
        ptr += sizeof(szValue);
        size_t szNumBytes = sizeof(szValue);
        TYPE value;
        int iValue;
        for (auto vertex : dcel.vVertex)
        {
            value = vertex.getRefPoint()->getX();
            memcpy(ptr, &value, sizeof(TYPE));
            ptr += sizeof(TYPE);
            szNumBytes += sizeof(TYPE);
            value = vertex.getRefPoint()->getY();
            memcpy(ptr, &value, sizeof(TYPE));
            ptr += sizeof(TYPE);
            szNumBytes += sizeof(TYPE);
            iValue = vertex.getOrigin();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
        }

        // Copy edges data.
        szValue = dcel.getNumEdges();
        memcpy(ptr, &szValue, sizeof(szValue));
        ptr += sizeof(szValue);
        szNumBytes += sizeof(szValue);
        for (auto edge : dcel.vEdges)
        {
            iValue = edge.getOrigin();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
            iValue = edge.getTwin();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
            iValue = edge.getPrevious();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
            iValue = edge.getNext();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
            iValue = edge.getFace();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
        }

        // Copy faces data.
        szValue = dcel.getNumFaces();
        memcpy(ptr, &szValue, sizeof(szValue));
        ptr += sizeof(szValue);
        szNumBytes += sizeof(szValue);
        for (auto face : dcel.vFaces)
        {
            iValue = face.getEdge();
            memcpy(ptr, &iValue, sizeof(iValue));
            ptr += sizeof(iValue);
            szNumBytes += sizeof(iValue);
        }

        // Write data.
        ofs.write(buffer, sizeof(char)*szNumBytes);

        // Close file.
        ofs.close();

        // Deallocate data.
        delete[] buffer;
        isSuccess = true;
    }
    // Error opening file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
    }

    return isSuccess;
}
