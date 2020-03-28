//
// Created by delaunay on 25/3/20.
//
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayManager.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void DisplayManager::process()
{
    // Clear display.
    DisplayService::clear();

    // Draw figures
    draw();

    // Full images into display.
    DisplayService::flush();

    // Remove figures
    reset();
}


/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/**
 * @fn      draw
 * @brief   DisplayService all images in display
 */
void DisplayManager::draw()
{
    // Iterate all elements and display them
    for (auto item : vDisplayables)
    {
        item->display();
    }
}

