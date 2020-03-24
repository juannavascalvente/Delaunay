/*
 * Config.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: juan
 */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include <cfloat>
#include "generic.h"
#include "Logging.h"
#include <string>

using namespace std;


/***********************************************************************************************************************
* Defines section
***********************************************************************************************************************/
#define	DEFAULT_N_POINTS				10
#define	DEFAULT_INPUT_FLAT_FILENAME		"inFlat.txt"
#define	DEFAULT_INPUT_DCEL_FILENAME		"inDCEL.txt"
#define	DEFAULT_INPUT_GRAPH_FILENAME	"inGraph.txt"
//#define DEFAULT_INPUT_VORONOI_FILENAME	"inVoronoi.txt"
//#define DEFAULT_INPUT_GABRIEL_FILENAME	"inGabriel.txt"
#define	DEFAULT_OUTPUT_FLAT_FILENAME	"outFlat.txt"
#define	DEFAULT_OUTPUT_DCEL_FILENAME	"outDCEL.txt"
#define	DEFAULT_OUTPUT_GRAPH_FILENAME	"outGraph.txt"
#define DEFAULT_OUTPUT_VORONOI_FILENAME "outVoronoi.txt"
#define DEFAULT_OUTPUT_GABRIEL_FILENAME	"outGabriel.txt"
#define DEFAULT_MIN_X					-1
#define DEFAULT_MAX_X					5000
#define DEFAULT_MIN_Y					-1
#define DEFAULT_MAX_Y					5000
#define DEFAULT_ANCHORS					1
#define DEFAULT_NCLUSTERS				1
#define DEFAULT_CLUSTER_RADIUS			100

// Fields in configuration file.
#define N_POINTS_PARAM				"N_POINTS"
#define INPUT_FLAT_FILE_PARAM		"INPUT_FLAT_FILE"
#define INPUT_DCEL_FILE_PARAM		"INPUT_DCEL_FILE"
#define INPUT_GRAPH_FILE_PARAM		"INPUT_GRAPH_FILE"
//#define INPUT_VORONOI_FILE_PARAM	"INPUT_VORONOI_FILE"
//#define INPUT_GABRIEL_FILE_PARAM	"INPUT_GABRIEL_FILE"
#define OUTPUT_FLAT_FILE_PARAM		"OUTPUT_FLAT_FILE"
#define OUTPUT_DCEL_FILE_PARAM		"OUTPUT_DCEL_FILE"
#define OUTPUT_GRAPH_FILE_PARAM		"OUTPUT_GRAPH_FILE"
#define OUTPUT_VORONOI_FILE_PARAM	"OUTPUT_VORONOI_FILE"
#define OUTPUT_GABRIEL_FILE_PARAM	"OUTPUT_GABRIEL_FILE"
#define CLOSEST_POINT_PARAM			"CLOSEST_POINT"
#define ORIGIN_POINT_PARAM			"ORIGIN_POINT"
#define DESTINATION_POINT_PARAM		"DESTINATION_POINT"
#define N_ANCHORS_PARAM				"N_ANCHORS"
#define MIN_LENGTH_EDGE_PARAM		"MIN_LENGTH_EDGE"
#define ZOOM_PARAM					"ZOOM"
#define CLUSTER_SET_PARAM			"CLUSTER_SET"


/***********************************************************************************************************************
* Static members section
***********************************************************************************************************************/
int Config::nPoints = DEFAULT_N_POINTS;
string Config::strConfigFileName;
string Config::inFlatFileName;
string Config::inDcelFileName;
string Config::inGraphFileName;
//string Config::inVoronoiFileName = DEFAULT_INPUT_VORONOI_FILENAME;
//string Config::inGabrielFileName = DEFAULT_INPUT_GABRIEL_FILENAME;
string Config::outFlatFileName;
string Config::outDcelFileName;
string Config::outGraphFileName;
string Config::outVoronoiFileName;
string Config::outGabrielFileName;
Point<TYPE> Config::closestPoint;
Point<TYPE> Config::originPoint;
Point<TYPE> Config::destinationPoint;
int Config::nAnchors = DEFAULT_ANCHORS;
TYPE Config::minLengthEdge = FLT_MAX;

// Initialize window dimensions.
int Config::iMinX = DEFAULT_MIN_X;
int Config::iMaxX = DEFAULT_MAX_X;
int Config::iMinY = DEFAULT_MIN_Y;
int Config::iMaxY = DEFAULT_MAX_Y;

