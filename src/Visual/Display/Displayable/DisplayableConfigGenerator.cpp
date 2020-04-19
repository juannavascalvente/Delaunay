/***********************************************************************************************************************
* Include section
***********************************************************************************************************************/
#include "DisplayableConfigGenerator.h"


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
