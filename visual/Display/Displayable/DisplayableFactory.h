//
// Created by delaunay on 25/3/20.
//

#ifndef DELAUNAY_DISPLAYABLEFACTORY_H
#define DELAUNAY_DISPLAYABLEFACTORY_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"
#include "Displayable.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayableFactory
{
public:

    static DispPolyLine* createPolyLine(vector<Point<TYPE>> &vPointsIn) { return new DispPolyLine(vPointsIn); };

    static DispPointsSet* createPointsSet(vector<Point<TYPE>> &vPointsIn) { return new DispPointsSet(vPointsIn); };

    static DispPolygon* createPolygon(vector<Point<TYPE>> &vPointsIn) { return new DispPolygon(vPointsIn); };

    static DispPolygonSet* createPolygonSet(vector<Polygon> &vPolygon) { return new DispPolygonSet(vPolygon); };

    static DispCircleSet* createCircleSet(vector<Circle> &vCircles) { return new DispCircleSet(vCircles); };

    static DispDcel *createDcel(Dcel *dcel, TYPE minLength=INVALID) { return new DispDcel(dcel, minLength); };
};


#endif //DELAUNAY_DISPLAYABLEFACTORY_H