// Cluster set parameters.
int Config::nClusters = DEFAULT_NCLUSTERS;
int Config::clusterRadius = DEFAULT_CLUSTER_RADIUS;


/***********************************************************************************************************************
* Public methods
***********************************************************************************************************************/
/***************************************************************************
* Name: 	getScreenCoordinates
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	returns the screen coordinates if set in configuration file
* 				or default value i.o.c.
***************************************************************************/
void Config::getScreenCoordinates(int &minX, int &minY, int &maxX, int &maxY)
{
    // Get screen coordinates.
    minX = Config::iMinX;
    maxX = Config::iMaxX;
    minY = Config::iMinY;
    maxY = Config::iMaxY;
}


bool Config::readConfig(const string &strFileName)
{
    bool isSuccess=true;				// Return value.

    if (!strFileName.empty())
    {
        strConfigFileName = strFileName;
    }

    // Initialize values to default.
    Config::setDefaultConfig();

	// Open configuration file.
    ifstream ifs;
	ifs.open(strConfigFileName, ios::in);

	// Check file is opened.
	if (ifs.is_open())
	{
		// Read lines until end of file.
        string strLine;
		while (getline(ifs, strLine))
		{
            size_t pos = 0;
            const string strSeparator = ",";
            string strField;
            string strValue;
            TYPE newValue=0.0;

            // Trim line and skip blank lines.
			strLine = trim(strLine);
			if (strLine.length() > 0)
			{
				// PENDING_REFACTOR: Create function that gets the value.
				// Extract field and value.
                const string strDelimiter = "=";
				while ((pos = strLine.find(strDelimiter)) != std::string::npos)
				{
                    strField = strLine.substr(0, pos);
					strLine.erase(0, pos + strDelimiter.length());
				}
                strValue = strLine;

				// PENDING_REFACTOR: Create a function that gets the parameter
				// option as an integer and remove elfis by a switch.
				if (strField == N_POINTS_PARAM)
				{
					// Get input value.
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> Config::nPoints;
				}
				else if (strField == INPUT_FLAT_FILE_PARAM)
				{
					// Get input value.
					Config::inFlatFileName = strValue;
				}
				else if (strField == INPUT_DCEL_FILE_PARAM)
				{
					// Get input value.
					Config::inDcelFileName = strValue;
				}
				else if (strField == INPUT_GRAPH_FILE_PARAM)
				{
					// Get input value.
					Config::inGraphFileName = strValue;
				}
//				else if (strField == INPUT_VORONOI_FILE_PARAM)
//				{
//					// Get input value.
//					Config::inVoronoiFileName = strValue;
//				}
//				else if (strField == INPUT_GABRIEL_FILE_PARAM)
//				{
//					// Get input value.
//					Config::inGabrielFileName = strValue;
//				}
				else if (strField == OUTPUT_FLAT_FILE_PARAM)
				{
					// Get input value.
					Config::outFlatFileName = strValue;
				}
				else if (strField == OUTPUT_DCEL_FILE_PARAM)
				{
					// Get input value.
					Config::outDcelFileName = strValue;
				}
				else if (strField == OUTPUT_GRAPH_FILE_PARAM)
				{
					// Get input value.
					Config::outGraphFileName = strValue;
				}
				else if (strField == OUTPUT_VORONOI_FILE_PARAM)
				{
					// Get input value.
					Config::outVoronoiFileName = strValue;
				}
				else if (strField == OUTPUT_GABRIEL_FILE_PARAM)
				{
					// Get input value.
					Config::outGabrielFileName = strValue;
				}
				else if (strField == N_ANCHORS_PARAM)
				{
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> Config::nAnchors;
				}
				// Get point to locate.
				else if (strField == CLOSEST_POINT_PARAM)
				{
					// Extract two values.
					strField = strValue;
					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> newValue;
					Config::closestPoint.setX(newValue);
                    strValue = strField;
					std::istringstream ss2(strValue);
					ss2.imbue(std::locale::classic());
					ss2 >> newValue;
					Config::closestPoint.setY(newValue);
				}
				// Get origin point of the line.
				else if (strField == ORIGIN_POINT_PARAM)
				{
					// Extract two values.
					strField = strValue;
					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> newValue;
					Config::originPoint.setX(newValue);
                    strValue = strField;
					std::istringstream ss2(strValue);
					ss2.imbue(std::locale::classic());
					ss2 >> newValue;
					Config::originPoint.setY(newValue);
				}
				// Get destination point of the line.
				else if (strField == DESTINATION_POINT_PARAM)
				{
					// Extract two values.
					strField = strValue;
					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> newValue;
					Config::destinationPoint.setX(newValue);
                    strValue = strField;
					std::istringstream ss2(strValue);
					ss2.imbue(std::locale::classic());
					ss2 >> newValue;
					Config::destinationPoint.setY(newValue);
				}
				// Check REMOVE_LOWER_EDGES parameter.
				else if (strField == MIN_LENGTH_EDGE_PARAM)
				{
					// Get input value.
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> Config::minLengthEdge;
				}
				// Check ZOOM parameter.
				else if (strField == ZOOM_PARAM)
				{
					// Extract two values.
					strField = strValue;
					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> Config::iMinX;

					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss2(strValue);
					ss2.imbue(std::locale::classic());
					ss2 >> Config::iMaxX;

					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss3(strValue);
					ss3.imbue(std::locale::classic());
					ss3 >> Config::iMinY;

                    strValue = strField;
					std::istringstream ss4(strValue);
					ss4.imbue(std::locale::classic());
					ss4 >> Config::iMaxY;
				}
				else if (strField == CLUSTER_SET_PARAM)
				{
					// Extract two values.
					strField = strValue;
					pos = strField.find(strSeparator);
                    strValue = strField.substr(0, pos);
					strField.erase(0, pos + strSeparator.length());
					std::istringstream ss(strValue);
					ss.imbue(std::locale::classic());
					ss >> Config::nClusters;
                    strValue = strField;
					std::istringstream ss2(strValue);
					ss2.imbue(std::locale::classic());
					ss2 >> Config::clusterRadius;
				}
				// Field does not exist.
				else if (strField.length() > 0)
				{
					std::cout << "Unknown parameter. Skipping line " << strLine << std:: endl;
                    isSuccess = FAILURE;
				}
			}
		}

		// Close file.
		ifs.close();
	}
	else
	{
		Logging::buildText(__FILE__, __FUNCTION__, "Cannot open configuration file ");
		Logging::buildText(__FILE__, __FUNCTION__, strFileName);
		Logging::write(true, Warning);
		Logging::buildText(__FILE__, __FUNCTION__, "Applying default values.");
		Logging::write(true, Warning);
        isSuccess = FAILURE;
	}

	return isSuccess;
}

