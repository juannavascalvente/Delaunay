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

    static DispPoint* createPoint(Point<TYPE> &point) { return new DispPoint(point); };

    static DispPointsSet* createPointsSet(vector<Point<TYPE>> &vPointsIn) { return new DispPointsSet(vPointsIn); };

    static DispPolygon* createPolygon(vector<Point<TYPE>> &vPointsIn) { return new DispPolygon(vPointsIn); };

    static DispDcel* createDcel(Dcel *dcel) { return new DispDcel(dcel); };
};


#endif //DELAUNAY_DISPLAYABLEFACTORY_H
