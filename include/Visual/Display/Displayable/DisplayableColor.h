//
// Created by delaunay on 27/3/20.
//

#ifndef DELAUNAY_DISPLAYABLECOLOR_H
#define DELAUNAY_DISPLAYABLECOLOR_H


/***********************************************************************************************************************
* Defines section
***********************************************************************************************************************/
#define MAX_COLOR_VALUE                 (1.0)
#define MIN_COLOR_VALUE                 (0.0)
#define DEFAULT_COLOR_VALUE             (MAX_COLOR_VALUE)


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayableColor
{
    float fRed;
    float fGreen;
    float fBlue;
public:
    DisplayableColor() : fRed(DEFAULT_COLOR_VALUE), fGreen(DEFAULT_COLOR_VALUE), fBlue(DEFAULT_COLOR_VALUE) {};
    DisplayableColor(float fRedIn, float fGreenIn, float fBlueIn) : fRed(fRedIn), fGreen(fGreenIn), fBlue(fBlueIn) {};

    /*******************************************************************************************************************
    * Get functions
    *******************************************************************************************************************/
    float getRed()   const { return fRed; };
    float getGreen() const { return fGreen; };
    float getBlue()  const { return fBlue; };
};


class DisplayableColorRed : public DisplayableColor
{
public:
    DisplayableColorRed() : DisplayableColor(MAX_COLOR_VALUE, MIN_COLOR_VALUE, MIN_COLOR_VALUE) {};
};


class DisplayableColorGreen : public DisplayableColor
{
public:
    DisplayableColorGreen() : DisplayableColor(MIN_COLOR_VALUE, MAX_COLOR_VALUE, MIN_COLOR_VALUE) {};
};


class DisplayableColorBlue : public DisplayableColor
{
public:
    DisplayableColorBlue() : DisplayableColor(MIN_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE) {};
};


class DisplayableColorLightBlue : public DisplayableColor
{
public:
    DisplayableColorLightBlue() : DisplayableColor(MIN_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE) {};
};


class DisplayableColorYellow : public DisplayableColor
{
public:
    DisplayableColorYellow() : DisplayableColor(MAX_COLOR_VALUE, MAX_COLOR_VALUE, MIN_COLOR_VALUE) {};
};


class DisplayableColorWhite : public DisplayableColor
{
public:
    DisplayableColorWhite() : DisplayableColor(MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE) {};
};


#endif //DELAUNAY_DISPLAYABLECOLOR_H
