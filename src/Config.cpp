/*
 * Config.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: juan
 */

#include "Config.h"
#include <errno.h>
#include <float.h>
#include "generic.h"
#include "Logging.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#ifdef DEBUG_GEOMETRICAL
//#define DEBUG_PARSE_ZOOM
//#define DEBUG_READ_CONFIG
#endif

/**************************************************************************
* Defines section
**************************************************************************/
#define	DEFAULT_N_POINTS				10
#define	DEFAULT_INPUT_FLAT_FILENAME		"inFlat.txt"
#define	DEFAULT_INPUT_DCEL_FILENAME		"inDCEL.txt"
#define	DEFAULT_INPUT_GRAPH_FILENAME	"inGraph.txt"
#define DEFAULT_INPUT_VORONOI_FILENAME	"inVoronoi.txt"
#define DEFAULT_INPUT_GABRIEL_FILENAME	"inGabriel.txt"
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
#define DEFAULT_CLUSTER_RADIUS				100

// Fields in configuration file.
#define N_POINTS_PARAM				"N_POINTS"
#define INPUT_FLAT_FILE_PARAM		"INPUT_FLAT_FILE"
#define INPUT_DCEL_FILE_PARAM		"INPUT_DCEL_FILE"
#define INPUT_GRAPH_FILE_PARAM		"INPUT_GRAPH_FILE"
#define INPUT_VORONOI_FILE_PARAM	"INPUT_VORONOI_FILE"
#define INPUT_GABRIEL_FILE_PARAM	"INPUT_GABRIEL_FILE"
#define OUTPUT_FLAT_FILE_PARAM		"OUTPUT_FLAT_FILE"
#define OUTPUT_DCEL_FILE_PARAM		"OUTPUT_DCEL_FILE"
#define OUTPUT_GRAPH_FILE_PARAM		"OUTPUT_GRAPH_FILE"
#define OUTPUT_VORONOI_FILE_PARAM	"OUTPUT_VORONOI_FILE"
#define OUTPUT_GABRIEL_FILE_PARAM	"OUTPUT_GABRIEL_FILE"
#define CLOSEST_POINT_PARAM			"CLOSEST_POINT"
#define N_ANCHORS_PARAM				"N_ANCHORS"
#define MIN_LENGTH_EDGE_PARAM		"MIN_LENGTH_EDGE"
#define ZOOM_PARAM					"ZOOM"
#define CLUSTER_SET_PARAM			"CLUSTER_SET"

#define	PARAMETERS_NPOINTS		1
#define	PARAMETERS_IN_FLAT		2
#define	PARAMETERS_IN_DCEL		3
#define	PARAMETERS_OUT_FLAT		4
#define	PARAMETERS_OUT_DCEL		5
#define	PARAMETERS_CLOSEST		6
#define	PARAMETERS_MIN_EDGE_LEN	7
#define	PARAMETERS_ZOOM			8
#define	PARAMETERS_CLUSTER		9
#define	PARAMETERS_NANCHORS		10
#define	PARAMETERS_IN_GRAPH		11
#define	PARAMETERS_OUT_GRAPH	12
#define	PARAMETERS_IN_VORONOI	13
#define	PARAMETERS_OUT_VORONOI	14
#define	PARAMETERS_IN_GABRIEL	15
#define	PARAMETERS_OUT_GABRIEL	16
#define	PARAMETERS_ALL			100

#define MAX_LINE_LENGTH		200


/***************************************************************************
* Private functions headers.
***************************************************************************/
int	parse_Int(char *strValue, int *value);
int	parse_TYPE(char *strValue, TYPE *value);
int	parse_Two_Int(char *value, int *value1, int *value2);
int	parse_Point(char *value, Point<TYPE> *p);
int parse_Zoom(char *value, int *min_X, int *max_X, int *min_Y, int *max_Y);

