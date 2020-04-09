/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Logging.h"
#include "PointsReader.h"

#include <sys/stat.h>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool PointsReader::read(const string &fileName, bool isBinary, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess;		// Return value.

    // Read points from flat points file
    if (isBinary)
    {
        isSuccess = PointsReader::readBinary(fileName, vPoints);
    }
    // Read points from binary file
    else
    {
        isSuccess = PointsReader::readFlat(fileName, vPoints);
    }

    return isSuccess;
}


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/**
 * @fn      readFlat
 * @brief   read the set of points from a flat file
 *
 * @param   fileName        (IN) File name
 * @param   vPoints         (OUT) Points set
 * @return  true if file read successfully
 *          false otherwise
 */
bool PointsReader::readFlat(const string &fileName, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=true;	// Return value.

    // Open file.
    ifstream ifs(fileName.c_str(), ios::in);
    if (ifs.is_open())
    {
        TYPE x=0.0, y=0.0;
        while (!ifs.eof())
        {
            // Read x coordinate
            if (ifs >> x)
            {
                // Read y coordinate
                if (ifs >> y)
                {
                    Point<TYPE> p(x,y);
                    vPoints.push_back(p);
                }
                else
                {
                    Logging::buildText(__FUNCTION__, __FILE__, "Last point does not have Y coordinate: ");
                    Logging::buildText(__FUNCTION__, __FILE__, fileName);
                    Logging::write(true, Error);
                    isSuccess = false;
                }
            }
        }

        // Close file.
        ifs.close();
    }
    // Error opening points file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        isSuccess = false;
    }

    return isSuccess;
}


/**
 * @fn      readBinary
 * @brief   read the set of points from a binary file
 *
 * @param   fileName        (IN) File name
 * @param   vPoints         (OUT) Points set
 * @return  true if file read successfully
 *          false otherwise
 */
bool PointsReader::readBinary(const string &fileName, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=true;	// Return value.

    // Open file
    ifstream ifs(fileName.c_str(), ios::in);
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
        }
        else
        {
            // Check file has a round number of points
            size_t szPointSize=sizeof(TYPE)*2;
            if ((ifs.gcount() % szPointSize) != 0)
            {
                Logging::buildText(__FUNCTION__, __FILE__, "Last point has no Y coordinate ");
                Logging::buildText(__FUNCTION__, __FILE__, fileName);
                Logging::write(true, Error);
                isSuccess = false;
            }

            // Extract points from buffer
            size_t szPtr=0;
            size_t szNumPoints = ifs.gcount() / szPointSize;
            for (size_t i=0; i<szNumPoints ; i++)
            {
                string fs(&arr_cBuffer[szPtr]);
                TYPE x = std::stof(fs);
                szPtr += sizeof(TYPE);
                fs = &arr_cBuffer[szPtr];
                TYPE y = std::stof(fs);
                szPtr += sizeof(TYPE);

                // Add point to vector
                Point<TYPE> p(x,y);
                vPoints.push_back(p);
            }
        }

        // Free resources
        delete[] arr_cBuffer;
    }
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        isSuccess = false;
    }

    return isSuccess;
}

