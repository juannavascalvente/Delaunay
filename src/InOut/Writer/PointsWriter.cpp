/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "PointsWriter.h"
#include "Logging.h"

#include <cstring>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool PointsWriter::write(const string &fileName, vector<Point<TYPE>> &vPoints, bool isBinary)
{
    bool isSuccess;		// Return value.

    // Read points from flat points file
    if (isBinary)
    {
        isSuccess = PointsWriter::writeBinary(fileName, vPoints);
    }
    // Read points from binary file
    else
    {
        isSuccess = PointsWriter::writeFlat(fileName, vPoints);
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
bool PointsWriter::writeFlat(const string &fileName, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=false;	// Return value.

    try
    {
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
    }
    catch (const ofstream::failure& e)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
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
bool PointsWriter::writeBinary(const string &fileName, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=false;	// Return value.

    try
    {
        // Open file.
        ofstream ofs(fileName.c_str(), ios::out | ios::binary);
        if (ofs.is_open())
        {
            // Allocate buffer to store points
            auto *arr_cBuffer = new char[vPoints.size()*sizeof(Point<TYPE>)];
            size_t szCnt=0;

            // Points loop
            for (auto point : vPoints)
            {
                // Write X coordinate
                TYPE value = point.getX();
                memcpy(&arr_cBuffer[szCnt], &value, sizeof(TYPE));
                szCnt += sizeof(TYPE);

                // Write Y coordinate
                value = point.getY();
                memcpy(&arr_cBuffer[szCnt], &value, sizeof(TYPE));
                szCnt += sizeof(TYPE);
            }

            // Write buffer
            ofs.write(arr_cBuffer, szCnt);

            // Free resources
            ofs.close();
            delete[] arr_cBuffer;

            isSuccess = true;
        }
    }
    catch (const ofstream::failure& e)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
    }

    return isSuccess;
}