Config::Config(string fileName)
{
	// Set default parameters.
	this->initialized = true;
	this->nPoints = DEFAULT_N_POINTS;
	this->configFileName = fileName;
	this->inFlatFileName = DEFAULT_INPUT_FLAT_FILENAME;
	this->inDcelFileName = DEFAULT_INPUT_DCEL_FILENAME;
	this->inGraphFileName = DEFAULT_INPUT_GRAPH_FILENAME;
	this->inVoronoiFileName = DEFAULT_INPUT_VORONOI_FILENAME;
	this->inGabrielFileName = DEFAULT_INPUT_GABRIEL_FILENAME;
	this->outFlatFileName = DEFAULT_OUTPUT_FLAT_FILENAME;
	this->outDcelFileName = DEFAULT_OUTPUT_DCEL_FILENAME;
	this->outGraphFileName = DEFAULT_OUTPUT_GRAPH_FILENAME;
	this->outVoronoiFileName = DEFAULT_OUTPUT_VORONOI_FILENAME;
	this->outGabrielFileName = DEFAULT_OUTPUT_GABRIEL_FILENAME;
	this->closestPoint = Point<TYPE>(INVALID, INVALID);
	this->nAnchors = INVALID;
	this->minLengthEdge = FLT_MAX;

	// Initialize window dimensions.
	this->minX = DEFAULT_MIN_X;
	this->maxX = DEFAULT_MAX_X;
	this->minY = DEFAULT_MIN_Y;
	this->maxY = DEFAULT_MAX_Y;

	// Cluster set parameters.
	this->nClusters = DEFAULT_NCLUSTERS;
	this->clusterRadius = DEFAULT_CLUSTER_RADIUS;

	this->readConfig();
}


/***************************************************************************
* Name: setDefaultConfig
* IN:	N/A
* OUT:	N/A
* Description: Sets default values in configuration parameters.
***************************************************************************/
void	Config::setDefaultConfig(void)
{
	// Set default parameters.
	this->nPoints = DEFAULT_N_POINTS;
	this->inFlatFileName = DEFAULT_INPUT_FLAT_FILENAME;
	this->inDcelFileName = DEFAULT_INPUT_DCEL_FILENAME;
	this->inGraphFileName = DEFAULT_INPUT_GRAPH_FILENAME;
	this->inVoronoiFileName = DEFAULT_INPUT_VORONOI_FILENAME;
	this->inGabrielFileName = DEFAULT_INPUT_GABRIEL_FILENAME;
	this->outFlatFileName = DEFAULT_OUTPUT_FLAT_FILENAME;
	this->outDcelFileName = DEFAULT_OUTPUT_DCEL_FILENAME;
	this->outGraphFileName = DEFAULT_OUTPUT_GRAPH_FILENAME;
	this->outVoronoiFileName = DEFAULT_OUTPUT_VORONOI_FILENAME;
	this->outGabrielFileName = DEFAULT_OUTPUT_GABRIEL_FILENAME;
	this->closestPoint = Point<TYPE>(INVALID, INVALID);
	this->nAnchors = DEFAULT_ANCHORS;
	this->minLengthEdge = FLT_MAX;

	// Initialize window dimensions.
	this->minX = DEFAULT_MIN_X;
	this->maxX = DEFAULT_MAX_X;
	this->minY = DEFAULT_MIN_Y;
	this->maxY = DEFAULT_MAX_Y;

	// Cluster set parameters.
	this->nClusters = DEFAULT_NCLUSTERS;
	this->clusterRadius = DEFAULT_CLUSTER_RADIUS;
}

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
	if (this->initialized)
	{
		// Get screen coordinates.
		minX = this->minX;
		maxX = this->maxX;
		minY = this->minY;
		maxY = this->maxY;
	}
	else
	{
		// Get default screen coordinates.
		minX = DEFAULT_MIN_X;
		maxX = DEFAULT_MAX_X;
		minY = DEFAULT_MIN_Y;
		maxY = DEFAULT_MAX_Y;
	}
}


