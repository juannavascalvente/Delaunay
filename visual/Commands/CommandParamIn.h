//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDPARAMIN_H
#define DELAUNAY_COMMANDPARAMIN_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"

#include <cstddef>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GeneratorCmdParamIn
{
    size_t szNumPoints;

public:
    explicit GeneratorCmdParamIn(size_t szNumPointsIn) : szNumPoints(szNumPointsIn) {};

    /**
     * Getter/Setter
     */
    size_t getNumPoints() const { return szNumPoints;}
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GeneratorClusterCmdParamIn : public GeneratorCmdParamIn
{
    TYPE fRadius;
    size_t szNumClusters;
public:
    explicit GeneratorClusterCmdParamIn(size_t szNumPointsIn, size_t szNumClustersIn, TYPE fRadiusIn) : GeneratorCmdParamIn(szNumPointsIn),
                                                                                                        szNumClusters(szNumClustersIn),
                                                                                                        fRadius(fRadiusIn)
                                                                                                        {};
    /**
     * Getter/Setter
     */
    TYPE getFRadius() const { return fRadius; }
    size_t getSzNumClusters() const { return szNumClusters; }
};

#endif //DELAUNAY_COMMANDPARAMIN_H
