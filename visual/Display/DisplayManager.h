//
// Created by delaunay on 25/3/20.
//

#ifndef DELAUNAY_DISPLAYMANAGER_H
#define DELAUNAY_DISPLAYMANAGER_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayService.h"
#include "Displayable.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayManager
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    vector<Displayable *> vDisplayables;

    /*******************************************************************************************************************
     * Private methods declaration
     *******************************************************************************************************************/
    /**
      * @fn     reset
      * @brief  Reset manager state
     */
    inline void reset()
    {
        for (auto item : vDisplayables)
        {
            delete item;
        }
        vDisplayables.clear();
    };

    void draw();
public:

    DisplayManager() : vDisplayables(0) {};

    void process();

    /**
     * @fn      add
     * @brief   Add an element to be displayed.
     *
     * @param   displayable
     */
    void add(Displayable *displayable) { vDisplayables.push_back(displayable); };
};


#endif //DELAUNAY_DISPLAYMANAGER_H
