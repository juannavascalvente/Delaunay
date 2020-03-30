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
    virtual void updateStatus() {};
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
class CommandResultRead : public CommandResult
{
    Dcel *dcel;
public:
    CommandResultRead(bool isSuccess, StoreService *service, Dcel *dcelIn) : CommandResult(isSuccess, service), dcel(dcelIn) {};

    void updateStatus() override
    {
        storeService->getStatus()->set(false, isSuccess, false, false, false, false);
    };

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        // Add figure display.
        vector<Point<TYPE>> vPoints;
        for (size_t i=0; i< Config::getNPoints(); i++)
        {
            vPoints.push_back(*dcel->getRefPoint(i));
        }
        vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
// TODO This could be the same as CommandResultRead
class CommandResultStarTriangulation : public CommandResult
{
    Dcel *dcel;
public:
    CommandResultStarTriangulation(bool isSuccess, StoreService *service, Dcel *dcelIn) : CommandResult(isSuccess, service), dcel(dcelIn) {};

    void updateStatus() override
    {
        storeService->getStatus()->set(false, true, true, false, false, false);
    };

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel()));
    };
};

#endif //DELAUNAY_COMMANDRESULT_H
