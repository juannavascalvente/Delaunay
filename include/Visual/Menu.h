/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "MenuOption.h"
#include "StoreService.h"

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

    StoreService service;

    int createGenerateMenu();
    int createReadMenu();

public:

    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
	explicit Menu(StoreService &s);

    /**
     * @fn      updateMenu
     * @brief   Updates menu entries and updates status
     */
    void updateMenu();

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


