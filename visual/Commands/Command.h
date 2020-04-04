#ifndef DELAUNAY_COMMAND_H
#define DELAUNAY_COMMAND_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "CommandParamIn.h"
#include "CommandResult.h"
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
    CmdParamIn in;
    CommandResult *result;

    /*******************************************************************************************************************
    * Protected class methods
    *******************************************************************************************************************/
    virtual bool isRunnable() { return true; };
    virtual void printRunnableMsg() {};
    virtual CommandResult * runCommand() { return createResult(); };
    virtual CommandResult* createResult()
    {
        Status status;
        vector<Displayable*> vDisplayable(0);
        return new CommandResult(getSuccess(), *in.getStoreService()->getStatus(), in.getStoreService(), vDisplayable);
    };

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    Command() : isSuccess(false), in(nullptr, nullptr), result(nullptr) {};
    explicit Command(StoreService *storeServiceIn, ConfigService *configService) : isSuccess(false),
                                                                                   in(storeServiceIn, configService),
                                                                                   result(nullptr) {};
    virtual ~Command() { delete result; };


    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
    bool getSuccess() const { return isSuccess; }
    CommandResult *getResult() const { return result;}
    void setIsSuccess(bool isSuccessIn) { isSuccess = isSuccessIn; }

    /**
     * @fn      run
     * @brief   runs command if command can be executed
     *
     * @return  true if command was ran
     *          false otherwise
     */
    void run()
    {
        setIsSuccess(false);

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
    CommandNull(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {}

    /**
     * @fn      isRunnable
     * @brief   Null command cannnot execute
     *
     * @return  false
     */
    bool isRunnable() override  { return false; };
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
    CommandReadConfig(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {}

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
        bool isRunSuccess = Config::readConfig();

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
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
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandGenerateRandom(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {}

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
        return (in.getConfigService()->getNumPoints() > 0);
    };

    /**
     * @fn      printRunnableMsg
     * @brief   Prints message if command cannot be ran
     */
    void printRunnableMsg() override
    {
        // Check number of points
        if (in.getConfigService()->getNumPoints() == 0)
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
        size_t szNumPoints = in.getConfigService()->getNumPoints();

        // Reset store data
        in.getStoreService()->reset();

        // Run command
        bool isRunSuccess = DcelGenerator::generateRandom(szNumPoints, vPoints);
        if (isRunSuccess)
        {
            in.getStoreService()->save(vPoints);
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = Status(false, true, false, false, false, false);

        vector<Displayable*> vDisplayable(0);
        if (getSuccess())
        {
            // Add figure display
            vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandGenerateCluster(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {}

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
        return (in.getConfigService()->getNumPoints() > 0.0) &&
                (in.getConfigService()->getNumClusters() > 0) &&
                (in.getConfigService()->getNumPoints() > 0);
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
        if (in.getConfigService()->getNumPoints() <= 0.0)
        {
            cout << "One of the values is not higher than 0" << endl;
        }

        // Check number of clusters
        if (in.getConfigService()->getNumClusters() == 0)
        {
            cout << "Number of cluster to generate is zero" << endl;
        }

        // Check number of points
        if (in.getConfigService()->getNumPoints() == 0)
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
        size_t szNumPoints = in.getConfigService()->getNumPoints();
        size_t szNumClusters = in.getConfigService()->getNumClusters();
        TYPE radius = in.getConfigService()->getRadius();

        // Run command
        bool isRunSuccess = DcelGenerator::generateClusters(szNumPoints, szNumClusters, radius, vPoints);
        if (isRunSuccess)
        {
            in.getStoreService()->save(vPoints);
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = Status(false, true, false, false, false, false);

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add figure display
            vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandStarTriangulation : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandStarTriangulation(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {}

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
        return (!in.getStoreService()->getStatus()->isTriangulation() &&
                !in.getStoreService()->getStatus()->isDelaunay());
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
        auto *triangulation = new StarTriangulation(in.getStoreService()->getPoints());
        bool isRunSuccess = triangulation->build();

        // Save result
        if (isRunSuccess)
        {
            in.getStoreService()->save(*triangulation);
        }

        delete triangulation;

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = Status(false, true, true, false, false, false);

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getStarTriang()->getDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel, INVALID));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandDelaunay : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandDelaunay(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};

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
        return !in.getStoreService()->getStatus()->isDelaunay();
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
        bool isRunSuccess=false;
        if (status->isTriangulation())
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->delaunay();
            delaunay->setAlgorithm(FROM_STAR);
        }
        else
        {
            // Build Delaunay from DCEL.
            if (!status->isDelaunay())
            {
                isRunSuccess = delaunay->incremental();
            }
        }

        // Run command
        StarTriangulation *triangulation = in.getStoreService()->getStarTriang();

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = Status(false, true, true, true, false, false);

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel, INVALID));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    Polygon *hull;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandConvexHull(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService), hull(nullptr) {};


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
        return (in.getStoreService()->getStatus()->isTriangulation() ||
                in.getStoreService()->getStatus()->isDelaunay());
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

        // Computing convex hull
        bool isRunSuccess;
        if (status->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->convexHull();
            hull = in.getStoreService()->getDelaunay()->getConvexHull();
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->convexHull();
            hull = triangulation->getConvexHull();
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add polygon points
            vector<Point<TYPE>> vPoints;
            hull->getPoints(vPoints);
            vDisplayable.push_back(DisplayableFactory::createPolygon(vPoints));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandVoronoi : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandVoronoi(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return in.getStoreService()->getStatus()->isDelaunay();
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
        bool isRunSuccess = voronoi->init(dcel);

        // Check init was success
        if (isRunSuccess)
        {
            // Compute Voronoi diagram.
            isRunSuccess = voronoi->build(true);
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status
        Status status = Status(false, true, true, true, true, false);

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add delaunay and voronoi
            vDisplayable.push_back(DisplayableFactory::createDcel(in.getStoreService()->getDcel()));
            vDisplayable.push_back(DisplayableFactory::createDcel(in.getStoreService()->getVoronoi()->getRefDcel()));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandGabriel : public Command
{
    vector<Line> vLines;
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandGabriel(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isDelaunay() && status->isVoronoi();
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
        bool isRunSuccess = gabriel->build();

        if (isRunSuccess)
        {
            Point<TYPE> *vertex1;	    // First vertex.
            Point<TYPE> *vertex2;	    // Second vertex.
            Dcel	*dcelRef;

            // Get reference to gabriel dcel.
            Gabriel *gabrielIn = in.getStoreService()->getGabriel();
            dcelRef = gabriel->getDcel();

            // Draw Gabriel edges.
            // TODO https://github.com/juannavascalvente/Delaunay/issues/60 -> Add dashed lines to highlight results
            for (size_t edgeIndex=0; edgeIndex<gabrielIn->getSize() ;edgeIndex++)
            {
                // Check if current edge mamtches Gabriel restriction.s
                if (gabrielIn->isSet(edgeIndex))
                {
                    // Get origin vertex of edge.
                    vertex1 = dcelRef->getRefPoint(dcelRef->getOrigin(edgeIndex)-1);

                    // Get destination vertex of edge.
                    vertex2 = dcelRef->getRefPoint(dcelRef->getOrigin(dcelRef->getTwin(edgeIndex)-1)-1);

                    Line line(*vertex1, *vertex2);
                    vLines.push_back(line);
                }
            }
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay and gabriel graph
            vDisplayable.push_back(DisplayableFactory::createDcel(in.getStoreService()->getDcel()));
            vDisplayable.push_back(DisplayableFactory::createPolyLine(vLines));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    Line line;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandTriangulationPath(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isDelaunay() && (delaunay->getAlgorithm() == INCREMENTAL);
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
        bool isRunSuccess = delaunay->findPath(line, vFacesId);

        if (isRunSuccess)
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
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayables;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDcel();
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayables.push_back(dispDelaunay);

            // Add points whose path is drawn
            vector<Point<TYPE>> vPoints;
            vPoints.push_back(line.getOrigin());
            vPoints.push_back(line.getDest());
            vDisplayables.push_back(DisplayableFactory::createPolygon(vPoints));

            // Add faces
            vDisplayables.push_back(DisplayableFactory::createPolygonSet(vPolygons));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayables);
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
    Line line;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandVoronoiPath(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isVoronoi() && (delaunay->getAlgorithm() == INCREMENTAL);
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
        bool isRunSuccess = false;
        if (delaunay->findClosestPoint(line.getOrigin(), *voronoi, closest, initialFace, distance) &&
            delaunay->findClosestPoint(line.getDest(), *voronoi, closest, finalFace, distance))
        {
            // Add faces to set.
            extremeFaces.add(initialFace+1);
            extremeFaces.add(finalFace+1);

            // Find path.
            isRunSuccess = voronoi->getRefDcel()->findPath(extremeFaces, line, vFacesId);
        }

        if (isRunSuccess)
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
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getVoronoi()->getRefDcel();

            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayable.push_back(dispDelaunay);

            // Add points whose path is drawn
            vector<Point<TYPE>> vPoints;
            vPoints.push_back(line.getOrigin());
            vPoints.push_back(line.getDest());
            vDisplayable.push_back(DisplayableFactory::createPolygon(vPoints));

            // Add faces
            vDisplayable.push_back(DisplayableFactory::createPolygonSet(vPolygons));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandClosestPoint(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isTriangulation() &&
        ((status->isDelaunay() && status->isVoronoi()) || (delaunay->getAlgorithm() != INCREMENTAL));
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
        bool isRunSuccess=false;
        Status *status = in.getStoreService()->getStatus();
        if (status->isDelaunay())
        {
            // Find node that surrounds input point p.
            if (status->isVoronoi())
            {
                Delaunay *delaunay = in.getStoreService()->getDelaunay();
                Voronoi *voronoi = in.getStoreService()->getVoronoi();
                isRunSuccess = delaunay->findClosestPoint(point, *voronoi, closest, pointIndex, distance);
            }
        }
        else
        {
            // Find closest using brute force.
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->findClosestPoint(point, closest, distance);
        }

        // Add point to locate and closest point
        if (isRunSuccess)
        {
            vPoints.push_back(point);
            vPoints.push_back(closest);
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDcel();
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayable.push_back(dispDelaunay);

            // Add points (point to locate and closest point)
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            dispPoints->setPointSize(5.0);
            vDisplayable.push_back(dispPoints);
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Point<TYPE>> vPoints;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFindFace(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isDelaunay() && (delaunay->getAlgorithm() == INCREMENTAL);
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
        bool isRunSuccess=false;
        bool isImaginaryFace=false;
        if (status->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->findFace(point, faceId, isImaginaryFace);
        }
//        else
//        {
//            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
//            this->isSuccess = triangulation->findFace(point, faceId);
//        }

        // Add point to locate
        vPoints.push_back(point);

        // Add face if it is not imaginary
        if (isRunSuccess && !isImaginaryFace)
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
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDcel();
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayable.push_back(dispDelaunay);

            // Add points (point to locate and closest point)
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            dispPoints->setPointSize(3.0);
            vDisplayable.push_back(dispPoints);

            // Add faces
            Displayable *dispFace = DisplayableFactory::createPolygonSet(vPolygons);
            dispFace->setPointSize(3.0);
            vDisplayable.push_back(dispFace);
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandTwoClosest(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isTriangulation();
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
        bool isRunSuccess;
        Status *status = in.getStoreService()->getStatus();
        if (status->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->findTwoClosest(iPointIdx1, iPointIdx2);
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->findTwoClosest(iPointIdx1, iPointIdx2);
        }

        // Add closest points
        if (isRunSuccess)
        {
            vPoints.push_back(*in.getStoreService()->getDcel()->getRefPoint(iPointIdx1));
            vPoints.push_back(*in.getStoreService()->getDcel()->getRefPoint(iPointIdx2));
        }

        // Build result
        setIsSuccess(isRunSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDcel();
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayable.push_back(dispDelaunay);

            // Add points (point to locate and closest point)
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            dispPoints->setPointSize(5.0);
            vDisplayable.push_back(dispPoints);
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandFilterEdges : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFilterEdges(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isTriangulation();
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
        // Build result
        setIsSuccess(true);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable *> vDisplayable;
        if (getSuccess()) {
            vDisplayable.push_back(DisplayableFactory::createDcel(in.getStoreService()->getDcel(),
                                                                  in.getConfigService()->getMinLengthEdge()));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Circle> vCircles;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandCircumcentres(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isTriangulation();
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
        setIsSuccess(true);

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
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));

            // Add points to display.
            vDisplayable.push_back(DisplayableFactory::createCircleSet(vCircles));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Circle> vCircles;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandEdgeCircle(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        return status->isTriangulation();
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
        setIsSuccess(true);

        // Loop all faces (but external).
        Dcel *dcel = in.getStoreService()->getDcel();
        for (int edgeIndex=0; edgeIndex<dcel->getNEdges() ;edgeIndex++)
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
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));

            // Add points to display.
            vDisplayable.push_back(DisplayableFactory::createCircleSet(vCircles));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    explicit CommandDcelInfo(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService)
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
        return status->isTriangulation();
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
        setIsSuccess(true);

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
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    explicit  CommandVoronoiInfo(StoreService *storeServiceIn, ConfigService *configService) : CommandDcelInfo(storeServiceIn, configService)
    {
        dcel = storeServiceIn->getVoronoi()->getRefDcel();
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
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandClear(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


    /**
     * @fn      run
     * @brief   Do nothing as job is done in display results
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        setIsSuccess(true);

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
        Status status = Status(true, false, false, false, false, false);
        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Point<TYPE>> vPoints;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadPoints(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        Status status = Status(false, true, false, false, false, false);
        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Point<TYPE>> vPoints;
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadPointsDcel(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        Status status = Status(false, true, false, false, false, false);
        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
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
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadDcel(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        Status status = Status(false, true, true, true, false, false);
        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadDelaunay : public Command
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadDelaunay(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        // Set status to update
        Status status = Status(false, true, true, true, false, false);

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));
        }

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
//class CommandReadVoronoi : public Command
//{
//    /*******************************************************************************************************************
//    * Class members
//    *******************************************************************************************************************/
//    vector<Displayable*> vDisplayable;
//
//public:
//    /*******************************************************************************************************************
//    * Public class methods
//    *******************************************************************************************************************/
//    explicit CommandReadVoronoi(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};
//
//
//    /**
//     * @fn      run
//     * @brief   Read Delaunay from file
//     *
//     * @return  true read was successfully
//     *          false otherwise
//     */
//    CommandResult* runCommand() override
//    {
//        // Reset store data
//        in.getStoreService()->reset();
//
//        // Run command
//        Dcel *dcel = in.getStoreService()->getDcel();
//        Delaunay *delaunay = in.getStoreService()->getDelaunay();
//        delaunay->setDCEL(dcel);
//        this->isSuccess = DelaunayIO::read(Config::getInDCELFilename(), Config::getInGraphFilename(), *delaunay);
//
//        if (this->isSuccess)
//        {
//            delaunay->setAlgorithm(INCREMENTAL);
//        }
//
//        // Build result
//        return createResult();
//    }
//
//
//    /**
//     * @fn      createResult
//     * @brief   Creates command result
//     */
//    CommandResult *createResult() override
//    {
//        // Set status
//        Status status = Status(false, true, true, true, true, false);
//
//        // Add items to display
//        vector<Displayable*> vDisplayableIn;
//        if (getSuccess())
//        {
//            Dcel *dcel = in.getStoreService()->getDcel();
//            Voronoi *voronoi = in.getStoreService()->getVoronoi();
//
//            // Add delaunay and voronoi
//            vDisplayableIn.push_back(DisplayableFactory::createDcel(dcel));
//            vDisplayableIn.push_back(DisplayableFactory::createDcel(voronoi->getRefDcel()));
//        }
//
//        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayableIn);
//    }
//};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFile : public Command
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFile(StoreService *storeServiceIn, ConfigService *configService) : Command(storeServiceIn, configService) {};


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
        // Set status to update
        Status status = *in.getStoreService()->getStatus();

        // Add items to display
        vector<Displayable*> vDisplayableIn;

        return new CommandResult(getSuccess(), status, in.getStoreService(), vDisplayableIn);
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
    explicit CommandWriteFileDcel(StoreService *storeServiceIn, ConfigService *configService) : CommandWriteFile(storeServiceIn, configService) {};

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
    explicit CommandWriteFileDelaunay(StoreService *storeServiceIn, ConfigService *configService) : CommandWriteFile(storeServiceIn, configService) {};

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
    explicit CommandWriteFileVoronoi(StoreService *storeServiceIn, ConfigService *configService) : CommandWriteFile(storeServiceIn, configService) {};

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
    explicit CommandWriteFileGabriel(StoreService *storeServiceIn, ConfigService *configService) : CommandWriteFile(storeServiceIn, configService) {};

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
