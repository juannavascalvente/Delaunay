//
// Created by delaunay on 30/3/20.
//

#ifndef DELAUNAY_LINEFACTORY_H
#define DELAUNAY_LINEFACTORY_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Line.h"
#include <vector>


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
class LineFactory
{

public:
    static void generateRandom(size_t szNum, vector<Line> &vLines);
    static void readFromConfig(vector<Line> &vLines);
};

#endif //DELAUNAY_LINEFACTORY_H
