/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreData.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
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
