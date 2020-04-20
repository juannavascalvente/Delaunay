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
    static int		nPoints;			// # points in set.
    static string	strConfigFileName;  // Configuration file name.

    static string	inFlatFileName;		// Flat output points file name.
    static string	outFlatFileName;	// DCEL input data file name.

    static string	inDcelFileName;		// DCEL input data file name.
    static string	outDcelFileName;	// DCEL output data file name.

    static string	inGraphFileName;	// Graph input data file name.
    static string	outGraphFileName;	// Graph output data file name.

//    static string	inVoronoiFileName;	// Voronoi input data file name.
    static string	outVoronoiFileName;	// Voronoi output data file name.

//    static string	inGabrielFileName;	// Gabriel input data file name.
    static string	outGabrielFileName;	// Gabriel output data file name.

    static Point<TYPE>	closestPoint;
    static TYPE		    minLengthEdge;

    static Point<TYPE>	originPoint;		// Line origin point.
    static Point<TYPE>	destinationPoint;	// Line destination point.

    static int		nClusters;		    // # clusters in set when generating cluster set.
    static TYPE 	clusterRadius;	    // Cluster radius.

    static int		nAnchors;		    // # anchors to locate point.

    static int		iMinX;				// Min value in x-coordinates.
    static int		iMaxX;				// Max value in x-coordinates.
    static int		iMinY;				// Min value in y-coordinates.
    static int		iMaxY;				// Max value in y-coordinates.

    static string   strExtraOutput;     // Extra output file name

    static void setDefaultConfig();

public:
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
    static bool readConfig(const string &strFileName="");

	/*******************************************************************************************************************
	 *  GET functions.
	 ******************************************************************************************************************/
	static void getScreenCoordinates( int &minX, int &minY, int &maxX, int &maxY);

	static inline Point<TYPE> getClosestPoint() { return(Config::closestPoint); };
    static inline int getNPoints()  { return(Config::nPoints); };
    static inline int getNClusters(){ return(Config::nClusters); };
	static inline int getNAnchors() { return(Config::nAnchors); };
	static inline TYPE getRadius()   { return(Config::clusterRadius); };
	static inline string getInFlatFilename()    { return(Config::inFlatFileName); };
	static inline string getInDCELFilename()    { return(Config::inDcelFileName); };
	static inline string getInGraphFilename()   { return(Config::inGraphFileName); };
//	static inline string getInVoronoiFilename() { return(Config::inVoronoiFileName); };
//	static inline string getInGabrielFilename() { return(Config::inGabrielFileName); };
	static inline string getOutFlatFilename()   { return(Config::outFlatFileName); };
	static inline string getOutDCELFilename()   { return(Config::outDcelFileName); };
	static inline string getOutGraphFilename()  { return(Config::outGraphFileName); };
	static inline string getOutVoronoiFilename() { return(Config::outVoronoiFileName); };
	static inline string getOutGabrielFilename() { return(Config::outGabrielFileName); };
	static inline TYPE getMinLengthEdge()    { return (Config::minLengthEdge);}
    static Point<TYPE> getDestinationPoint() { return Config::destinationPoint;}
    static Point<TYPE> getOriginPoint()      { return Config::originPoint;}
    static inline string getExtraOutputFilename()   { return(Config::strExtraOutput); };
};

#endif /* CONFIG_H_ */
