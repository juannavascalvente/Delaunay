/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "FigureIO.h"
#include "PointsReader.h"
#include "PointsWriter.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool FigureIO::read(const string &strFileName, Polygon &out)
{
    // Read points from file
    vector<Point<TYPE>> vPoints;
    bool isSuccess = PointsReader::read(strFileName, vPoints);

    if (isSuccess)
    {
        Polygon readPolygon(vPoints);
        out = readPolygon;
    }

    return isSuccess;
}


bool FigureIO::write(const string &strFileName, const Polygon &in)
{
    // Read points from file
    bool isSuccess = PointsWriter::write(strFileName, in.vPoints);

    return isSuccess;
}


void FigureIO::print(const Polygon &polygon, std::ostream& out)
{
    // Print all points.
    for (auto point : polygon.vPoints)
    {
        point.print(out);
    }
}

