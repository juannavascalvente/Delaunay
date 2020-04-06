//
// Created by delaunay on 27/3/20.
//

#ifndef DELAUNAY_DISPLAYABLECONFIGGENERATOR_H
#define DELAUNAY_DISPLAYABLECONFIGGENERATOR_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayableColor.h"
#include "DisplayableConfig.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayableConfigGenerator
{
    /*******************************************************************************************************************
    * Private members
    *******************************************************************************************************************/
    static size_t szCounter;
    static vector<DisplayableColor> vColors;

    /*******************************************************************************************************************
    * Private methods
    *******************************************************************************************************************/
    /**
     * @fn      getNextColor
     * @brief   Get next color and updates counter to next color
     *
     * @return  Next color
     */
    static DisplayableColor getNextColor()
    {
        szCounter %= vColors.size();
        return vColors[szCounter++];
    }
public:

    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    /**
     * @fn      getNextConfig
     * @brief   Get next displayable configuration
     *
     * @return  Next displayable configuration
     */
    static DisplayableConfig getNextConfig()
    {
        DisplayableConfig config;
        config.setColor(getNextColor());

        return config;
    }

    /**
     * @fn      reset
     * @brief   Reset color counter
     */
    static void reset() { szCounter = 0; };
};


#endif //DELAUNAY_DISPLAYABLECONFIGGENERATOR_H
