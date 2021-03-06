/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreData.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void StoreData::deleteStar()
{
    delete triangulation;
    triangulation = nullptr;
}


void StoreData::deleteDelaunay()
{
    delete delaunay;
    delaunay = nullptr;
}


void StoreData::deleteVoronoi()
{
    delete voronoi;
    voronoi = nullptr;
}


void StoreData::deleteGabriel()
{
    delete gabriel;
    gabriel = nullptr;
}


void StoreData::save(vector<Point<TYPE>> &v)
{
    vPoints = v;
}


void StoreData::save(StarTriangulation &t)
{
    delete triangulation;
    triangulation = new StarTriangulation(t);
}


void StoreData::save(Delaunay &d)
{
    delete delaunay;
    delaunay = new Delaunay(d);
}


void StoreData::save(Voronoi &v)
{
    delete voronoi;
    voronoi = new Voronoi(v);
}


void StoreData::save(Gabriel &g)
{
    delete gabriel;
    gabriel = new Gabriel(g);
}
