/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "PointsWriter.h"

#include "FileExtensionChecker.h"
#include "Logging.h"

#include <cstring>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool PointsWriter::write(const string &fileName, const vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Read points from flat points file
        if (FileExtensionChecker::isBinary(fileName))
        {
            isSuccess = PointsWriter::writeBinary(fileName, vPoints);
        }
        // Read points from binary file
        else
        {
            isSuccess = PointsWriter::writeFlat(fileName, vPoints);
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
 * @fn              writeFlat
 * @brief           Writes set of points to file in plain format
 * @details         Plain format is:    X(1), Y(1), X(2), Y(2), ........ , X(n-1), Y(n-1), X(n), Y(n)
 *
 * @param fileName  (IN) File name where points are written
 * @param vPoints   (IN) Set of points to write
 * @return          true if file written successfully
 *                  false otherwise
 */
bool PointsWriter::writeFlat(const string &fileName, const vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=false;	// Return value.

    // Open file.
    ofstream ofs(fileName.c_str(), ios::out);
    if (ofs.is_open())
    {
        // Points loop
        ofs << fixed;
        ofs.precision(6);
        for (auto point : vPoints)
        {
            ofs << " " << point;
        }

        // Close file.
        ofs.close();

        isSuccess = true;
    }

    return isSuccess;
}

/**
 * @fn              writeBinary
 * @brief           Writes set of points to file in binary format
 * @details         Binary format is:    X(1)Y(1)X(2)Y(2)........X(n-1)Y(n-1)X(n)Y(n)
 *                  The size of each element will depend on the definition of TYPE macro
 *
 * @param fileName  (IN) File name where points are written
 * @param vPoints   (IN) Set of points to write
 * @return          true if file written successfully
 *                  false otherwise
 */
bool PointsWriter::writeBinary(const string &fileName, const vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=false;	// Return value.

    // Open file.
    ofstream ofs(fileName.c_str(), ios::out | ios::binary);
    if (ofs.is_open())
    {
        // Allocate buffer to store points
        auto *buffer = new TYPE[vPoints.size()*2];

        // Points loop
        size_t idx=0;
        for (auto point : vPoints)
        {
            // Write X coordinate
            buffer[idx++] = point.getX();
            buffer[idx++] = point.getY();
        }

        // Write buffer
        ofs.write(reinterpret_cast<char*>(buffer), sizeof(TYPE)*idx);

        // Free resources
        ofs.close();
        delete[] buffer;

        isSuccess = true;
    }


    return isSuccess;
}