/***********************************************************************************************************************
* Private methods
***********************************************************************************************************************/
/***************************************************************************
* Name: setDefaultConfig
* IN:	N/A
* OUT:	N/A
* Description: Sets default values in configuration parameters.
***************************************************************************/
void Config::setDefaultConfig()
{
    // Set default parameters.
    Config::nPoints = DEFAULT_N_POINTS;
    Config::inFlatFileName = DEFAULT_INPUT_FLAT_FILENAME;
    Config::inDcelFileName = DEFAULT_INPUT_DCEL_FILENAME;
    Config::inGraphFileName = DEFAULT_INPUT_GRAPH_FILENAME;
//    Config::inVoronoiFileName = DEFAULT_INPUT_VORONOI_FILENAME;
//    Config::inGabrielFileName = DEFAULT_INPUT_GABRIEL_FILENAME;
    Config::outFlatFileName = DEFAULT_OUTPUT_FLAT_FILENAME;
    Config::outDcelFileName = DEFAULT_OUTPUT_DCEL_FILENAME;
    Config::outGraphFileName = DEFAULT_OUTPUT_GRAPH_FILENAME;
    Config::outVoronoiFileName = DEFAULT_OUTPUT_VORONOI_FILENAME;
    Config::outGabrielFileName = DEFAULT_OUTPUT_GABRIEL_FILENAME;
    Config::closestPoint = Point<TYPE>(INVALID, INVALID);
    Config::originPoint = Point<TYPE>(INVALID, INVALID);
    Config::destinationPoint = Point<TYPE>(INVALID, INVALID);
    Config::nAnchors = DEFAULT_ANCHORS;
    Config::minLengthEdge = FLT_MAX;

    // Initialize window dimensions.
    Config::iMinX = DEFAULT_MIN_X;
    Config::iMaxX = DEFAULT_MAX_X;
    Config::iMinY = DEFAULT_MIN_Y;
    Config::iMaxY = DEFAULT_MAX_Y;

    // Cluster set parameters.
    Config::nClusters = DEFAULT_NCLUSTERS;
    Config::clusterRadius = DEFAULT_CLUSTER_RADIUS;
}
