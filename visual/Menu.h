/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "MenuOption.h"
#include "Status.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Menu
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	int	menu_Id;
	int	sub_Menu_Input;
	int	sub_Menu_Id2;
	int	sub_Menu_Id3;
	int	sub_Menu_Id4;
	int	sub_Menu_Id5;
	int	subMenuRead;
	int	subMenuGenerate;

public:

    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
	Menu();

    /**
     * @fn      updateMenu
     * @brief   Updates menu entries and updates status
     *
     * @param   status  (IN/OUT)    Status data
     */
    void updateMenu(Status *status);

    /**
     * @fn      getMenuOption
     * @brief   Get last menu option
     * @return  menu option
     */
	static MenuOption getMenuOption();

	/**
	 * @fn      resetMenuOption
	 * @brief   Reset menu option
	 */
	static void	resetMenuOption();
};


