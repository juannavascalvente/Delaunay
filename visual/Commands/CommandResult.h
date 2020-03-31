//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDRESULT_H
#define DELAUNAY_COMMANDRESULT_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "Dcel.h"
#include "defines.h"
#include "DisplayableFactory.h"
#include "Point.h"
#include "StoreService.h"

using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResult
{
protected:
    bool isSuccess;
    StoreService *storeService;

public:
    CommandResult(bool isSuccessIn, StoreService *storeServiceIn) : isSuccess(isSuccessIn), storeService(storeServiceIn) {};

    /**
     * @fn      updateStatus
     * @brief   Updates status based on result execution
     */
    virtual void updateStatus() {};

    /**
     * @fn      createDisplayables
     * @brief   Add elements to display
     *
     * @param   vDisplayable    (OUT) Vector that contains elements to display
     */
    virtual void createDisplayables(vector<Displayable*> &vDisplayable) {};


    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
    bool wasSuccess() const { return isSuccess; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultNull : public CommandResult
{
public:
    explicit CommandResultNull(bool isSuccess) : CommandResult(isSuccess, nullptr) {};
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultPoints : public CommandResult
{
    vector<Point<TYPE>> vPoints;
public:
    CommandResultPoints(bool isSuccess, StoreService *service, vector<Point<TYPE>> &vPointsIn) : CommandResult(isSuccess, service), vPoints(vPointsIn) {};

    void updateStatus() override
    {
        if (wasSuccess())
        {
            storeService->getStatus()->set(false, true, false, false, false, false);
        }
    };

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            // Add figure display
            vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
// TODO This could be the same as CommandResultRead
class CommandResultTriangulation : public CommandResult
{
    Dcel *dcel;
    TYPE minLength;
public:
    CommandResultTriangulation(bool isSuccess, StoreService *service, Dcel *dcelIn, TYPE minLengthIn=INVALID) :
                                                                                        CommandResult(isSuccess, service),
                                                                                        dcel(dcelIn),
                                                                                        minLength(minLengthIn) {};

    void updateStatus() override
    {
        if (wasSuccess())
        {
            storeService->getStatus()->set(false, true, true, false, false, false);
        }
    };

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel(), minLength));
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultDelaunay : public CommandResultTriangulation
{
public:
    CommandResultDelaunay(bool isSuccess, StoreService *service, Dcel *dcelIn) : CommandResultTriangulation(isSuccess, service, dcelIn) {};

    void updateStatus() override
    {
        if (wasSuccess())
        {
            storeService->getStatus()->set(false, true, true, true, false, false);
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultPolygon : public CommandResult
{
    Polygon *polygon;
public:
    CommandResultPolygon(bool isSuccess, StoreService *service, Polygon *polygonIn) : CommandResult(isSuccess, service), polygon(polygonIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            // Add polygon points
            vector<Point<TYPE>> vPoints;
            polygon->getPoints(vPoints);
            vDisplayable.push_back(DisplayableFactory::createPolygon(vPoints));
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResulVoronoi : public CommandResult
{
    Dcel *dcel;
    Voronoi *voronoi;
public:
    CommandResulVoronoi(bool isSuccess, StoreService *service, Dcel *dcelIn, Voronoi *voronoiIn) :  CommandResult(isSuccess, service),
                                                                                                    dcel(dcelIn),
                                                                                                    voronoi(voronoiIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            // Add delaunay and voronoi
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel()));
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getVoronoi()->getRefDcel()));
        }
    };

    void updateStatus() override
    {
        if (wasSuccess())
        {
            storeService->getStatus()->set(false, true, true, true, true, false);
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultGabriel : public CommandResult
{
    Dcel *dcel;
    Gabriel *gabriel;
public:
    CommandResultGabriel(bool isSuccess, StoreService *service, Dcel *dcelIn, Gabriel *gabrielIn) : CommandResult(isSuccess, service),
                                                                                                    dcel(dcelIn),
                                                                                                    gabriel(gabrielIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            Point<TYPE> *vertex1;	    // First vertex.
            Point<TYPE> *vertex2;	    // Second vertex.
            Dcel	*dcelRef;

            // Get reference to gabriel dcel.
            dcelRef = gabriel->getDcel();

            // Draw Gabriel edges.
            vector<Line> vLines;
            // TODO https://github.com/juannavascalvente/Delaunay/issues/60
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

            // Add Delaunay and gabriel graph
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel()));
            vDisplayable.push_back(DisplayableFactory::createPolyLine(vLines));
        }
    };

    void updateStatus() override
    {
        if (wasSuccess())
        {
            storeService->getStatus()->set(false, true, true, true, true, true);
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultPath : public CommandResult
{
    Dcel *dcel;
    Line line;
    vector<Polygon> vPolygons;

public:
    CommandResultPath(bool isSuccess, StoreService *service, Dcel *dcelIn, Line &lineIn, vector<Polygon> &vPolygonsIn) :
                                                                                        CommandResult(isSuccess, service),
                                                                                        dcel(dcelIn),
                                                                                        line(lineIn),
                                                                                        vPolygons(vPolygonsIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
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
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultClosestPoint : public CommandResult
{
    Dcel *dcel;
    vector<Point<TYPE>> &vPoints;

public:
    CommandResultClosestPoint(bool isSuccess, StoreService *service, Dcel *dcelIn, vector<Point<TYPE>> &vPointsIn) :
                                                                                    CommandResult(isSuccess, service),
                                                                                    dcel(dcelIn),
                                                                                    vPoints(vPointsIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(dcel);
            vDisplayable.push_back(dispDelaunay);

            // Add points (point to locate and closest point)
            Displayable *dispPoints = DisplayableFactory::createPointsSet(vPoints);
            dispPoints->setPointSize(5.0);
            vDisplayable.push_back(dispPoints);
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultFace : public CommandResult
{
    Dcel *dcel;
    vector<Point<TYPE>> &vPoints;
    vector<Polygon> vPolygons;

public:
    CommandResultFace(bool isSuccess, StoreService *service, Dcel *dcelIn, vector<Point<TYPE>> &vPointsIn, vector<Polygon> &vPolygonsIn) :
                                                                                CommandResult(isSuccess, service),
                                                                                dcel(dcelIn),
                                                                                vPoints(vPointsIn),
                                                                                vPolygons(vPolygonsIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
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
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultCircles : public CommandResult
{
    Dcel *dcel;
    vector<Circle> vCircles;

public:
    CommandResultCircles(bool isSuccess, StoreService *service, Dcel *dcelIn, vector<Circle> &vCirclesIn) :
                                                                                    CommandResult(isSuccess, service),
                                                                                    dcel(dcelIn),
                                                                                    vCircles(vCirclesIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (wasSuccess())
        {
            // Add Delaunay triangulation
            vDisplayable.push_back(DisplayableFactory::createDcel(dcel));

            // Add points to display.
            vDisplayable.push_back(DisplayableFactory::createCircleSet(vCircles));
        }
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultDisplay : public CommandResult
{
    vector<Displayable*> vDisplayable;

public:
    CommandResultDisplay(bool isSuccess, StoreService *service, vector<Displayable*> &vDisplayableIn) :
                                                                                    CommandResult(isSuccess, service),
                                                                                    vDisplayable(vDisplayableIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayableOut) override
    {
        vDisplayableOut = vDisplayable;
    }
};


#endif //DELAUNAY_COMMANDRESULT_H