/***************************************************************************
* Name: 	readConfig
* IN:		NONE
* OUT:		NONE
* IN/OUT:	NONE
* GLOBAL:	"Config" object updated.
* RETURN:	true if file exists and parameters are ok. false i.o.c.
* Description: 	reads the configuration file parameters and updates "Config"
* 				object. If file does not exists then set default values.
* 				If a wrong file is found then it is skipped and current
* 				field value is kept.
***************************************************************************/
int 	Config::readConfig()
{
	int	 ret=SUCCESS;				// Return value.
	string	line = "";
	string delimiter = "=";
	string separator = ",";
	size_t pos = 0;
	std::string field;
	std::string value;
	ifstream ifs;
	TYPE newValue=0.0;

	// Open configuration file.
	ifs.open(this->configFileName.c_str(), ios::in);

	// Check file is opened.
	if (ifs.is_open())
	{
		// Read lines until end of file.
		while (getline(ifs, line))
		{
			// Trim line and skip blank lines.
			line = trim(line);
			if (line.length() > 0)
			{
				// PENDING_REFACTOR: Create function that gets the value.
				// Extract field and value.
				while ((pos = line.find(delimiter)) != std::string::npos)
				{
					field = line.substr(0, pos);
					line.erase(0, pos + delimiter.length());
				}
				value = line;

				// PENDING_REFACTOR: Create a function that gets the parameter
				// option as an integer and remove elfis by a switch.
				if (field.compare(N_POINTS_PARAM) == 0)
				{
					// Get input value.
					std::istringstream ss(value);
					ss.imbue(std::locale::classic());
					ss >> this->nPoints;
				}
				else if (field.compare(INPUT_FLAT_FILE_PARAM) == 0)
				{
					// Get input value.
					this->inFlatFileName = value;
				}
				else if (field.compare(INPUT_DCEL_FILE_PARAM) == 0)
				{
					// Get input value.
					this->inDcelFileName = value;
				}
				else if (field.compare(INPUT_GRAPH_FILE_PARAM) == 0)
				{
					// Get input value.
					this->inGraphFileName = value;
				}
				else if (field.compare(INPUT_VORONOI_FILE_PARAM) == 0)
				{
					// Get input value.
					this->inVoronoiFileName = value;
				}
				else if (field.compare(INPUT_GABRIEL_FILE_PARAM) == 0)
				{
					// Get input value.
					this->inGabrielFileName = value;
				}
				else if (field.compare(OUTPUT_FLAT_FILE_PARAM) == 0)
				{
					// Get input value.
					this->outFlatFileName = value;
				}
				else if (field.compare(OUTPUT_DCEL_FILE_PARAM) == 0)
				{
					// Get input value.
					this->outDcelFileName = value;
				}
				else if (field.compare(OUTPUT_GRAPH_FILE_PARAM) == 0)
				{
					// Get input value.
					this->outGraphFileName = value;
				}
				else if (field.compare(OUTPUT_VORONOI_FILE_PARAM) == 0)
				{
					// Get input value.
					this->outVoronoiFileName = value;
				}
				else if (field.compare(OUTPUT_GABRIEL_FILE_PARAM) == 0)
				{
					// Get input value.
					this->outGabrielFileName = value;
				}
				else if (field.compare(N_ANCHORS_PARAM) == 0)
				{
					std::istringstream ss(value);
					ss.imbue(std::locale::classic());
					ss >> this->nAnchors;
				}
				// Get # anchors to locate point.
				else if (field.compare(CLOSEST_POINT_PARAM) == 0)
				{
					// Extract two values.
					field = value;
					pos = field.find(separator);
					value = field.substr(0, pos);
					field.erase(0, pos + separator.length());
					std::istringstream ss(value);
					ss.imbue(std::locale::classic());
					ss >> newValue;
					this->closestPoint.setX(newValue);
					value = field;
					std::istringstream ss2(value);
					ss2.imbue(std::locale::classic());
					ss2 >> newValue;
					this->closestPoint.setY(newValue);
				}
				// Check REMOVE_LOWER_EDGES parameter.
				else if (field.compare(MIN_LENGTH_EDGE_PARAM) == 0)
				{
					// Get input value.
					std::istringstream ss(value);
					ss.imbue(std::locale::classic());
					ss >> this->minLengthEdge;
				}
				// Check ZOOM parameter.
				else if (field.compare(ZOOM_PARAM) == 0)
				{
					// Extract two values.
					field = value;
					pos = field.find(separator);
					value = field.substr(0, pos);
					field.erase(0, pos + separator.length());
					std::istringstream ss(value);
					ss.imbue(std::locale::classic());
					ss >> newValue;
					this->minX = newValue;


					pos = field.find(separator);
					value = field.substr(0, pos);
					field.erase(0, pos + separator.length());
					std::istringstream ss2(value);
					ss2.imbue(std::locale::classic());
					ss2 >> newValue;
					this->maxX = newValue;

					pos = field.find(separator);
					value = field.substr(0, pos);
					field.erase(0, pos + separator.length());
					std::istringstream ss3(value);
					ss3.imbue(std::locale::classic());
					ss3 >> newValue;
					this->minY = newValue;


					value = field;
					std::istringstream ss4(value);
					ss4.imbue(std::locale::classic());
					ss4 >> newValue;
					this->maxY = newValue;
				}
				else if (field.compare(CLUSTER_SET_PARAM) == 0)
				{
					// Extract two values.
					field = value;
					pos = field.find(separator);
					value = field.substr(0, pos);
					field.erase(0, pos + separator.length());
					std::istringstream ss(value);
					ss.imbue(std::locale::classic());
					ss >> newValue;
					this->nClusters = newValue;
					value = field;
					std::istringstream ss2(value);
					ss2.imbue(std::locale::classic());
					ss2 >> newValue;
					this->clusterRadius = newValue;
				}
				// Field does not exist.
				else if (field.length() > 0)
				{
					std::cout << "Unknown parameter. Skipping line " << line << std:: endl;
					ret = FAILURE;
				}
			}
		}

		// Close file.
		ifs.close();
	}
	else
	{
		Logging::buildText(__FILE__, __FUNCTION__, "Cannot open configuration file ");
		Logging::buildText(__FILE__, __FUNCTION__, this->configFileName);
		Logging::write(true, Warning);
		Logging::buildText(__FILE__, __FUNCTION__, "Applying default values.");
		Logging::write(true, Warning);
		this->setDefaultConfig();
		ret = FAILURE;
	}

#ifdef DEBUG_READ_CONFIG
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "Configuration parameters:" << this->configFileName << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "# points:\t\t\t" << this->nPoints << std::endl;
	std::cout << "Input flat:\t\t\t" << this->inFlatFileName << std::endl;
	std::cout << "Input DCEL:\t\t\t" << this->inDcelFileName << std::endl;
	std::cout << "Input graph:\t\t\t" << this->inGraphFileName << std::endl;
	std::cout << "Input voronoi:\t\t\t" << this->inVoronoiFileName << std::endl;
	std::cout << "Output flat:\t\t\t" << this->outFlatFileName << std::endl;
	std::cout << "Output DCEL:\t\t\t" << this->outDcelFileName << std::endl;
	std::cout << "Output graph:\t\t\t" << this->outDcelFileName << std::endl;
	std::cout << "Output voronoi:\t\t\t" << this->outVoronoiFileName << std::endl;
	std::cout << "Closest point:\t\t\t" << this->closestPoint << std::endl;
	std::cout << "# anchors:\t\t\t" << this->nAnchors << std::endl;
	std::cout << "Minimum edge length:\t\t" << this->minLengthEdge << std::endl;
	std::cout << "Window size from \t\t(" << this->min_X << "," << this->min_Y <<
			") to (" << this->max_X << "," << this->max_Y << ")." << std::endl;
#endif

	return(ret);
}

