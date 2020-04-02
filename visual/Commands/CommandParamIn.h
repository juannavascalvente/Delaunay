//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDPARAMIN_H
#define DELAUNAY_COMMANDPARAMIN_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"
#include "ConfigService.h"
#include "StoreService.h"

#include <cstddef>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CmdParamIn
{
    ConfigService *cfgService;
    StoreService *storeService;
public:
    explicit CmdParamIn(StoreService *storeServiceIn, ConfigService *cfgServiceIn) : cfgService(cfgServiceIn),
                                                                                     storeService(storeServiceIn) {};

    ConfigService *getConfigService() const { return cfgService; }
    StoreService *getStoreService() const { return storeService; }
};

#endif //DELAUNAY_COMMANDPARAMIN_H
