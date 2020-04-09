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

    // Delete all data
    deleteTriangulation();
    deleteDelaunay();
    deleteVoronoi();
    deleteGabriel();
}


void StoreService::save(StarTriangulation &triangulation)
{
    // Save triangulation
    repository->getData()->save(triangulation);

    // Reset points in store
    vector<Point<TYPE>> vPoints;
    triangulation.getRefDcel()->getPoints(vPoints);
    repository->getData()->save(vPoints);
}


void StoreService::save(Delaunay &delaunay)
{
    // Save Delaunay
    repository->getData()->save(delaunay);

    // Reset points in store
    vector<Point<TYPE>> vPoints;
    delaunay.getRefDcel()->getPoints(vPoints);
    repository->getData()->save(vPoints);
};
