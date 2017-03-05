/*
 * Menu.h
 *
 *  Created on: Jul 4, 2016
 *      Author: juan
 */

#include "Status.h"

/**************************************************************************
* Defines section
**************************************************************************/
// Default menu option.
#define DEFAULT_OPTION 		200

#define	PARAMETERS				1
#define RANDOMLY 				11
#define CLUSTER					12
#define READ_POINTS_FLAT_FILE	13
#define READ_POINTS_DCEL_FILE	14
#define	READ_DCEL				15
#define	READ_DELAUNAY			16
#define	READ_VORONOI			17
#define READ_GABRIEL 			18
#define STAR_TRIANGULATION  	20
#define CONVEX_HULL         	30
#define DELAUNAY            	40
#define CIRCUMCENTRES       	50
#define CORRIDOR            	51
#define EDGE_CIRCLES        	52
#define VORONOI             	60
#define GABRIEL             	61
#define CLOSEST_POINT       	62
#define TWO_CLOSEST         	63
#define FIND_FACE				64
#define FILTER_EDGES			65
#define TRIANGULATION_PATH		66
#define VORONOI_PATH			67
#define DCEL_INFO         		71
#define VORONOI_INFO          	72
#define CLEAR               	80
#define	SHAKE_POINTS			81
#define	CHECK_DCEL  			82
#define ZOOM 					90
#define QUIT                	100

#define WRITE_POINTS 			21
#define WRITE_DCEL				22
#define WRITE_DELAUNAY 			23
#define WRITE_VORONOI 			24
#define WRITE_GABRIEL 			25


//****************************************************************************
//                           	MENU CLASS
//****************************************************************************
class Menu
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	int	menu_Id;
	int	sub_Menu_Input;
	int	sub_Menu_Id2;
	int	sub_Menu_Id3;
	int	sub_Menu_Id4;
	int	sub_Menu_Id5;

	Status	*status;
public:

	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Menu();
	Menu(Status *status);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	void 	updateMenu();

	int		getMenuOption();
	void	setMenuOption(int value);
	void	resetMenuOption();
};


