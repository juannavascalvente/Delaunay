//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDPARAMOUT_H
#define DELAUNAY_COMMANDPARAMOUT_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CmdParamOut
{
    StoreService *storeService;
public:
    explicit CmdParamOut(StoreService *storeServiceIn) : storeService(storeServiceIn) {};

    StoreService *getStoreService() const { return storeService; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GeneratorCmdParamOut
{
    Dcel *dcel;
public:
    explicit GeneratorCmdParamOut(Dcel *dcelIn) : dcel(dcelIn) {};

    /**
     * Getter/Setter
     */
    Dcel &getDcel() { return(*dcel); }
};

#endif //DELAUNAY_COMMANDPARAMOUT_H
