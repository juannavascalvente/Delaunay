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
#include "DcelFigureBuilder.h"
#include "FigureIO.h"
#include "LineFactory.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "PointsWriter.h"
#include "VoronoiIO.h"


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
        vector<Displayable*> vDisplayable(0);
        return new CommandResult(getSuccess(), vDisplayable);
    };

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    Command() : isSuccess(false), in(nullptr), result(nullptr) {};
    explicit Command(StoreService *storeServiceIn) : isSuccess(false), in(storeServiceIn), result(nullptr) {};
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
    explicit CommandNull(StoreService *storeServiceIn) : Command(storeServiceIn) {}

    /**
     * @fn      isRunnable
     * @brief   Null command can be executed but does nothing
     *
     * @return  false
     */
    bool isRunnable() override  { return true; };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandFail : public Command
{
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFail(StoreService *storeServiceIn) : Command(storeServiceIn) {}

    /**
     * @fn      isRunnable
     * @brief   Null command cannot execute
     *
     * @return  false
     */
    bool isRunnable() override  { return false; };

    /**
     * @fn      printRunnableMsg
     * @brief   Fail command cannot be executed
     */
    void printRunnableMsg() override
    {
        cout << "Wrong command has been created and cannot be executed" << endl;
    };
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
    explicit CommandReadConfig(StoreService *storeServiceIn) : Command(storeServiceIn) {}

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
    explicit CommandGenerateRandom(StoreService *storeServiceIn) : Command(storeServiceIn) {}

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
        return (in.getStoreService()->getNumPoints() > 0);
    };

    /**
     * @fn      printRunnableMsg
     * @brief   Prints message if command cannot be ran
     */
    void printRunnableMsg() override
    {
        // Check number of points
        if (in.getStoreService()->getNumPoints() == 0)
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
        size_t szNumPoints = in.getStoreService()->getNumPoints();

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
        vector<Displayable*> vDisplayable(0);
        if (getSuccess())
        {
            // Add figure display
            vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandGenerateCluster(StoreService *storeServiceIn) : Command(storeServiceIn) {}

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
        return (in.getStoreService()->getNumPoints() > 0.0) &&
                (in.getStoreService()->getNumClusters() > 0) &&
                (in.getStoreService()->getNumPoints() > 0);
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
        if (in.getStoreService()->getNumPoints() <= 0.0)
        {
            cout << "One of the values is not higher than 0" << endl;
        }

        // Check number of clusters
        if (in.getStoreService()->getNumClusters() == 0)
        {
            cout << "Number of cluster to generate is zero" << endl;
        }

        // Check number of points
        if (in.getStoreService()->getNumPoints() == 0)
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
        size_t szNumPoints = in.getStoreService()->getNumPoints();
        size_t szNumClusters = in.getStoreService()->getNumClusters();
        TYPE radius = in.getStoreService()->getRadius();

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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add figure display
            vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandStarTriangulation(StoreService *storeServiceIn) : Command(storeServiceIn) {}

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
        return (!in.getStoreService()->isTriangulation() &&
                !in.getStoreService()->isDelaunay());
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getStarTriang()->getRefDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel, INVALID));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandDelaunay(StoreService *storeServiceIn) : Command(storeServiceIn) {};

    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain there is no set of points
     */
    void printRunnableMsg() override
    {
        cout << "There is no set of points to use" << endl;
    };


    /**
     * @fn      isRunnable
     * @brief   Checks there is a set of points to use as input
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // Set of points must exist
        return in.getStoreService()->isSetCreated();
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
        bool isRunSuccess=true;

        // If Delaunay already exists -> do nothing
        if (!in.getStoreService()->isDelaunay())
        {
            // Get reference to current DCEL and Delaunay
            auto *delaunay = new Delaunay(in.getStoreService()->getPoints());

            // Build Delaunay from DCEL.
            isRunSuccess = delaunay->build();

            // Save result
            if (isRunSuccess)
            {
                in.getStoreService()->save( *delaunay);
            }

            // Free resources
            delete delaunay;
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel, INVALID));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    Polygon hull;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandConvexHull(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return (in.getStoreService()->isTriangulation() ||
                in.getStoreService()->isDelaunay());
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
        // Computing convex hull
        bool isRunSuccess;
        if (in.getStoreService()->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->convexHull();
            in.getStoreService()->getDelaunay()->getConvexHull(hull);
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->convexHull();
            triangulation->getConvexHull(hull);
        }

        if (!Config::getExtraOutputFilename().empty())
        {
            FigureIO::write(Config::getExtraOutputFilename(), hull);
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add polygon points
            vector<Point<TYPE>> vPoints;
            hull.getPoints(vPoints);
            vDisplayable.push_back(DisplayableFactory::createPolygon(vPoints));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandVoronoi(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return in.getStoreService()->isDelaunay();
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
        // Initialize voronoi data
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        Dcel *dcel = delaunay->getRefDcel();
        auto *voronoi = new Voronoi(dcel);

        // Compute Voronoi diagram
        bool isRunSuccess = voronoi->build();

        // Save result
        if (isRunSuccess)
        {
            in.getStoreService()->save( *voronoi);
        }

        // Free resources
        delete voronoi;

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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add delaunay and voronoi
            Displayable *displayDcel = DisplayableFactory::createDcel(in.getStoreService()->getDelaunay()->getRefDcel());
            displayDcel->setDashedLines();
            vDisplayable.push_back(displayDcel);
            vDisplayable.push_back(DisplayableFactory::createDcel(in.getStoreService()->getVoronoi()->getRefDcel()));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandGabriel(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return in.getStoreService()->isDelaunay() && in.getStoreService()->isVoronoi();
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
        auto *gabriel = new Gabriel(*delaunay->getRefDcel(), *voronoi);
        bool isRunSuccess = gabriel->build();

        if (isRunSuccess)
        {
            Point<TYPE> *vertex1;	    // First vertex.
            Point<TYPE> *vertex2;	    // Second vertex.
            Dcel	*dcelRef;

            // Get reference to gabriel dcel.
            dcelRef = gabriel->getDcel();

            // Draw Gabriel edges.
            for (size_t edgeIndex=0; edgeIndex<gabriel->getSize() ;edgeIndex++)
            {
                // Check if current edge mamtches Gabriel restriction.s
                if (gabriel->isSet(edgeIndex))
                {
                    // Get origin vertex of edge.
                    vertex1 = dcelRef->getRefPoint(dcelRef->getOrigin(edgeIndex)-1);

                    // Get destination vertex of edge.
                    vertex2 = dcelRef->getRefPoint(dcelRef->getOrigin(dcelRef->getTwin(edgeIndex)-1)-1);

                    Line line(*vertex1, *vertex2);
                    vLines.push_back(line);
                }
            }

            // Save result
            in.getStoreService()->save(*gabriel);
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay and gabriel graph
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            Displayable *displayDcel = DisplayableFactory::createDcel(delaunay->getRefDcel());
            displayDcel->setDashedLines();
            vDisplayable.push_back(displayDcel);
            vDisplayable.push_back(DisplayableFactory::createPolyLine(vLines));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    Dcel *dcel;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandTriangulationPath(StoreService *storeServiceIn) : Command(storeServiceIn), dcel(nullptr) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation path can be only computed using incremental Delaunay
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exits" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation must exits
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // A triangulation must exits
        return in.getStoreService()->isDelaunay() || in.getStoreService()->isTriangulation();
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
        vector<int> vFacesId;
        Point<TYPE> p = line.getOrigin();
        Point<TYPE> q = line.getDest();

//        cout << fixed;
//        cout.precision(6);
//        cout << p << endl;
//        cout << q << endl;

        if (!Config::getExtraOutputFilename().empty())
        {
            vector<Point<TYPE>> vPath;
            vPath.push_back(p);
            vPath.push_back(q);
            PointsWriter::write(Config::getExtraOutputFilename()+"in.bin", vPath);
        }

        // Check what triangulation is active
        bool isRunSuccess;
        if (in.getStoreService()->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->findPath(p, q, vFacesId);
            dcel = delaunay->getRefDcel();
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->findPath(p, q, vFacesId);
            dcel = triangulation->getRefDcel();
        }

        if (isRunSuccess)
        {
            vector<Point<TYPE>> vPathPoints;
            for (auto face : vFacesId)
            {
                vector<Point<TYPE>> vFacesPoints;
                DcelFigureBuilder::getFacePoints(face, *dcel, vFacesPoints);

                Polygon polygon;
                for (auto point : vFacesPoints)
                {
                    polygon.add(point);
                    vPathPoints.push_back(point);
                }
                vPolygons.push_back(polygon);
            }

            if (!Config::getExtraOutputFilename().empty())
            {
                PointsWriter::write(Config::getExtraOutputFilename(), vPathPoints);
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
        // Add items to display
        vector<Displayable*> vDisplayables;
        if (getSuccess())
        {
            // Add Delaunay triangulation
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

        return new CommandResult(getSuccess(), vDisplayables);
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
    explicit CommandVoronoiPath(StoreService *storeServiceIn) : Command(storeServiceIn) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation path can be only computed using incremental Delaunay
     */
    void printRunnableMsg() override
    {
        cout << "Voronoi diagram has not been computed" << endl;
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
        return in.getStoreService()->isVoronoi();
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
        Point<TYPE> p = line.getOrigin();
        Point<TYPE> q = line.getDest();

        if (!Config::getExtraOutputFilename().empty())
        {
            vector<Point<TYPE>> vLocate;
            vLocate.push_back(p);
            vLocate.push_back(q);
            PointsWriter::write(Config::getExtraOutputFilename()+"in.bin", vLocate);
        }

        // Compute triangles path between two points.
        vector<int> vFacesId;
        Voronoi *voronoi = in.getStoreService()->getVoronoi();

        // Get extreme point faces.
        https://github.com/juannavascalvente/Delaunay/issues/62
        bool isRunSuccess = false;
        Delaunay *delaunay = in.getStoreService()->getDelaunay();
        int pointFace1;
        int pointFace2;
        Point<TYPE> closest;
        if (delaunay->findClosestPoint(p, voronoi, closest, pointFace1) &&
            delaunay->findClosestPoint(q, voronoi, closest, pointFace2))
        {
            // Add faces to set.
            vector<int> vFaces;
            vFaces.push_back(pointFace1 + 1);
            vFaces.push_back(pointFace2 + 1);

            // Find path.
            isRunSuccess = voronoi->getRefDcel()->findPath(vFaces, line, vFacesId);
        }

        if (isRunSuccess)
        {
            vector<Point<TYPE>> vPathPoints;

            for (auto face : vFacesId)
            {
                vector<Point<TYPE>> vFacesPoints;
                DcelFigureBuilder::getFacePoints(face, *voronoi->getRefDcel(), vFacesPoints);

                Polygon polygon;
                for (auto point : vFacesPoints)
                {
                    polygon.add(point);
                    vPathPoints.push_back(point);
                }
                vPolygons.push_back(polygon);
            }

            if (!Config::getExtraOutputFilename().empty())
            {
                PointsWriter::write(Config::getExtraOutputFilename(), vPathPoints);
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getVoronoi()->getRefDcel();

            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayable.push_back(dispDelaunay);

            // Add faces path
            vDisplayable.push_back(DisplayableFactory::createPolygonSet(vPolygons));

            // Add points whose path is drawn
            vector<Point<TYPE>> vPoints;
            vPoints.push_back(line.getOrigin());
            vPoints.push_back(line.getDest());
            vDisplayable.push_back(DisplayableFactory::createPolygon(vPoints));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandClosestPoint(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return (in.getStoreService()->isDelaunay() && in.getStoreService()->isVoronoi()) || in.getStoreService()->isTriangulation();
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

        if (!Config::getExtraOutputFilename().empty())
        {
            vector<Point<TYPE>> vLocate;
            vLocate.push_back(point);
            PointsWriter::write(Config::getExtraOutputFilename()+"in.bin", vLocate);
        }

        // Find closest point
        int	pointIndex=0;	// Index of the closest point.

        // Check if Delaunay triangulation computed.
        bool isRunSuccess;
        if (in.getStoreService()->isDelaunay() && in.getStoreService()->isVoronoi())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            Voronoi *voronoi = in.getStoreService()->getVoronoi();
            isRunSuccess = delaunay->findClosestPoint(point, voronoi, closest, pointIndex);
        }
        else
        {
            // Find closest using brute force.
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->findClosestPoint(point, nullptr, closest, pointIndex);
        }

        // Add point to locate and closest point
        if (isRunSuccess)
        {
            vPoints.push_back(point);
            vPoints.push_back(closest);

            vector<Point<TYPE>> vPointsClosest;
            vPointsClosest.push_back(closest);
            if (!Config::getExtraOutputFilename().empty())
            {
                PointsWriter::write(Config::getExtraOutputFilename(), vPointsClosest);
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add triangulation
            Displayable *dcel;
            if (in.getStoreService()->isDelaunay() && in.getStoreService()->isVoronoi())
            {
                Delaunay *delaunay = in.getStoreService()->getDelaunay();
                dcel = DisplayableFactory::createDcel(delaunay->getRefDcel());
            }
            else
            {
                StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
                dcel = DisplayableFactory::createDcel(triangulation->getRefDcel());
            }
            vDisplayable.push_back(dcel);

            // Add points (point to locate and closest point)
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            dispPoints->setPointSize(5.0);
            vDisplayable.push_back(dispPoints);
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    Dcel *dcel;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFindFace(StoreService *storeServiceIn) : Command(storeServiceIn), dcel(nullptr) {};


    /**
     * @fn       printRunnableMsg
     * @brief    Prints message to explain that triangulation path can be only computed using incremental Delaunay
     */
    void printRunnableMsg() override
    {
        cout << "A triangulation must exits" << endl;
    }


    /**
     * @fn      isRunnable
     * @brief   Checks a triangulation must exits
     *
     * @return  true if command can be ran
     *          false otherwise
     */
    bool isRunnable() override
    {
        // A triangulation must exits
        return in.getStoreService()->isDelaunay() || in.getStoreService()->isTriangulation();
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

        if (!Config::getExtraOutputFilename().empty())
        {
            vector<Point<TYPE>> vLocate;
            vLocate.push_back(point);
            PointsWriter::write(Config::getExtraOutputFilename()+"in.bin", vLocate);
        }

        // Find face.
        bool isRunSuccess;
        if (in.getStoreService()->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->findFace(point, faceId);
            dcel = delaunay->getRefDcel();
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->findFace(point, faceId);
            dcel = triangulation->getRefDcel();
        }

        // Add point to locate
        vPoints.push_back(point);

        // Add face if it is not imaginary
        vPolygons.clear();
        if (isRunSuccess)
        {
            if (faceId != EXTERNAL_FACE)
            {
                vector<Point<TYPE>> vFacesPoints;
                DcelFigureBuilder::getFacePoints(faceId, *dcel, vFacesPoints);

                if (!Config::getExtraOutputFilename().empty())
                {
                    PointsWriter::write(Config::getExtraOutputFilename(), vFacesPoints);
                }

                Polygon polygon;
                for (auto item : vFacesPoints)
                {
                    polygon.add(item);
                }
                vPolygons.push_back(polygon);
            }
            else
            {
                cout << "Poins is external to convex hull" << endl;
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
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

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandTwoClosest(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return in.getStoreService()->isTriangulation() || in.getStoreService()->isDelaunay();
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
        Point<TYPE> p;
        Point<TYPE> q;
        bool isRunSuccess;
        if (in.getStoreService()->isDelaunay())
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            isRunSuccess = delaunay->findTwoClosest(p, q);
        }
        else
        {
            StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
            isRunSuccess = triangulation->findTwoClosest(p, q);
        }

        // Add closest points
        if (isRunSuccess)
        {
            Delaunay *delaunay = in.getStoreService()->getDelaunay();
            vPoints.push_back(p);
            vPoints.push_back(q);

            if (!Config::getExtraOutputFilename().empty())
            {
                PointsWriter::write(Config::getExtraOutputFilename(), vPoints);
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
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add triangulation
            Displayable *dcel;
            if (in.getStoreService()->isDelaunay())
            {
                Delaunay *delaunay = in.getStoreService()->getDelaunay();
                dcel = DisplayableFactory::createDcel(delaunay->getRefDcel());
            }
            else
            {
                StarTriangulation *triangulation = in.getStoreService()->getStarTriang();
                dcel = DisplayableFactory::createDcel(triangulation->getRefDcel());
            }
            vDisplayable.push_back(dcel);

            // Add points (point to locate and closest point)
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            dispPoints->setPointSize(5.0);
            vDisplayable.push_back(dispPoints);
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandFilterEdges(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return in.getStoreService()->isTriangulation() || in.getStoreService()->isDelaunay();
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
        // Add items to display
        vector<Displayable *> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel;
            if (in.getStoreService()->isDelaunay())
            {
                dcel = in.getStoreService()->getDelaunay()->getRefDcel();
            }
            else
            {
                dcel = in.getStoreService()->getStarTriang()->getRefDcel();
            }

            vDisplayable.push_back(DisplayableFactory::createDcel(dcel, in.getStoreService()->getMinLengthEdge()));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandCircumcentres(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return in.getStoreService()->isTriangulation() || in.getStoreService()->isDelaunay();
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
        // Add circles
        Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
        for (int faceID=1; faceID< dcel->getNumFaces() ; faceID++)
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
        setIsSuccess(true);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));

            // Add points to display.
            vDisplayable.push_back(DisplayableFactory::createCircleSet(vCircles));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandEdgeCircle(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        return in.getStoreService()->isTriangulation() || in.getStoreService()->isDelaunay();
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
        // Loop all faces (but external).
        Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
        for (int edgeIndex=0; edgeIndex< dcel->getNumEdges() ; edgeIndex++)
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
        setIsSuccess(true);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            // Add Delaunay triangulation
            Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));

            // Add points to display.
            vDisplayable.push_back(DisplayableFactory::createCircleSet(vCircles));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
        for (size_t i=0; i < dcel->getNumVertex() ; i++)
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
        for (size_t edgeIndex=0; edgeIndex< dcel->getNumEdges() ; edgeIndex++)
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
        for (size_t faceId=0; faceId< dcel->getNumFaces() ; faceId++)
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
    explicit CommandDcelInfo(StoreService *storeServiceIn) : Command(storeServiceIn)
    {
        if (in.getStoreService()->isDelaunay())
        {
            dcel = in.getStoreService()->getDelaunay()->getRefDcel();
        }
        else
        {
            dcel = in.getStoreService()->getStarTriang()->getRefDcel();
        }
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
        return in.getStoreService()->isTriangulation() || in.getStoreService()->isDelaunay();
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
        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit  CommandVoronoiInfo(StoreService *storeServiceIn) : CommandDcelInfo(storeServiceIn)
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
    explicit CommandClear(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        in.getStoreService()->reset();

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandReadPoints(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        this->isSuccess = PointsReader::read(Config::getInFlatFilename(), vPoints);

        if (this->isSuccess)
        {
            // Add points
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            vDisplayable.push_back(dispPoints);

            // Save result
            in.getStoreService()->save(vPoints);
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
        return new CommandResult(getSuccess(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandReadStar : public Command
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadStar(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        auto *t = new StarTriangulation();
        this->isSuccess = DcelReader::read(Config::getInDCELFilename(), *t->getRefDcel());
        if (this->isSuccess)
        {
            in.getStoreService()->save(*t);
        }

        // Free resources
        delete t;

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Set in.getStoreService() to update

        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getStarTriang()->getRefDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    explicit CommandReadDelaunay(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        auto *delaunay = new Delaunay();
        this->isSuccess = DelaunayIO::read(Config::getInDCELFilename(), Config::getInGraphFilename(), *delaunay);
        if (this->isSuccess)
        {
            in.getStoreService()->save(*delaunay);
        }

        // Free resources
        delete delaunay;

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Add items to display
        vector<Displayable*> vDisplayable;
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));
        }

        return new CommandResult(getSuccess(), vDisplayable);
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
    vector<Displayable*> vDisplayable;

public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandReadVoronoi(StoreService *storeServiceIn) : Command(storeServiceIn) {};

    /**
     * @fn      run
     * @brief   Read Voronoi from file
     *
     * @return  true read was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Reset store data
        in.getStoreService()->reset();

        // Run command
        auto *delaunay = new Delaunay();
        auto *voronoi  = new Voronoi();
        bool isSuccess = DelaunayIO::read(Config::getInDCELFilename(), Config::getInGraphFilename(), *delaunay);
        if (isSuccess)
        {
            isSuccess = VoronoiIO::read(Config::getOutVoronoiFilename(), *voronoi);
        }

        // Save data
        if (isSuccess)
        {
            // Save result
            in.getStoreService()->save(*delaunay);
            in.getStoreService()->save(*voronoi);
        }

        // Free resources
        delete delaunay;
        delete voronoi;

        // Build result
        setIsSuccess(isSuccess);
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Add items to display
        if (getSuccess())
        {
            Dcel *dcel = in.getStoreService()->getDelaunay()->getRefDcel();
            Voronoi *voronoi = in.getStoreService()->getVoronoi();

            // Add delaunay and voronoi
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));
            vDisplayable.push_back(DisplayableFactory::createDcel(voronoi->getRefDcel()));
        }

        return new CommandResult(getSuccess(), vDisplayable);
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteFile : public Command
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteFile(StoreService *storeServiceIn) : Command(storeServiceIn) {};


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
        this->isSuccess = PointsWriter::write(Config::getOutFlatFilename(), in.getStoreService()->getPoints());

        // Build result
        return createResult();
    }


    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        // Add items to display
        vector<Displayable*> vDisplayableIn;

        return new CommandResult(getSuccess(), vDisplayableIn);
    }

    CmdParamIn *getInput() { return &in; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandWriteDcel : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteDcel(StoreService *storeServiceIn) : CommandWriteFile(storeServiceIn) {};

    /**
     * @fn      run
     * @brief   Write Dcel to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Check what triangulation is being used
        Dcel *dcel;
        if (in.getStoreService()->isTriangulation())
        {
            dcel = in.getStoreService()->getStarTriang()->getRefDcel();
        }
        else
        {
            dcel = in.getStoreService()->getDelaunay()->getRefDcel();
        }

        // Write dcel to file
        this->isSuccess = DcelWriter::write(Config::getOutDCELFilename(), *dcel);

        // Build result
        return createResult();
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandFileDelaunay : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandFileDelaunay(StoreService *storeServiceIn) : CommandWriteFile(storeServiceIn) {};

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
class CommandWriteVoronoi : public CommandWriteFile
{
public:
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    explicit CommandWriteVoronoi(StoreService *storeServiceIn) : CommandWriteFile(storeServiceIn) {};

    /**
     * @fn      run
     * @brief   Write Delaunay to file
     *
     * @return  true built was successfully
     *          false otherwise
     */
    CommandResult* runCommand() override
    {
        // Write Delaunay to file
        Delaunay *delaunay = getInput()->getStoreService()->getDelaunay();
        bool isSuccess = DelaunayIO::write(Config::getInDCELFilename(), Config::getInGraphFilename(), *delaunay);

        // Write Voronoi to file
        if (isSuccess)
        {
            isSuccess = VoronoiIO::write(Config::getOutVoronoiFilename(), *getInput()->getStoreService()->getVoronoi());
        }

        // Build result
        setIsSuccess(isSuccess);
        return createResult();
    }
};

#endif //DELAUNAY_COMMAND_H
