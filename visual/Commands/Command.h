#ifndef DELAUNAY_COMMAND_H
#define DELAUNAY_COMMAND_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "CommandParamIn.h"
#include "CommandResult.h"
#include "Config.h"
#include "DcelGenerator.h"
#include "DcelReader.h"
#include "DcelWriter.h"
#include "DelaunayIO.h"
#include "VoronoiIO.h"
#include "GabrielIO.h"
#include "DcelFigureBuilder.h"
#include "LineFactory.h"
#include "PointFactory.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Command
{
protected:

    /*******************************************************************************************************************
    * Protected class members
    *******************************************************************************************************************/
    bool isSuccess;
    CommandResult *result;

    /*******************************************************************************************************************
    * Protected class methods
    *******************************************************************************************************************/
    virtual bool isRunnable() { return true; };
    virtual void printRunnableMsg() {};
    virtual CommandResult * runCommand() { return createResult(); };
    virtual CommandResult* createResult() { return new CommandResultNull(false); };

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    Command() : isSuccess(false), result(nullptr) {};
    virtual ~Command() { delete result; };

    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
    bool getSuccess() const { return isSuccess; }
    CommandResult *getResult() const { return result;}

    /**
     * @fn      run
     * @brief   runs command if command can be executed
     *
     * @return  true if command was ran
     *          false otherwise
     */
    void run()
    {
        isSuccess = false;

        // Check command can be executed
        if (!isRunnable())
        {
            printRunnableMsg();
            result = this->createResult();
        }
        else
        {
            // Run command
            result = runCommand();
        }
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandNull : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandNull() = default;

    /**
     * @fn      isRunnable
     * @brief   Null command cannnot execute
     *
     * @return  false
     */
    bool isRunnable() override  { return false; };

    /**
     * @fn      runCommand
     * @brief   does not execute anything. Command when there is nothing to run
     *
     * @return  true
     */
    CommandResult * runCommand() override
    {
        return new CommandResultNull(false);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadConfig : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandReadConfig() = default;

    /**
     * @fn      runCommand
     * @brief   reads comfigutation file
     *
     * @return  true if file read successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Run command
        this->isSuccess = Config::readConfig();

        // Build result
        return new CommandResultNull(this->isSuccess);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandGenerateRandom : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    GeneratorCmdParamIn in;
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandGenerateRandom(GeneratorCmdParamIn &inParam) : in(inParam) {}

    /**
     * @fn      isRunnable
     * @brief   Checks if number of points higher than 0
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Number of points higher than 0
        return (in.getNumPoints() > 0);
    };

    /**
     * @fn      printRunnableMsg
     * @brief   Prints message if command cannot be ran
     */
    void printRunnableMsg() override
    {
        // Check number of points
        if (in.getNumPoints() == 0)
        {
            cout << "Number of points to generate is zero" << endl;
        }
    };

    /**
     * @fn      run
     * @brief   Generates a random set of points
     *
     * @return  true is set generated successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        this->isSuccess = DcelGenerator::generateRandom(in.getNumPoints(), *dcel);

        // Add point display
        for (size_t i=0; i< in.getNumPoints(); i++)
        {
            vPoints.push_back(*dcel->getRefPoint(i));
        }

        // Build result
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultPoints(getSuccess(), in.getStoreService(), vPoints);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandGenerateCluster : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    GeneratorClusterCmdParamIn in;
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandGenerateCluster(GeneratorClusterCmdParamIn &inParam) : in(inParam) {}

    /**
     * @fn      isRunnable
     * @brief   Checks conditions to execute command:
     *          1.- Radius higher than 0
     *          2.- Number of clusters higher than 0
     *          3.- Number of points higher than 0
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // All values are higher than 0
        return (in.getFRadius() > 0.0) && (in.getSzNumClusters() > 0) && (in.getNumPoints() > 0);
    };

    /**
     * @fn      printRunnableMsg
     * @brief   Prints message if command cannot be ran
     */
    void printRunnableMsg() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Check radius
        if (in.getFRadius() <= 0.0)
        {
            cout << "One of the values is not higher than 0" << endl;
        }

        // Check number of clusters
        if (in.getSzNumClusters() == 0)
        {
            cout << "Number of cluster to generate is zero" << endl;
        }

        // Check number of points
        if (in.getNumPoints() == 0)
        {
            cout << "Number of points to generate is zero" << endl;
        }
    };

    /**
     * @fn      run
     * @brief   Generates a random set of points
     *
     * @return  true is set generated successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        this->isSuccess = DcelGenerator::generateClusters(in.getNumPoints(), in.getSzNumClusters(), in.getFRadius(), *dcel);

        // Add point display
        for (size_t i=0; i< in.getNumPoints(); i++)
        {
            vPoints.push_back(*dcel->getRefPoint(i));
        }

        // Build result
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultPoints(getSuccess(), in.getStoreService(), vPoints);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandStarTriangulation : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    StarTriangulationParamCmdIn in;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandStarTriangulation(StarTriangulationParamCmdIn &inParam) : in(inParam) {};

    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation has been already computed
     */
    void printRunnableMsg() override
    {
        cout << "Triangulation already computed" << endl;
    };


    /**
     * @fn      isRunnable
     * @brief   Checks nor star triangulation nor Delaunay are already displayed
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Star triangulation and Delaunay have not been already created
        return (!in.getStoreService()->getStatus()->isTriangulationCreated() &&
                !in.getStoreService()->getStatus()->isDelaunayCreated());
    };

    /**
     * @fn      run
     * @brief   Builds star triangulation
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Run command
        StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
        this->isSuccess = triangulation->build(in.getStoreService()->getDcel());

        // Build result
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultTriangulation(getSuccess(), in.getStoreService(), dcel);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandDelaunay : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandDelaunay(CmdParamIn &inParam) : in(inParam) {};

    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that Delaunay triangulation has been already computed
     */
    void printRunnableMsg() override
    {
        cout << "Triangulation already computed" << endl;
    };


    /**
     * @fn      isRunnable
     * @brief   Checks Delaunay triangulation is not already displayed
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Star triangulation and Delaunay have not been already created
        return !in.getStoreService()->getStatus()->isDelaunayCreated();
    };

    /**
     * @fn      run
     * @brief   Builds star triangulation
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Get reference to current DCEL and Delaunay
        Dcel *dcel = in.getStoreService()->getDcel();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        delaunay->setDCEL(dcel);

        // Get reference to status
        Status *status = in.getStoreService()->getStatus();

        // Build Delaunay from Star triangulation.
        if (status->isTriangulationCreated())
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            this->isSuccess = triangulation->delaunay();
            delaunay->setAlgorithm(FROM_STAR);
        }
        else
        {
            // Build Delaunay from DCEL.
            if (!status->isDelaunayCreated())
            {
                this->isSuccess = delaunay->incremental();
            }
        }

        // Run command
        StarTriangulation *triangulation = in.getStoreService()->getStarTriang();

        // Build result
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultDelaunay(getSuccess(), in.getStoreService(), dcel);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandConvexHull : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    Polygon *hull;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandConvexHull(CmdParamIn &inParam) : in(inParam), hull(nullptr) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that a triangulation is required
     */
    void printRunnableMsg() override
    {
        cout << "Triangulation was not computed computed" << endl;
    };


    /**
     * @fn      isRunnable
     * @brief   Checks Delaunay triangulation is not already displayed
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Star triangulation and Delaunay or have been already created
        return (in.getStoreService()->getStatus()->isTriangulationCreated() ||
                in.getStoreService()->getStatus()->isDelaunayCreated());
    };


    /**
     * @fn      run
     * @brief   Builds star triangulation
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Get reference to status
        Status *status = in.getStoreService()->getStatus();

        // Computing convex hull.
        if (status->isDelaunayCreated())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            this->isSuccess = delaunay->convexHull();
            hull = in.getStoreService()->getDelaunay()->getConvexHull();
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            this->isSuccess = triangulation->convexHull();
            hull = triangulation->getConvexHull();
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultPolygon(getSuccess(), in.getStoreService(), hull);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandVoronoi : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandVoronoi(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that Delaunay triangulation is required
     */
    void printRunnableMsg() override
    {
        cout << "Delaunay triangulation is required" << endl;
    };


    /**
     * @fn      isRunnable
     * @brief   Checks Delaunay triangulation has been created
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Delaunay must be created to create Voronoi diagram
        return in.getStoreService()->getStatus()->isDelaunayCreated();
    };


    /**
     * @fn      run
     * @brief   Builds star triangulation
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Initialize voronoi data.
        Dcel *dcel = in.getStoreService()->getDcel();
        Voronoi *voronoi = in.getStoreService()->getVoronoi();
        this->isSuccess = voronoi->init(dcel);

        // Check init was success
        if (this->isSuccess)
        {
            // Compute Voronoi diagram.
            this->isSuccess = voronoi->build(true);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        Voronoi *voronoi = in.getStoreService()->getVoronoi();
        return new CommandResulVoronoi(getSuccess(), in.getStoreService(), dcel, voronoi);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandGabriel : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandGabriel(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that Delaunay triangulation and Voronoi diagram are required
     */
    void printRunnableMsg() override
    {
        cout << "Delaunay triangulation and Voronoi diagram are required" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks Delaunay triangulation has been created
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Delaunay and Voronoi must exist
        Status *status = in.getStoreService()->getStatus();
        return status->isDelaunayCreated() && status->isVoronoiCreated();
    }


    /**
     * @fn      run
     * @brief   Builds Gabriel graph in a given triangulation
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Build Gabriel graph.
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        Voronoi *voronoi = in.getStoreService()->getVoronoi();
        Gabriel *gabriel = in.getStoreService()->getGabriel();
        gabriel->init(delaunay->getRefDcel(), voronoi);
        this->isSuccess = gabriel->build();

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        Gabriel *gabriel = in.getStoreService()->getGabriel();
        return new CommandResultGabriel(getSuccess(), in.getStoreService(), dcel, gabriel);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandTriangulationPath : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    Line line;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandTriangulationPath(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation path can be only computed using incremental Delaunay
     */
    void printRunnableMsg() override
    {
        cout << "Triangulation is not Delaunay or it was not computed using incremenal algorithm" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks Delaunay triangulation has been created and it as built using incremental algorithm
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Delaunay and Voronoi must exist
        Status *status = in.getStoreService()->getStatus();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        return status->isDelaunayCreated() && (delaunay->getAlgorithm() == INCREMENTAL);
    }


    /**
     * @fn      run
     * @brief   Builds set of faces path in a trianguatlion between two points
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Get points.
        vector<Line> vLines;
        LineFactory::readFromConfig(vLines);
        line = vLines.at(0);
        if (line.isInvalid())
        {
            LineFactory::generateRandom(1, vLines);
            line = vLines.at(1);
        }

        // https://github.com/juannavascalvente/Delaunay/issues/61
        // Compute triangles path between two points.
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        vector<int> vFacesId;
        this->isSuccess = delaunay->findPath(line, vFacesId);

        if (this->isSuccess)
        {
            for (auto face : vFacesId)
            {
                vector<Point<TYPE>> vFacesPoints;
                DcelFigureBuilder::getFacePoints(face, *in.getStoreService()->getDcel(), vFacesPoints);

                Polygon polygon;
                for (auto point : vFacesPoints)
                {
                    polygon.add(point);
                }
                vPolygons.push_back(polygon);
            }
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultPath(getSuccess(), in.getStoreService(), dcel, line, vPolygons);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandVoronoiPath : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    Line line;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandVoronoiPath(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation path can be only computed using incremental Delaunay
     */
    void printRunnableMsg() override
    {
        cout << "Triangulation is not Delaunay or it was not computed using incremenal algorithm" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks Delaunay and Voronoi have been created and it as built using incremental algorithm
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Delaunay and Voronoi must exist
        // TODO https://github.com/juannavascalvente/Delaunay/issues/10
        Status *status = in.getStoreService()->getStatus();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        return status->isVoronoiCreated() && (delaunay->getAlgorithm() == INCREMENTAL);
    }


    /**
     * @fn      run
     * @brief   Builds set of faces path in a trianguatlion between two points
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult * runCommand() override
    {
        // Get points.
        vector<Line> vLines;
        LineFactory::readFromConfig(vLines);
        line = vLines.at(0);
        if (line.isInvalid())
        {
            LineFactory::generateRandom(1, vLines);
            line = vLines.at(1);
        }

        // Compute triangles path between two points.
        vector<int> vFacesId(0);
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        Voronoi *voronoi = in.getStoreService()->getVoronoi();
        int	 initialFace=0;			// Initial face in the path.
        int	 finalFace=0;			// Final face in the path.
        Point<TYPE> closest;		// Closest point.
        double distance=0.0;		// Distance between points.
        Set<int> extremeFaces(2);	// First and last faces in the path.

        // Get extreme point faces.
        https://github.com/juannavascalvente/Delaunay/issues/62
        this->isSuccess = false;
        if (delaunay->findClosestPoint(line.getOrigin(), *voronoi, closest, initialFace, distance) &&
            delaunay->findClosestPoint(line.getDest(), *voronoi, closest, finalFace, distance))
        {
            // Add faces to set.
            extremeFaces.add(initialFace+1);
            extremeFaces.add(finalFace+1);

            // Find path.
            this->isSuccess = voronoi->getRefDcel()->findPath(extremeFaces, line, vFacesId);
        }

        if (this->isSuccess)
        {
            for (auto face : vFacesId)
            {
                vector<Point<TYPE>> vFacesPoints;
                DcelFigureBuilder::getFacePoints(face, *voronoi->getRefDcel(), vFacesPoints);

                Polygon polygon;
                for (auto point : vFacesPoints)
                {
                    polygon.add(point);
                }
                vPolygons.push_back(polygon);
            }
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getVoronoi()->getRefDcel();
        return new CommandResultPath(getSuccess(), in.getStoreService(), dcel, line, vPolygons);
    }
};



/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandClosestPoint : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandClosestPoint(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation must exist
     */
    void printRunnableMsg() override
    {
        cout << "Incremental Delaunay and Voronoi required, or triangulation built with no incremental" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks delauany and voronoi or triangulation was not incremental
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        return status->isTriangulationCreated() &&
        ((status->isDelaunayCreated() && status->isVoronoiCreated()) || (delaunay->getAlgorithm() != INCREMENTAL));
    }


    /**
     * @fn      run
     * @brief   Builds set of faces path in a triangulation between two points
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Get point to locate
        Point<TYPE> point, closest;
        PointFactory::readFromConfig(point);

        // Find closest point
        int	pointIndex=0;	// Index of the closest point.
        double distance;

        // Check if Delaunay triangulation computed.
        Status *status = in.getStoreService()->getStatus();
        if (status->isDelaunayCreated())
        {
            // Find node that surrounds input point p.
            if (status->isVoronoiCreated())
            {
                Delaunay *delaunay = in.getStoreService()->getDelaunay();
                Voronoi *voronoi = in.getStoreService()->getVoronoi();
                this->isSuccess = delaunay->findClosestPoint(point, *voronoi, closest, pointIndex, distance);
            }
        }
        else
        {
            // Find closest using brute force.
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            this->isSuccess = triangulation->findClosestPoint(point, closest, distance);
        }

        // Add point to locate and closest point
        if (this->isSuccess)
        {
            vPoints.push_back(point);
            vPoints.push_back(closest);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultClosestPoint(getSuccess(), in.getStoreService(), dcel, vPoints);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandFindFace : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Point<TYPE>> vPoints;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFindFace(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that Incremental Delaunay must exist
     */
    void printRunnableMsg() override
    {
        cout << "Incremental Delaunay must exist to find face" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation exist
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        return status->isDelaunayCreated() && (delaunay->getAlgorithm() == INCREMENTAL);
    }


    /**
     * @fn      run
     * @brief   Builds set of faces path in a triangulation between two points
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        int faceId=0;			// Face to find
        Point<TYPE> point;		// Point to locate

        // Check if input point parameter provided by user.
        PointFactory::readFromConfig(point);

        // Find face.
        Status *status = in.getStoreService()->getStatus();
        bool isImaginaryFace=false;
        if (status->isDelaunayCreated())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            this->isSuccess = delaunay->findFace(point, faceId, isImaginaryFace);
        }
//        else
//        {
//            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
//            this->isSuccess = triangulation->findFace(point, faceId);
//        }

        // Add point to locate
        vPoints.push_back(point);

        // Add face if it is not imaginary
        if (this->isSuccess && !isImaginaryFace)
        {
            vector<Point<TYPE>> vFacesPoints;
            DcelFigureBuilder::getFacePoints(faceId, *in.getStoreService()->getDcel(), vFacesPoints);

            Polygon polygon;
            for (auto item : vFacesPoints)
            {
                polygon.add(item);
            }
            vPolygons.push_back(polygon);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultFace(getSuccess(), in.getStoreService(), dcel, vPoints, vPolygons);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandTwoClosest : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandTwoClosest(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that a triangulation must exist
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exist to find two closest points" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation exist
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        return status->isTriangulationCreated();
    }


    /**
     * @fn      run
     * @brief   Builds set of faces path in a triangulation between two points
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        int iPointIdx1, iPointIdx2;
        Status *status = in.getStoreService()->getStatus();
        if (status->isDelaunayCreated())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            this->isSuccess = delaunay->findTwoClosest(iPointIdx1, iPointIdx2);
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            this->isSuccess = triangulation->findTwoClosest(iPointIdx1, iPointIdx2);
        }

        // Add closest points
        if (this->isSuccess)
        {
            vPoints.push_back(*in.getStoreService()->getDcel()->getRefPoint(iPointIdx1));
            vPoints.push_back(*in.getStoreService()->getDcel()->getRefPoint(iPointIdx2));
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultClosestPoint(getSuccess(), in.getStoreService(), dcel, vPoints);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandFilterEdges : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    TYPE minLen;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFilterEdges(CmdParamIn &inParam, TYPE minLenIn) : in(inParam), minLen(minLenIn) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that a triangulation must exist
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exist" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation exist
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        return status->isTriangulationCreated();
    }

    /**
     * @fn      run
     * @brief   Do nothing as job is done in display results
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        this->isSuccess = true;

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultTriangulation(getSuccess(), in.getStoreService(), dcel, minLen);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandCircumcentres : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Circle> vCircles;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandCircumcentres(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that a triangulation must exist
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exist" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation exist
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        return status->isTriangulationCreated();
    }

    /**
     * @fn      run
     * @brief   Do nothing as job is done in display results
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        this->isSuccess = true;

        // Add circles
        Dcel *dcel = in.getStoreService()->getDcel();
        for (int faceID=1; faceID<dcel->getNFaces() ;faceID++)
        {
            // Skip imaginary faces.
            if (!dcel->imaginaryFace(faceID))
            {
                // Get points of the triangle.
                int		points[NPOINTS_TRIANGLE];	        // Triangle points.
                dcel->getFaceVertices(faceID, points);

                // Build circle.
                vector<Point<TYPE>> vPoints;
                vPoints.push_back(*dcel->getRefPoint(points[0]-1));
                vPoints.push_back(*dcel->getRefPoint(points[1]-1));
                vPoints.push_back(*dcel->getRefPoint(points[2]-1));
                Circle circle = Circle(vPoints);
                vCircles.push_back(circle);
            }
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultCircles(getSuccess(), in.getStoreService(), dcel, vCircles);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandEdgeCircle : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Circle> vCircles;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandEdgeCircle(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that a triangulation must exist
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exist" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation exist
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        return status->isTriangulationCreated();
    }

    /**
     * @fn      run
     * @brief   Do nothing as job is done in display results
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        this->isSuccess = true;

        // Loop all faces (but external).
        Dcel *dcel = in.getStoreService()->getDcel();
        for (size_t edgeIndex=0; edgeIndex<dcel->getNEdges() ;edgeIndex++)
        {
            // Skip imaginary edges.
            if (!dcel->hasNegativeVertex(edgeIndex+1))
            {
                Point<TYPE> origin, dest;  	// Origin and destination points.
                Point<TYPE> middle;	    	// Edge middle point.

                // Create line.
                dcel->getEdgePoints(edgeIndex, origin, dest);
                Line line = Line(origin, dest);

                // Compute middle point of edge.
                line.getMiddle(middle);

                // Create circle
                TYPE radius = origin.distance(middle);
                Circle circle = Circle(&middle, radius);

                // Add circles
                vCircles.push_back(circle);
            }
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultCircles(getSuccess(), in.getStoreService(), dcel, vCircles);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandDcelInfo : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Displayable*> vDisplayable;

    void createDcelPointsInfo(vector<Text> &info)
    {
        // Draw all points of the set.
        for (size_t i=0; i < dcel->getNVertex() ; i++)
        {
            // Get and draw i-point.
            Point<TYPE> *point = dcel->getRefPoint(i);
            string strText = std::to_string(i+1);

            Text text(point->getX(), point->getY(), strText);
            info.push_back(text);
        }
    }


    void createDcelEdgeInfo(vector<Text> &info)
    {
        // Loop all edges.
        for (size_t edgeIndex=0; edgeIndex<dcel->getNEdges() ;edgeIndex++)
        {
            // Check if twin edge already visited.
            if ((edgeIndex+1) < dcel->getTwin(edgeIndex))
            {
                // Check edge is real.
                if (!dcel->hasNegativeVertex((int) edgeIndex+1))
                {
                    // Get edge extreme points.
                    Point<TYPE> origin, dest;	// Extreme points of edges.
                    dcel->getEdgePoints(edgeIndex, origin, dest);

                    // Compute middle point of edge.
                    Point<TYPE> middle;         // Middle point of the edge.
                    Point<TYPE>::middlePoint(&origin, &dest, &middle);

                    // Print information.
                    string strText = std::to_string(edgeIndex+1) + " - " + std::to_string(dcel->getTwin(edgeIndex));

                    Text text(middle.getX(), middle.getY(), strText);
                    info.push_back(text);
                }
            }
        }
    }


    void createDcelFacesInfo(vector<Text> &info)
    {
        // Loop all faces (skip external face).
        for (size_t faceId=0; faceId<dcel->getNFaces() ;faceId++)
        {
            // If any vertex is imaginary then face is not drawn.
            if (!dcel->imaginaryFace(faceId))
            {
                Polygon polygon;

                // Get edge in current face.
                size_t firstEdgeIndex = dcel->getFaceEdge(faceId)-1;
                size_t edgeIndex = firstEdgeIndex;
                do
                {
                    // Add origin point to polygon.
                    Point<TYPE> origin;			// Edge origin point.
                    origin = *dcel->getRefPoint(dcel->getOrigin(edgeIndex)-1);
                    polygon.add(origin);

                    // Next edge in face.
                    edgeIndex = dcel->getNext(edgeIndex)-1;
                } while(edgeIndex != firstEdgeIndex);

                // Compute face centroid.
                Point<TYPE> center;			// Middle point of the edge.
                polygon.centroid(center);
                polygon.reset();

                // Print information.
                string strText = std::to_string(faceId);

                Text text(center.getX(), center.getY(), strText);
                info.push_back(text);
            }
        }
    }

protected:
    Dcel *dcel;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandDcelInfo(CmdParamIn &inParam) : in(inParam)
    {
        dcel = in.getStoreService()->getDcel();
    };


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that a triangulation must exist
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exist" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation exist
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Triangulation must exist
        Status *status = in.getStoreService()->getStatus();
        return status->isTriangulationCreated();
    }

    /**
     * @fn      run
     * @brief   Do nothing as job is done in display results
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        this->isSuccess = true;

        // Add Delaunay triangulation
        Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
        vDisplayable.push_back(dispDelaunay);

        vector<Text> vPointsInfo;
        this->createDcelPointsInfo(vPointsInfo);
        Displayable *dispPointsInfo = DisplayableFactory::createTextSet(vPointsInfo);
        vDisplayable.push_back(dispPointsInfo);

        vector<Text> vEdgesInfo;
        this->createDcelEdgeInfo(vEdgesInfo);
        Displayable *dispEdgesInfo = DisplayableFactory::createTextSet(vEdgesInfo);
        vDisplayable.push_back(dispEdgesInfo);

        vector<Text> vFacesInfo;
        this->createDcelFacesInfo(vFacesInfo);
        Displayable *dispFacesInfo = DisplayableFactory::createTextSet(vFacesInfo);
        vDisplayable.push_back(dispFacesInfo);

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultDisplay(getSuccess(), in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandVoronoiInfo : public CommandDcelInfo
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit  CommandVoronoiInfo(CmdParamIn &inParam) : CommandDcelInfo(inParam)
    {
        dcel = inParam.getStoreService()->getVoronoi()->getRefDcel();
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandClear : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandClear(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Do nothing as job is done in display results
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        this->isSuccess = true;

        // Reset
        in.getStoreService()->getStatus()->reset();

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultDisplay(getSuccess(), in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadPoints : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Point<TYPE>> vPoints;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadPoints(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Read points from a flat file
     *
     * @return  true read was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        this->isSuccess = DcelReader::readPoints(Config::getInFlatFilename(), true, *dcel);

        if (this->isSuccess)
        {
            // Add point display
            for (size_t i=0; i< dcel->getNVertex() ; i++)
            {
                vPoints.push_back(*dcel->getRefPoint(i));
            }

            // Add points
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            vDisplayable.push_back(dispPoints);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultDisplay(getSuccess(), in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadPointsDcel : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Point<TYPE>> vPoints;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadPointsDcel(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Read points from a dcel file
     *
     * @return  true read was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        this->isSuccess = DcelReader::readPoints(Config::getInDCELFilename(), false, *dcel);

        if (this->isSuccess)
        {
            // Add point display
            for (size_t i=0; i< dcel->getNVertex() ; i++)
            {
                vPoints.push_back(*dcel->getRefPoint(i));
            }

            // Add points
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            vDisplayable.push_back(dispPoints);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultDisplay(getSuccess(), in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadDcel : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadDcel(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Read dcel from file
     *
     * @return  true read was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        this->isSuccess = DcelReader::read(Config::getInDCELFilename(), false, *dcel);

        if (this->isSuccess)
        {
            delaunay->setDCEL(dcel);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultDelaunay(getSuccess(), in.getStoreService(), dcel);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadDelaunay : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadDelaunay(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Read Delaunay from file
     *
     * @return  true read was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        delaunay->setDCEL(dcel);
        this->isSuccess = DelaunayIO::read(Config::getInDCELFilename(), Config::getInGraphFilename(), *delaunay);

        if (this->isSuccess)
        {
            delaunay->setAlgorithm(INCREMENTAL);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        return new CommandResultDelaunay(getSuccess(), in.getStoreService(), dcel);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadVoronoi : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadVoronoi(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Read Delaunay from file
     *
     * @return  true read was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        Dcel *dcel = in.getStoreService()->getDcel();
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        delaunay->setDCEL(dcel);
        this->isSuccess = DelaunayIO::read(Config::getInDCELFilename(), Config::getInGraphFilename(), *delaunay);

        if (this->isSuccess)
        {
            delaunay->setAlgorithm(INCREMENTAL);
        }

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        Dcel *dcel = in.getStoreService()->getDcel();
        Voronoi *voronoi = in.getStoreService()->getVoronoi();
        return new CommandResulVoronoi(getSuccess(), in.getStoreService(), dcel, voronoi);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFile : public Command
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    CmdParamIn  in;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFile(CmdParamIn &inParam) : in(inParam) {};


    /**
     * @fn      run
     * @brief   Write set of points to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Write points to file
        this->isSuccess = DcelWriter::writePoints(Config::getOutFlatFilename(), INVALID, *in.getStoreService()->getDcel());

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultDisplay(getSuccess(), in.getStoreService(), vDisplayable);
    }

    CmdParamIn *getInput() { return &in; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFileDcel : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFileDcel(CmdParamIn &inParam) : CommandWriteFile(inParam) {};

    /**
     * @fn      run
     * @brief   Write Dcel to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Write dcel to file
        this->isSuccess = DcelWriter::write(Config::getOutDCELFilename(), false, *getInput()->getStoreService()->getDcel());

        // Build result
        return createResult();
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFileDelaunay : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFileDelaunay(CmdParamIn &inParam) : CommandWriteFile(inParam) {};

    /**
     * @fn      run
     * @brief   Write Delaunay to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Write dcel to file
        this->isSuccess = DelaunayIO::write(Config::getOutDCELFilename(), Config::getOutGraphFilename(), *getInput()->getStoreService()->getDelaunay());

        // Build result
        return createResult();
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFileVoronoi : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFileVoronoi(CmdParamIn &inParam) : CommandWriteFile(inParam) {};

    /**
     * @fn      run
     * @brief   Write Delaunay to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Write dcel to file
        this->isSuccess = VoronoiIO::write(Config::getOutVoronoiFilename(), *getInput()->getStoreService()->getVoronoi());

        // Build result
        return createResult();
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFileGabriel : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFileGabriel(CmdParamIn &inParam) : CommandWriteFile(inParam) {};

    /**
     * @fn      run
     * @brief   Write Delaunay to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Write dcel to file
        this->isSuccess = GabrielIO::writeBinary(Config::getOutGabrielFilename(), *getInput()->getStoreService()->getGabriel());

        // Build result
        return createResult();
    }
};

#endif //DELAUNAY_COMMAND_H