/***************************************************************************
* Private functions bodies.
***************************************************************************/
int	parse_Int(char *strValue, int *value)
{
	int ret=SUCCESS;		// Return value.

	// Convert string into value.
	(*value) = strtol(strValue, (char **) NULL, 10);

	// Check conversion error.
	if (errno == ERANGE)
	{
		printf("Error parsing integer value %s\n", strValue);
		ret = FAILURE;
	}

	return(ret);
}

int	parse_TYPE(char *strValue, TYPE *value)
{
	int ret=SUCCESS;		// Return value.
	char* pEnd;

	// Convert string into value.
	(*value) = strtod(strValue, &pEnd);

	// Check conversion error.
	if (errno == ERANGE)
	{
		printf("Error parsing TYPE value %s\n", strValue);
		ret = FAILURE;
	}

	return(ret);
}

int	parse_Two_Int(char *value, int *value1, int *value2)
{
	int ret=SUCCESS;		// Return value.
	char *pch=NULL;
	int	length=0;
	char field[MAX_LINE_LENGTH];

#ifdef DEBUG_PARSE_POINT
	printf("Input line is %s\n", value);
#endif

	// Get pointer to ',' character.
	pch = strchr(value,',');

	// Check if character exists in input string.
	if (pch == NULL)
	{
		ret = FAILURE;
#ifdef DEBUG_PARSE_POINT
		printf("Input line %s does not contain ,\n", value);
#endif
	}
	else
	{
		length = pch - value;

#ifdef DEBUG_PARSE_POINT
		printf("Input line contains , at position %d\n", length);
#endif
	    // Copy field.
		strncpy(field, value, length);
		if (parse_Int(field, value1) == FAILURE)
		{
			printf("Error parsing first integer value %s\n", field);
		}
		else
		{
#ifdef DEBUG_PARSE_POINT
		printf("Input line field is %d\n", value1);
#endif
			// Copy value.
			strncpy(field, &value[length + 1], strlen(value) - length - 1);
			if (parse_Int(field, value2) == FAILURE)
			{
				printf("Error parsing second integer value %s\n", field);
			}
		}
#ifdef DEBUG_PARSE_POINT
		printf("Input line value is %d\n", value2);
#endif
	}

	return(ret);
}

