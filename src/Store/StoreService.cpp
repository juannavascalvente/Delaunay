//
// Created by delaunay on 29/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreService.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
void StoreService::destroy()
{
    repository->deleteInstance();
}


void StoreService::reset()
{
    // Reset points set
    getPoints().clear();

    // Reset Star triangulation
    if (getStarTriang())
    {
        getStarTriang()->reset();
    }

    // Reset Delaunay
    if (getDelaunay())
    {
        getDelaunay()->reset();
    }

    // Reset Voronoi
    if (getVoronoi())
    {
        getVoronoi()->reset();
    }

    // Reset Gabriel
    if (getGabriel())
    {
        getGabriel()->reset();
    }
}
