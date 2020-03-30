//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMAND_H
#define DELAUNAY_COMMAND_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "CommandParamIn.h"
#include "CommandParamOut.h"
#include "CommandResult.h"
#include "Config.h"
#include "LineFactory.h"
#include "DcelGenerator.h"
#include "DcelFigureBuilder.h"


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
     * @fn       printRunnableMsg
     * @brief    Prints message to explain null command has no task
     */
    void printRunnableMsg() override { cout << "Null command does not execute any action" << endl; };

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
    GeneratorCmdParamOut out;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandGenerateRandom(GeneratorCmdParamIn &inParam, GeneratorCmdParamOut &outParam) : in(inParam), out(outParam) {}

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
        // Run command
        this->isSuccess = DcelGenerator::generateRandom(in.getNumPoints(), out.getDcel());

        // Build result
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultRead(getSuccess(), in.getStoreService(), &out.getDcel());
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
    GeneratorCmdParamOut out;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandGenerateCluster(GeneratorClusterCmdParamIn &inParam, GeneratorCmdParamOut &outParam) : in(inParam), out(outParam) {}

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
        this->isSuccess = DcelGenerator::generateClusters(in.getNumPoints(), in.getSzNumClusters(), in.getFRadius(), out.getDcel());

        // Build result
        return createResult();
    }

    /**
     * @fn      createResult
     * @brief   Creates command result
     */
    CommandResult *createResult() override
    {
        return new CommandResultRead(getSuccess(), in.getStoreService(), &out.getDcel());
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
    GeneratorCmdParamOut out;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandStarTriangulation(StarTriangulationParamCmdIn &inParam, GeneratorCmdParamOut &outParam) : in(inParam), out(outParam) {};

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
        return new CommandResultTriangulation(getSuccess(), in.getStoreService(), &out.getDcel());
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
    CmdParamOut out;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandDelaunay(CmdParamIn &inParam, CmdParamOut &outParam) : in(inParam), out(outParam) {};

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
        Dcel *dcel = out.getStoreService()->getDcel();
        Delaunay *delaunay = out.getStoreService()->getDelaunay();
        delaunay->setDCEL(dcel);

        // Get reference to status
        Status *status = out.getStoreService()->getStatus();

        // Build Delaunay from Star triangulation.
        if (status->isTriangulationCreated())
        {
            StarTriangulation *triangulation = out.getStoreService()->getStarTriang();
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
        Dcel *dcel = out.getStoreService()->getDcel();
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
    CmdParamOut out;
    Polygon *hull;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandConvexHull(CmdParamIn &inParam, CmdParamOut &outParam) : in(inParam), out(outParam), hull(nullptr) {};


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
            StarTriangulation *triangulation = out.getStoreService()->getStarTriang();
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
    CmdParamOut out;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandVoronoi(CmdParamIn &inParam, CmdParamOut &outParam) : in(inParam), out(outParam) {};


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
        Dcel *dcel = out.getStoreService()->getDcel();
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
    CmdParamOut out;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandGabriel(CmdParamIn &inParam, CmdParamOut &outParam) : in(inParam), out(outParam) {};


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
        Dcel *dcel = out.getStoreService()->getDcel();
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
    CmdParamOut out;
    Line line;
    vector<Polygon> vPolygons;

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    CommandTriangulationPath(CmdParamIn &inParam, CmdParamOut &outParam) : in(inParam), out(outParam) {};


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
     * @brief   Builds star triangulation
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


#endif //DELAUNAY_COMMAND_H