int	parse_Point(char *value, Point<TYPE> *p)
{
	int ret=SUCCESS;		// Return value.
	char *pch=NULL;
	int	length=0;
	char field[MAX_LINE_LENGTH];
	TYPE value1;
	TYPE value2;

#ifdef DEBUG_PARSE_POINT
	printf("Input line is %s\n", value);
#endif

	// Get pointer to ',' character.
	pch = strchr(value,',');

	// Check if character exists in input string.
	if (pch == NULL)
	{
		ret = FAILURE;
#ifdef DEBUG_PARSE_POINT
		printf("Input line %s does not contain ,\n", value);
#endif
	}
	else
	{
		length = pch - value;

#ifdef DEBUG_PARSE_POINT
		printf("Input line contains , at position %d\n", length);
#endif
	    // Copy field.
		strncpy(field, value, length);
		if (parse_TYPE(field, &value1) == FAILURE)
		{
			printf("Error parsing first TYPE value %s\n", field);
		}
		else
		{
#ifdef DEBUG_PARSE_POINT
			printf("Input line field is %f\n", value1);
#endif
			// Copy value.
			strncpy(field, &value[length + 1], strlen(value) - length - 1);
			if (parse_TYPE(field, &value2) == FAILURE)
			{
				printf("Error parsing second TYPE value %s\n", field);
			}
			else
			{
				p->setX(value1);
				p->setY(value2);
#ifdef DEBUG_PARSE_POINT
				printf("Input line value is %f\n", value2);
#endif
			}
		}
	}

	return(ret);
}

int parse_Zoom(char *line, int *min_X, int *max_X, int *min_Y, int *max_Y)
{
	int 	ret=SUCCESS;			// Return value.
	char 	*pch=NULL;				// Pointer to character in string.
	int		nItems=0;				// # values read.

#ifdef DEBUG_PARSE_ZOOM
	printf("Input line is %s\n", line);
#endif

	// Get next pointer to ',' character.
	nItems = 0;
	pch = strtok (line,"=");
	pch = strtok (line,",");
	ret = parse_Int(pch, min_X);
	while (((pch = strtok (NULL,",")) != NULL) && (ret == SUCCESS))
	{
#ifdef DEBUG_PARSE_ZOOM
		printf ("%s\n", pch);
#endif
		// Check parameter to set.
		switch (nItems)
		{
			case 0:
			{
				if (parse_Int(pch, max_X) == FAILURE)
				{
					ret = FAILURE;
					printf("Error parsing max x coordinate string %s\n", pch);
				}
				break;
			}
			case 1:
			{
				if (parse_Int(pch, min_Y) == FAILURE)
				{
					ret = FAILURE;
					printf("Error parsing min Y coordinate string %s\n", pch);
				}
				break;
			}
			case 2:
			{
				if (parse_Int(pch, max_Y) == FAILURE)
				{
					ret = FAILURE;
					printf("Error parsing max Y coordinate string %s\n", pch);
				}
				break;
			}
			default:
			{
				printf("Error\n");
				break;
			}
		}

		nItems++;
	}

	if (nItems != 3)
	{
		ret = FAILURE;
#ifdef DEBUG_PARSE_ZOOM
		printf("Input line %s does not contain 4 points separated by \",\" character\n", line);
#endif
	}
#ifdef DEBUG_PARSE_ZOOM
	else
	{
		printf("New area from (%lf,%lf) to (%lf,%lf)\n", *min_X, *min_Y, *max_X, *max_Y);
	}
#endif

	return(ret);
}

