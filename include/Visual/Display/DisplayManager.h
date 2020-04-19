#ifndef DELAUNAY_DISPLAYMANAGER_H
#define DELAUNAY_DISPLAYMANAGER_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayService.h"
#include "Displayable.h"
#include "Menu.h"

#include <vector>
#include <Store/StoreService.h>

using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayManager
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    vector<Displayable *>   vDisplayables;
    vector<Displayable *>   vDisplayablesSaved;
    Menu 			        menu;		    // Menu object.

    /*******************************************************************************************************************
     * Private methods declaration
     *******************************************************************************************************************/
    /**
      * @fn     reset
      * @brief  Reset manager state
     */
    inline void reset()
    {
        // Remove displayable objects
        for (auto item : vDisplayables)
        {
            delete item;
        }
        vDisplayables.clear();

        // Reset displayable configuration
        DisplayableConfigGenerator::reset();
    };

    void draw();
public:

    /*******************************************************************************************************************
     * Public methods declaration
     *******************************************************************************************************************/
    DisplayManager(int argc, char **argv, StoreService *storeServ) : vDisplayables(0), menu(*storeServ)
    {
        // Initialize display
        DisplayService::init(argc, argv);
    };


    /**
     * @fn      process
     * @brief   Process items to be siplayed in the screen
     */
    void process();


    /**
     * @fn      add
     * @brief   Add an element to be displayed.
     *
     * @param   displayable
     */
    void add(Displayable *displayable) { vDisplayables.push_back(displayable); };


    /**
     * @fn      add
     * @brief   Add a vector of displayables
     *
     * @param   vector of displayable
     */
    void add(vector<Displayable*> &vDisplayablesIn) { vDisplayables = vDisplayablesIn; };


    /**
     * @fn      startLoop
     * @brief   Starts glu main loop
     */
    static void startLoop() { DisplayService::startLoop(); }


    /**
     * @fn      setLoopFunction
     * @brief   Set function to be executed in glu main loop
     */
    static void setLoopFunction(void (* f)()) { glutDisplayFunc((*f)); }

    /**
     * @fn      updateMenu
     * @brief   Updae menu entries
     */
    void updateMenu() { menu.updateMenu(); }
};


#endif //DELAUNAY_DISPLAYMANAGER_H
