/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "PointsWriter.h"
#include "Logging.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool PointsWriter::write(const string &fileName, vector<Point<TYPE>> &vPoints)
{
    bool isSuccess=true;	// Return value.
    ofstream ofs;		// Output file.

    // Open file.
    ofs.open(fileName.c_str(), ios::out);

    // Check file is opened.
    if (ofs.is_open())
    {
        // Points loop.
        for (auto point : vPoints)
        {
            ofs << " " << point;
        }

        // Close file.
        ofs.close();
    }
        // Error opening points file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        isSuccess = false;
    }

    return(isSuccess);
}