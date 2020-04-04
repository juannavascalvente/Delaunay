/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreData.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void StoreData::save(vector<Point<TYPE>> &vPointsIn)
{
    vPoints = vPointsIn;
}


void StoreData::save(StarTriangulation &in)
{
    delete triangulation;
    triangulation = new StarTriangulation(in);
}

void StoreData::save(Delaunay &in)
{
    delete delaunay;
    delaunay = new Delaunay(in);
}
