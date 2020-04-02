//
// Created by delaunay on 1/4/20.
//

#ifndef DELAUNAY_CONFIGSERVICE_H
#define DELAUNAY_CONFIGSERVICE_H

#include "Config.h"


class ConfigService
{
public:
    size_t getNumPoints() { return Config::getNPoints(); };
    size_t getNumClusters() { return Config::getNClusters(); };
    size_t getRadius() { return Config::getRadius(); };
    size_t getMinLengthEdge() { return Config::getMinLengthEdge(); };
};


#endif //DELAUNAY_CONFIGSERVICE_H
