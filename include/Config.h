/*
 * Config.h
 *
 *  Created on: Jul 4, 2016
 *      Author: juan
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "defines.h"
#include "Point.h"

//****************************************************************************
//                           	CONFIG CLASS
//****************************************************************************
class Config
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int		nPoints;			// # points in set.
	string	configFileName;		// Configuration file name.

	string	inFlatFileName;		// Flat output points file name.
	string	outFlatFileName;	// DCEL input data file name.

	string	inDcelFileName;		// DCEL input data file name.
	string	outDcelFileName;	// DCEL output data file name.

	string	inGraphFileName;	// Graph input data file name.
	string	outGraphFileName;	// Graph output data file name.

	string	inVoronoiFileName;	// Voronoi input data file name.
	string	outVoronoiFileName;	// Voronoi output data file name.

	string	inGabrielFileName;	// Gabriel input data file name.
	string	outGabrielFileName;	// Gabriel output data file name.

	Point<TYPE>	closestPoint;
	TYPE		minLengthEdge;

	Point<TYPE>	originPoint;		// Line origin point.
	Point<TYPE>	destinationPoint;	// Line destination point.

	int		nClusters;		// # clusters in set when generating cluster set.
	int 	clusterRadius;	// Cluster radius.

	int		nAnchors;		// # anchors to locate point.

	bool	initialized;
	int		minX;				// Min value in x-coordinates.
	int		maxX;				// Max value in x-coordinates.
	int		minY;				// Min value in y-coordinates.
	int		maxY;				// Max value in y-coordinates.

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Config(string fileName);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void setDefaultConfig(void);
	int  readConfig();

	// GET functions.
	void getScreenCoordinates( int &minX, int &minY, int &maxX, int &maxY);
	inline Point<TYPE> getClosestPoint() { return(this->closestPoint); };
	inline int getNPoints() { return(this->nPoints); };
	inline int getNClusters( ) { return(this->nClusters); };
	inline int getNAnchors() { return(this->nAnchors); };
	inline int getRadius() { return(this->clusterRadius); };
	inline string getInFlatFilename() { return(this->inFlatFileName); };
	inline string getInDCELFilename() { return(this->inDcelFileName); };
	inline string getInGraphFilename() { return(this->inGraphFileName); };
	inline string getInVoronoiFilename() { return(this->inVoronoiFileName); };
	inline string getInGabrielFilename() { return(this->inGabrielFileName); };
	inline string getOutFlatFilename() { return(this->outFlatFileName); };
	inline string getOutDCELFilename() { return(this->outDcelFileName); };
	inline string getOutGraphFilename() { return(this->outGraphFileName); };
	inline string getOutVoronoiFilename() { return(this->outVoronoiFileName); };
	inline string getOutGabrielFilename() { return(this->outGabrielFileName); };
	inline TYPE getMinLengthEdge() {return (this->minLengthEdge);}
	Point<TYPE> getDestinationPoint() const {return this->destinationPoint;}
	Point<TYPE> getOriginPoint() const {return this->originPoint;}
};

#endif /* CONFIG_H_ */
