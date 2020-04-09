//
// Created by delaunay on 26/3/20.
//

#ifndef DELAUNAY_DISPLAYABLECONFIG_H
#define DELAUNAY_DISPLAYABLECONFIG_H

/***********************************************************************************************************************
* Includes section
***********************************************************************************************************************/
#include "DisplayableColor.h"

#include <cstring>


/***********************************************************************************************************************
* Defines section
***********************************************************************************************************************/
#define DEFAULT_POINT_SIZE          (1.0)
#define DEFAULT_LINE_SIZE           (1.0)


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayableConfig
{
    float fPointSize;
    float fLineSize;
    DisplayableColor color;
public:
    DisplayableConfig() : fPointSize(DEFAULT_POINT_SIZE), fLineSize(DEFAULT_LINE_SIZE), color() {};

    /**
     * @fn      setPointSize
     * @brief   Sets point size
     *
     * @param   fSize       (IN) Point size
     */
    void setPointSize(float fSize) { fPointSize = fSize; }

    /**
     * @fn      setLineSize
     * @brief   Sets line size
     *
     * @param   fSize       (IN) Point size
     */
    void setLineSize(float fSize) { fLineSize = fSize; }

    /**
     * @fn      setColor
     * @brief   Sets color
     *
     * @param   arr_fColorsIn   (IN) RGB color vector
     */
    void setColor(DisplayableColor colorIn)
    {
        color = colorIn;
    }

    /**
     * @fn      getPointSize
     * @brief   Gets point size
     *
     * @return  Point size
     */
    float getPointSize() { return fPointSize; }

    /**
     * @fn      getLineSize
     * @brief   Gets line size
     *
     * @return  Line size
     */
    float getLineSize() { return fLineSize; }

    /**
     * @fn      getColor
     * @brief   Gets RGB color vector
     *
     * @return  RGB color vector
     */
    DisplayableColor getColor() { return color; }
};


#endif //DELAUNAY_DISPLAYABLECONFIG_H
