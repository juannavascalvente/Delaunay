/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "PointsReader.h"

#include "FileExtensionChecker.h"
#include "Logging.h"

#include <sys/stat.h>
#include <cstring>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool PointsReader::read(const string &fileName, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Read points from flat points file
        if (FileExtensionChecker::isBinary(fileName))
        {
            isSuccess = PointsReader::readBinary(fileName, vPoints);
        }
            // Read points from binary file
        else
        {
            isSuccess = PointsReader::readFlat(fileName, vPoints);
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
    bool isSuccess=false;	// Return value.

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
                }
            }
        }

        // Close file.
        ifs.close();
        isSuccess = true;
    }
    // Error opening points file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
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
    bool isSuccess=false;	// Return value.

    // Open file
    ifstream ifs(fileName.c_str(), ios::in | ios::binary);
    if (ifs.is_open())
    {
        // Get file size
        struct stat results = {};
        stat(fileName.c_str(), &results);

        // Allocate buffer
        auto *buffer = new TYPE[results.st_size];

        // Read data from file
        if (!ifs.read(reinterpret_cast<char*>(buffer), results.st_size))
        {
            Logging::buildText(__FUNCTION__, __FILE__, "Not enough data read from file: ");
            Logging::buildText(__FUNCTION__, __FILE__, fileName);
            Logging::buildText(__FUNCTION__, __FILE__, "Data read is: ");
            Logging::buildText(__FUNCTION__, __FILE__, (size_t) ifs.gcount());
            Logging::write(true, Error);
        }
        else
        {
            // Check file has a round number of points
            if ((ifs.gcount() % results.st_size) != 0)
            {
                Logging::buildText(__FUNCTION__, __FILE__, "Last point has no Y coordinate ");
                Logging::buildText(__FUNCTION__, __FILE__, fileName);
                Logging::write(true, Error);
            }

            // Extract points from buffer
            size_t szIdx=0;
            size_t szPointSize=sizeof(TYPE)*2;
            size_t szNumPoints = ifs.gcount() / szPointSize;
            for (size_t i=0; i<szNumPoints ; i++)
            {
                TYPE x = buffer[szIdx++];
                TYPE y = buffer[szIdx++];

                // Add point to vector
                Point<TYPE> p(x,y);
                vPoints.push_back(p);
            }
        }

        // Free resources
        delete[] buffer;
        isSuccess = true;
    }
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
    }

    return isSuccess;
}
