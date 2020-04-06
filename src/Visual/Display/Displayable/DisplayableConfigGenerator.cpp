//
// Created by delaunay on 27/3/20.
//

/***********************************************************************************************************************
* Include section
***********************************************************************************************************************/
#include "Visual/Display/Displayable/DisplayableConfigGenerator.h"


/***********************************************************************************************************************
* Private members initialization
***********************************************************************************************************************/
size_t DisplayableConfigGenerator::szCounter = 0;
vector<DisplayableColor> DisplayableConfigGenerator::vColors = {DisplayableColorRed(),
                                                                DisplayableColorGreen(),
                                                                DisplayableColorBlue(),
                                                                DisplayableColorLightBlue(),
                                                                DisplayableColorYellow(),
                                                                DisplayableColorWhite()
};
